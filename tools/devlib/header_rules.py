"""Rules and scaffolding for ``cp/**/*.hpp`` headers.

The command-line interface deliberately lives elsewhere.  This module keeps the
filesystem-independent validation and rendering logic small enough to reuse
from tests and from future developer tooling.
"""

from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
import re
from typing import Iterable


_SNAKE_CASE_RE = re.compile(r"^[a-z][a-z0-9]*(?:_[a-z0-9]+)*$")
_REPOSITORY_HEADER_PATH_RE = re.compile(
    r"cp/(?:[a-z0-9_]+/)*[a-z][a-z0-9]*(?:_[a-z0-9]+)*\.hpp"
)
_INCLUDE_PATH_TEXT_RE = re.compile(
    r"(?:[a-z0-9_]+/)*[a-z][a-z0-9]*(?:_[a-z0-9]+)*\.hpp"
)
_LOCAL_INCLUDE_RE = re.compile(
    rf'^#include "({_INCLUDE_PATH_TEXT_RE.pattern})"$'
)
_DIRECTIVE_RE = re.compile(r"^\s*#\s*([A-Za-z_][A-Za-z0-9_]*)\b")
_USING_NAMESPACE_STD_RE = re.compile(r"\busing\s+namespace\s+std\s*;")


class HeaderRuleError(ValueError):
    """Raised when a requested header scaffold violates a repository rule."""


@dataclass(frozen=True, order=True)
class HeaderIssue:
    """A single lint finding.

    ``line`` is one-based.  File-wide byte/encoding findings use line 1 so all
    issues remain straightforward to display in ``path:line`` form.
    """

    line: int
    code: str
    message: str
    path: Path

    def __str__(self) -> str:
        return f"{self.path}:{self.line}: {self.code}: {self.message}"


@dataclass(frozen=True)
class HeaderScaffold:
    """A validated, not-yet-written header scaffold."""

    path: Path
    relative_path: Path
    dependencies: tuple[str, ...]
    content: str


@dataclass(frozen=True)
class HeaderWriteResult:
    """Result of writing, or dry-running, one header scaffold."""

    path: Path
    content: str
    written: bool
    dry_run: bool


def _repo_relative_path(repo_root: Path, path: Path) -> tuple[Path, Path]:
    """Return ``(absolute_path, repo_relative_path)`` without requiring a file."""

    root = repo_root.resolve()
    candidate = path if path.is_absolute() else root / path
    candidate = candidate.resolve()
    try:
        relative = candidate.relative_to(root)
    except ValueError as exc:
        raise HeaderRuleError(f"header path escapes repository root: {path}") from exc
    return candidate, relative


def validate_header_path(repo_root: Path, header: Path) -> tuple[Path, Path]:
    """Validate a destination path and return absolute and relative forms.

    Valid public paths are below ``cp/``, end in ``.hpp``, and have a
    snake_case filename.  The path need not exist, which makes this suitable
    for scaffold planning and dry runs.
    """

    absolute, relative = _repo_relative_path(repo_root, header)
    if len(relative.parts) < 2 or relative.parts[0] != "cp":
        raise HeaderRuleError(f'header must be below "cp/": {relative.as_posix()}')
    if relative.suffix != ".hpp":
        raise HeaderRuleError(f"header must have a .hpp suffix: {relative.as_posix()}")
    for directory in relative.parts[1:-1]:
        if not _SNAKE_CASE_RE.fullmatch(directory):
            raise HeaderRuleError(
                f"header directory must be snake_case: {directory}"
            )
    if not _SNAKE_CASE_RE.fullmatch(relative.stem):
        raise HeaderRuleError(
            f"header filename must be snake_case: {relative.name}"
        )
    return absolute, relative


def normalize_dependencies(
    repo_root: Path,
    dependencies: Iterable[str | Path],
    *,
    require_exists: bool = True,
) -> tuple[str, ...]:
    """Validate dependencies and return unique repository paths in sort order.

    Layer validation is intentionally left to the dependency-graph module.  A
    caller can set ``require_exists=False`` when rendering an isolated preview.
    Duplicate dependencies are rejected instead of being silently hidden.
    """

    normalized: list[str] = []
    seen: set[str] = set()
    for dependency in dependencies:
        absolute, relative = validate_header_path(repo_root, Path(dependency))
        relative_text = relative.as_posix()
        if relative_text in seen:
            raise HeaderRuleError(f"duplicate dependency: {relative_text}")
        if require_exists and not absolute.is_file():
            raise HeaderRuleError(f"dependency does not exist: {relative_text}")
        seen.add(relative_text)
        normalized.append(relative_text)
    return tuple(sorted(normalized))


