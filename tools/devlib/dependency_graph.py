"""Build and validate the C++ header dependency graph.

The include directives in ``cp/**/*.hpp`` are the only source of dependency
edges.  Metadata deliberately contains no dependency list; it only annotates
headers with properties such as ``layer`` and ``status``.
"""

from __future__ import annotations

from collections import deque
from collections.abc import Iterable, Mapping
from pathlib import Path
import re
from typing import Any, TypeAlias


Graph: TypeAlias = dict[str, set[str]]
Issue: TypeAlias = dict[str, Any]

_LOCAL_INCLUDE_RE = re.compile(
    r'^\s*#\s*include\s*"(?P<path>(?:[a-z0-9_]+/)*[a-z][a-z0-9_]*\.hpp)"\s*$'
)
_FORBIDDEN_STABLE_DEPENDENCY_STATUSES = {
    "beta",
    "experimental",
    "deprecated",
}


def discover_headers(repo_root: str | Path) -> list[str]:
    """Return repository-relative ``cp/**/*.hpp`` paths in stable order."""

    root = Path(repo_root)
    cp_root = root / "cp"
    if not cp_root.is_dir():
        return []
    return sorted(
        path.relative_to(root).as_posix()
        for path in cp_root.rglob("*.hpp")
        if path.is_file()
    )


def parse_dependencies(
    header_path: str | Path, repo_root: str | Path | None = None
) -> list[str]:
    """Read one header and return repository paths for its cp-root includes.

    ``header_path`` may be absolute.  When it is repository-relative,
    ``repo_root`` supplies its base directory.  Commented-out directives are
    ignored.  Duplicate directives are collapsed because the graph models
    edges rather than include occurrences.
    """

    path = Path(header_path)
    if not path.is_absolute() and repo_root is not None:
        path = Path(repo_root) / path

    text = path.read_text(encoding="utf-8")
    uncommented = _mask_cpp_comments(text)
    dependencies: set[str] = set()
    for line in uncommented.splitlines():
        match = _LOCAL_INCLUDE_RE.fullmatch(line)
        if match:
            dependencies.add("cp/" + match.group("path"))
    return sorted(dependencies)


def build_graph(
    repo_root: str | Path, headers: Iterable[str | Path] | None = None
) -> Graph:
    """Build a dependency graph keyed by repository-relative header path."""

    root = Path(repo_root)
    relative_headers = (
        discover_headers(root)
        if headers is None
        else sorted({_repo_relative_path(root, path) for path in headers})
    )
    return {
        header: set(parse_dependencies(header, root))
        for header in relative_headers
    }