def render_header(dependencies: Iterable[str | Path] = ()) -> str:
    """Render the exact initial contents of a ``cp`` header.

    Dependencies are repository-relative ``cp/*.hpp`` paths, while emitted
    include directives are relative to the ``cp`` include root.
    """

    dependency_texts: list[str] = []
    seen: set[str] = set()
    for dependency in dependencies:
        text = Path(dependency).as_posix()
        if not _REPOSITORY_HEADER_PATH_RE.fullmatch(text):
            raise HeaderRuleError(f"invalid dependency path: {text}")
        if text in seen:
            raise HeaderRuleError(f"duplicate dependency: {text}")
        seen.add(text)
        dependency_texts.append(Path(text).relative_to("cp").as_posix())

    lines = ["#pragma once", "#include <bits/stdc++.h>"]
    lines.extend(f'#include "{path}"' for path in sorted(dependency_texts))
    lines.extend(["", "namespace poe {", "}"])
    return "\n".join(lines) + "\n"


def plan_new_header(
    repo_root: Path,
    header: Path,
    dependencies: Iterable[str | Path] = (),
    *,
    require_dependencies: bool = True,
) -> HeaderScaffold:
    """Validate and plan a new header without modifying the filesystem."""

    absolute, relative = validate_header_path(repo_root, header)
    normalized = normalize_dependencies(
        repo_root, dependencies, require_exists=require_dependencies
    )
    if relative.as_posix() in normalized:
        raise HeaderRuleError(f"header cannot include itself: {relative.as_posix()}")
    return HeaderScaffold(
        path=absolute,
        relative_path=relative,
        dependencies=normalized,
        content=render_header(normalized),
    )


def write_header(
    scaffold: HeaderScaffold,
    *,
    dry_run: bool = False,
    overwrite: bool = False,
) -> HeaderWriteResult:
    """Write a planned scaffold atomically enough for an interactive CLI.

    Existing files are rejected by default, including in dry-run mode, so a
    preview cannot claim that a subsequent real invocation is safe.  The
    ``overwrite`` escape hatch is explicit for callers that intentionally offer
    such a mode; a normal ``new-header`` command should leave it false.
    """

    if scaffold.path.exists() and not overwrite:
        raise FileExistsError(f"refusing to overwrite existing file: {scaffold.path}")
    if dry_run:
        return HeaderWriteResult(
            path=scaffold.path,
            content=scaffold.content,
            written=False,
            dry_run=True,
        )

    scaffold.path.parent.mkdir(parents=True, exist_ok=True)
    mode = "w" if overwrite else "x"
    with scaffold.path.open(mode, encoding="utf-8", newline="\n") as output:
        output.write(scaffold.content)
    return HeaderWriteResult(
        path=scaffold.path,
        content=scaffold.content,
        written=True,
        dry_run=False,
    )


def _mask_comments_and_literals(text: str) -> str:
    """Replace comments and C++ literals with spaces while preserving newlines.

    This is intentionally a small lexer, not a C++ parser.  It handles line and
    block comments, escaped ordinary string/character literals, and raw string
    literals.  That is sufficient to prevent the common false positives where
    examples mention ``#include``, macros, or ``using namespace std``.
    """

    result = list(text)
    length = len(text)
    index = 0

    def mask(start: int, end: int) -> None:
        for position in range(start, end):
            if result[position] != "\n":
                result[position] = " "

    while index < length:
        if text.startswith("//", index):
            end = text.find("\n", index + 2)
            if end == -1:
                end = length
            mask(index, end)
            index = end
            continue

        if text.startswith("/*", index):
            end_marker = text.find("*/", index + 2)
            end = length if end_marker == -1 else end_marker + 2
            mask(index, end)
            index = end
            continue

        raw_match = re.match(r'(?:u8|u|U|L)?R"([^ ()\\\t\r\n]{0,16})\(', text[index:])
        if raw_match is not None:
            delimiter = raw_match.group(1)
            terminator = ")" + delimiter + '"'
            content_start = index + raw_match.end()
            end_marker = text.find(terminator, content_start)
            end = length if end_marker == -1 else end_marker + len(terminator)
            mask(index, end)
            index = end
            continue

        literal_match = re.match(r'(?:u8|u|U|L)?(["\'])', text[index:])
        if literal_match is not None:
            quote = literal_match.group(1)
            cursor = index + literal_match.end()
            while cursor < length:
                if text[cursor] == "\\":
                    cursor += 2
                    continue
                if text[cursor] == quote:
                    cursor += 1
                    break
                cursor += 1
            mask(index, min(cursor, length))
            index = min(cursor, length)
            continue

        index += 1

    return "".join(result)