def validate_graph(
    repo_root: str | Path,
    metadata: Mapping[str, Mapping[str, Any]],
) -> list[Issue]:
    """Validate include, layer, aggregate-header, and status constraints.

    Every returned issue is a JSON-friendly dictionary containing at least
    ``severity``, ``code``, ``path``, and ``message``.  Independent violations
    on the same edge are intentionally reported independently.
    """

    graph = build_graph(repo_root)
    known_headers = set(graph)
    normalized_metadata = {
        _normalize_path(path): entry for path, entry in metadata.items()
    }
    issues: list[Issue] = []

    for source in sorted(graph):
        source_meta = normalized_metadata.get(source, {})
        source_layer = _metadata_layer(source_meta)
        source_status = _metadata_status(source_meta)

        for dependency in sorted(graph[source]):
            edge_fields = {"path": source, "dependency": dependency}

            if dependency not in known_headers:
                issues.append(
                    _issue(
                        "unknown_include",
                        **edge_fields,
                        message=f"{source} includes missing header {dependency}",
                    )
                )
                continue

            if dependency == source:
                issues.append(
                    _issue(
                        "self_dependency",
                        **edge_fields,
                        message=f"{source} includes itself",
                    )
                )

            if not _is_all_header(source) and _is_all_header(dependency):
                issues.append(
                    _issue(
                        "aggregate_dependency",
                        **edge_fields,
                        message=(
                            f"normal header {source} must not depend on aggregate "
                            f"header {dependency}"
                        ),
                    )
                )

            dependency_meta = normalized_metadata.get(dependency, {})
            dependency_layer = _metadata_layer(dependency_meta)
            if (
                source_layer is not None
                and dependency_layer is not None
                and dependency_layer >= source_layer
            ):
                relation = "same" if dependency_layer == source_layer else "higher"
                issues.append(
                    _issue(
                        "layer_violation",
                        **edge_fields,
                        source_layer=source_layer,
                        dependency_layer=dependency_layer,
                        message=(
                            f"layer {source_layer} header {source} depends on {relation} "
                            f"layer {dependency_layer} header {dependency}"
                        ),
                    )
                )

            dependency_status = _metadata_status(dependency_meta)
            if (
                source_status == "stable"
                and dependency_status in _FORBIDDEN_STABLE_DEPENDENCY_STATUSES
            ):
                issues.append(
                    _issue(
                        "status_violation",
                        **edge_fields,
                        source_status=source_status,
                        dependency_status=dependency_status,
                        message=(
                            f"stable header {source} depends on {dependency_status} "
                            f"header {dependency}"
                        ),
                    )
                )

    for component in _cyclic_components(graph):
        cycle = _cycle_witness(graph, component)
        issues.append(
            _issue(
                "dependency_cycle",
                path=cycle[0],
                nodes=sorted(component),
                cycle=cycle,
                message="dependency cycle: " + " -> ".join(cycle),
            )
        )

    return issues


def affected(graph: Mapping[str, Iterable[str]], target: str) -> list[str]:
    """Return ``target`` and every header that transitively depends on it."""

    target = _normalize_path(target)
    nodes = set(graph)
    reverse: dict[str, set[str]] = {node: set() for node in nodes}
    for source, dependencies in graph.items():
        for dependency in dependencies:
            reverse.setdefault(dependency, set()).add(source)
            nodes.add(dependency)

    if target not in nodes:
        return []

    result = {target}
    pending = [target]
    while pending:
        current = pending.pop()
        for dependent in reverse.get(current, ()):
            if dependent not in result:
                result.add(dependent)
                pending.append(dependent)
    return sorted(result)


def shortest_path(
    graph: Mapping[str, Iterable[str]], start: str, goal: str
) -> list[str] | None:
    """Return a shortest directed dependency path, or ``None`` if absent."""

    start = _normalize_path(start)
    goal = _normalize_path(goal)
    if start == goal:
        return [start]
    if start not in graph:
        return None

    parent: dict[str, str | None] = {start: None}
    queue = deque([start])
    while queue:
        current = queue.popleft()
        for dependency in sorted(graph.get(current, ())):
            if dependency in parent:
                continue
            parent[dependency] = current
            if dependency == goal:
                return _reconstruct_path(parent, goal)
            queue.append(dependency)
    return None


def why_path(
    graph: Mapping[str, Iterable[str]], source: str, dependency: str
) -> list[str] | None:
    """Explain why ``source`` depends on ``dependency`` with a shortest path."""

    return shortest_path(graph, source, dependency)


def _issue(code: str, *, message: str, **fields: Any) -> Issue:
    return {"severity": "error", "code": code, **fields, "message": message}


def _repo_relative_path(root: Path, path: str | Path) -> str:
    candidate = Path(path)
    if candidate.is_absolute():
        candidate = candidate.relative_to(root)
    return _normalize_path(candidate.as_posix())


def _normalize_path(path: str | Path) -> str:
    value = Path(path).as_posix()
    while value.startswith("./"):
        value = value[2:]
    return value


def _metadata_layer(metadata: Mapping[str, Any]) -> int | None:
    layer = metadata.get("layer")
    return layer if isinstance(layer, int) and not isinstance(layer, bool) else None