def lint_header(repo_root: Path, header: Path) -> list[HeaderIssue]:
    """Lint one header against the repository's exact physical-file rules.

    The returned issues are ordered by line and code.  The function never
    raises for malformed file contents; path and read failures are findings so
    a caller can lint many headers in one pass.
    """

    root = repo_root.resolve()
    display_path = header
    try:
        absolute, relative = _repo_relative_path(root, header)
        display_path = relative
    except HeaderRuleError:
        absolute = header if header.is_absolute() else root / header

    issues: list[HeaderIssue] = []

    def add(line: int, code: str, message: str) -> None:
        issues.append(HeaderIssue(line, code, message, display_path))

    try:
        raw = absolute.read_bytes()
    except OSError as exc:
        add(1, "read_error", str(exc))
        return issues

    has_bom = raw.startswith(b"\xef\xbb\xbf")
    if has_bom:
        add(1, "utf8_bom", "UTF-8 BOM is not allowed")
    if b"\r\n" in raw or b"\r" in raw:
        add(1, "line_endings", "use LF line endings; CR/CRLF is not allowed")
    if not raw.endswith(b"\n"):
        add(1, "missing_eof_newline", "file must end with a newline")

    try:
        text = raw.decode("utf-8-sig" if has_bom else "utf-8")
    except UnicodeDecodeError as exc:
        add(1, "invalid_utf8", f"file is not valid UTF-8: {exc}")
        text = raw.decode("utf-8-sig" if has_bom else "utf-8", errors="replace")

    # Normalize only for structural analysis.  The byte-level CR finding above
    # still ensures a CRLF file cannot pass.
    text = text.replace("\r\n", "\n").replace("\r", "\n")
    lines = text.splitlines()
    is_aggregate = absolute.name == "all.hpp"

    if len(lines) < 1 or lines[0] != "#pragma once":
        add(1, "pragma_once", 'physical line 1 must be exactly "#pragma once"')
    if not is_aggregate and (len(lines) < 2 or lines[1] != "#include <bits/stdc++.h>"):
        add(
            2,
            "bits_include",
            'physical line 2 must be exactly "#include <bits/stdc++.h>"',
        )

    dependencies: list[tuple[int, str]] = []
    cursor = 1 if is_aggregate else 2
    while cursor < len(lines):
        match = _LOCAL_INCLUDE_RE.fullmatch(lines[cursor])
        if match is None:
            break
        dependencies.append((cursor + 1, match.group(1)))
        cursor += 1

    dependency_names = [name for _, name in dependencies]
    if dependency_names != sorted(dependency_names):
        add(3, "include_order", "local includes must be sorted lexicographically")
    seen_dependencies: set[str] = set()
    for line_number, dependency in dependencies:
        if dependency in seen_dependencies:
            add(line_number, "duplicate_include", f"duplicate include: {dependency}")
        seen_dependencies.add(dependency)
        if dependency.startswith("cp/"):
            add(
                line_number,
                "include_root_prefix",
                'local includes must omit the leading "cp/"',
            )
            continue
        dependency_path = root / "cp" / dependency
        if not dependency_path.is_file():
            add(line_number, "missing_dependency", f"dependency does not exist: {dependency}")

    if is_aggregate:
        if cursor != len(lines):
            add(cursor + 1, "aggregate_body", "all.hpp must contain only local includes")
        implementation_start = cursor
    else:
        if cursor >= len(lines) or lines[cursor] != "":
            add(cursor + 1, "include_separator", "include block must be followed by one blank line")
            implementation_start = cursor
        else:
            implementation_start = cursor + 1
            if implementation_start >= len(lines) or lines[implementation_start] == "":
                add(
                    implementation_start + 1,
                    "extra_blank_line",
                    "there must be exactly one blank line before implementation",
                )
                while implementation_start < len(lines) and lines[implementation_start] == "":
                    implementation_start += 1

    masked = _mask_comments_and_literals(text)
    masked_lines = masked.splitlines()
    allowed_include_lines = {line for line, _ in dependencies}
    if not is_aggregate and len(lines) >= 2 and lines[1] == "#include <bits/stdc++.h>":
        allowed_include_lines.add(2)
    for line_number, line in enumerate(masked_lines, start=1):
        directive_match = _DIRECTIVE_RE.match(line)
        if directive_match is None:
            continue
        directive = directive_match.group(1)
        if directive in {"define", "undef"}:
            add(line_number, "macro_directive", f"#{directive} is not allowed in headers")
        if directive == "include" and line_number not in allowed_include_lines:
            original_line = lines[line_number - 1] if line_number <= len(lines) else ""
            if re.match(r"^\s*#\s*include\s*<", original_line):
                add(line_number, "extra_system_include", "only <bits/stdc++.h> is allowed")
            else:
                add(
                    line_number,
                    "include_after_preamble",
                    "includes are only allowed in the contiguous preamble",
                )

    for match in _USING_NAMESPACE_STD_RE.finditer(masked):
        line_number = masked.count("\n", 0, match.start()) + 1
        add(line_number, "using_namespace_std", '"using namespace std;" is not allowed')

    # ``implementation_start`` is calculated above as part of validating the
    # separator.  Referencing it avoids accidentally weakening that validation
    # during future refactors where the preamble scan may be extracted.
    _ = implementation_start
    return sorted(set(issues))


__all__ = [
    "HeaderIssue",
    "HeaderRuleError",
    "HeaderScaffold",
    "HeaderWriteResult",
    "lint_header",
    "normalize_dependencies",
    "plan_new_header",
    "render_header",
    "validate_header_path",
    "write_header",
]