def _metadata_status(metadata: Mapping[str, Any]) -> str | None:
    status = metadata.get("status")
    return status.lower() if isinstance(status, str) else None


def _is_all_header(path: str) -> bool:
    return Path(path).name == "all.hpp"


def _reconstruct_path(parent: Mapping[str, str | None], goal: str) -> list[str]:
    path: list[str] = []
    current: str | None = goal
    while current is not None:
        path.append(current)
        current = parent[current]
    path.reverse()
    return path


def _cyclic_components(graph: Mapping[str, Iterable[str]]) -> list[set[str]]:
    """Return non-trivial strongly connected components in stable order."""

    nodes = sorted(graph)
    known = set(nodes)
    adjacency = {
        node: sorted(dependency for dependency in graph[node] if dependency in known)
        for node in nodes
    }

    visited: set[str] = set()
    finish_order: list[str] = []
    for root in nodes:
        if root in visited:
            continue
        visited.add(root)
        stack: list[tuple[str, int]] = [(root, 0)]
        while stack:
            node, index = stack[-1]
            neighbors = adjacency[node]
            if index < len(neighbors):
                neighbor = neighbors[index]
                stack[-1] = (node, index + 1)
                if neighbor not in visited:
                    visited.add(neighbor)
                    stack.append((neighbor, 0))
            else:
                finish_order.append(node)
                stack.pop()

    reverse: dict[str, list[str]] = {node: [] for node in nodes}
    for source, dependencies in adjacency.items():
        for dependency in dependencies:
            reverse[dependency].append(source)
    for dependencies in reverse.values():
        dependencies.sort()

    assigned: set[str] = set()
    components: list[set[str]] = []
    for root in reversed(finish_order):
        if root in assigned:
            continue
        component = {root}
        assigned.add(root)
        pending = [root]
        while pending:
            node = pending.pop()
            for neighbor in reverse[node]:
                if neighbor not in assigned:
                    assigned.add(neighbor)
                    component.add(neighbor)
                    pending.append(neighbor)
        if len(component) > 1:
            components.append(component)

    return sorted(components, key=lambda component: min(component))


def _cycle_witness(graph: Mapping[str, Iterable[str]], component: set[str]) -> list[str]:
    start = min(component)
    for neighbor in sorted(set(graph[start]) & component):
        return_path = shortest_path(
            {
                node: set(graph[node]) & component
                for node in component
            },
            neighbor,
            start,
        )
        if return_path is not None:
            return [start, *return_path]
    # This is unreachable for a non-trivial strongly connected component.
    raise RuntimeError("failed to construct a cycle witness")


def _mask_cpp_comments(text: str) -> str:
    """Replace C/C++ comments with spaces while preserving newlines/strings."""

    result = list(text)
    index = 0
    in_block_comment = False
    quote: str | None = None
    escaped = False

    while index < len(text):
        char = text[index]
        next_char = text[index + 1] if index + 1 < len(text) else ""

        if in_block_comment:
            if char == "*" and next_char == "/":
                result[index] = " "
                result[index + 1] = " "
                index += 2
                in_block_comment = False
            else:
                if char not in "\r\n":
                    result[index] = " "
                index += 1
            continue

        if quote is not None:
            if escaped:
                escaped = False
            elif char == "\\":
                escaped = True
            elif char == quote:
                quote = None
            index += 1
            continue

        if char in {'"', "'"}:
            quote = char
            index += 1
        elif char == "/" and next_char == "*":
            result[index] = " "
            result[index + 1] = " "
            index += 2
            in_block_comment = True
        elif char == "/" and next_char == "/":
            while index < len(text) and text[index] not in "\r\n":
                result[index] = " "
                index += 1
        else:
            index += 1

    return "".join(result)


__all__ = [
    "Graph",
    "Issue",
    "affected",
    "build_graph",
    "discover_headers",
    "parse_dependencies",
    "shortest_path",
    "validate_graph",
    "why_path",
]
