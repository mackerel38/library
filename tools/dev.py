#!/usr/bin/env python3
"""Local development CLI for the competitive-programming library."""

from __future__ import annotations

import argparse
import subprocess
import sys
from pathlib import Path
from typing import Any, Iterable

from devlib import build_checks
from devlib import dependency_graph
from devlib import header_rules
from devlib import metadata as metadata_io
from devlib import stress_runner


DEFAULT_ROOT = Path(__file__).resolve().parents[1]


def _relative(root: Path, path: Path | str) -> str:
    candidate = Path(path)
    if not candidate.is_absolute():
        candidate = root / candidate
    try:
        return candidate.resolve().relative_to(root.resolve()).as_posix()
    except ValueError:
        return candidate.as_posix()


def _load_metadata(root: Path) -> dict[str, Any]:
    try:
        return metadata_io.load_metadata(root)
    except metadata_io.MetadataError as exc:
        raise SystemExit(f"FAIL metadata: {exc}") from exc


def _build_config(
    root: Path,
    data: dict[str, Any],
    *,
    profile: str = "judge",
) -> build_checks.BuildConfig:
    toolchain = data.get("toolchain", {})
    if not isinstance(toolchain, dict):
        raise SystemExit("FAIL metadata.toolchain must be an object")
    compiler = toolchain.get("cxx", "g++")
    key = {
        "judge": "judge_flags",
        "sanitizer": "sanitizer_flags",
        "debug": "debug_flags",
    }[profile]
    raw_flags = toolchain.get(key, [])
    if not isinstance(compiler, str) or not isinstance(raw_flags, list) or not all(
        isinstance(flag, str) for flag in raw_flags
    ):
        raise SystemExit(f"FAIL invalid toolchain profile: {profile}")
    flags = tuple(flag for flag in raw_flags if not flag.startswith("-std="))
    return build_checks.BuildConfig(
        repository_root=root,
        compiler=compiler,
        cxx_flags=flags,
    )


def _print_check(check: build_checks.CheckResult) -> None:
    print(f"{check.status.value:4} {check.name}: {check.message}")
    for detail in check.details:
        print(f"     {detail}")
    for command in check.command_results:
        if command.succeeded:
            continue
        print(f"     command: {command.display_command}")
        if command.stderr:
            for line in command.stderr.rstrip().splitlines():
                print(f"     stderr: {line}")
        if command.stdout:
            for line in command.stdout.rstrip().splitlines():
                print(f"     stdout: {line}")


def _print_report(report: Any) -> bool:
    for check in report.checks:
        _print_check(check)
    print(f"RESULT: {report.status.value}")
    return report.status is not build_checks.Severity.FAIL


def _metadata_consistency_issues(
    root: Path, data: dict[str, Any]
) -> list[dict[str, str]]:
    headers = set(dependency_graph.discover_headers(root))
    modules = data.get("modules", {})
    issues: list[dict[str, str]] = []
    for header in sorted(headers - set(modules)):
        issues.append(
            {
                "code": "missing_metadata",
                "path": header,
                "message": f"header is not registered in library.json: {header}",
            }
        )
    for header in sorted(set(modules) - headers):
        issues.append(
            {
                "code": "stale_metadata",
                "path": header,
                "message": f"metadata refers to a missing header: {header}",
            }
        )
    for header in sorted(headers & set(modules)):
        docs = modules[header].get("docs")
        if not isinstance(docs, str) or not (root / docs).is_file():
            issues.append(
                {
                    "code": "missing_docs",
                    "path": header,
                    "message": f"metadata docs file does not exist: {docs}",
                }
            )
            continue
        if not header.endswith("/all.hpp"):
            expected_docs = (
                Path("docs") / Path(header).relative_to("cp").with_suffix(".md")
            ).as_posix()
            if docs != expected_docs:
                issues.append(
                    {
                        "code": "docs_path",
                        "path": header,
                        "message": f"docs must be one-to-one: expected {expected_docs}, got {docs}",
                    }
                )
                continue
            docs_text = (root / docs).read_text(encoding="utf-8")
            expected_front_matter = f"documentation_of: //{header}"
            if not docs_text.startswith("---\n") or expected_front_matter not in docs_text:
                issues.append(
                    {
                        "code": "docs_front_matter",
                        "path": header,
                        "message": (
                            "docs must have oj-verify Front Matter: "
                            + expected_front_matter
                        ),
                    }
                )
            if "## APIリファレンス" not in docs_text:
                issues.append(
                    {
                        "code": "docs_api_reference",
                        "path": header,
                        "message": "docs must contain the synchronized API reference",
                    }
                )
            for symbol in modules[header].get("symbols", []):
                short_symbol = symbol.rsplit("::", 1)[-1]
                if short_symbol not in docs_text:
                    issues.append(
                        {
                            "code": "docs_symbol",
                            "path": header,
                            "message": f"public symbol is undocumented: {short_symbol}",
                        }
                    )
            include = f'#include "{Path(header).relative_to("cp").as_posix()}"'
            if include not in docs_text:
                issues.append(
                    {
                        "code": "docs_include",
                        "path": header,
                        "message": f"docs must show the public include: {include}",
                    }
                )
            if (
                "## 実在問題での使用例" not in docs_text
                and "専用の使用例は置かない" not in docs_text
            ):
                issues.append(
                    {
                        "code": "docs_example",
                        "path": header,
                        "message": "docs must contain a real-problem example or an explicit omission",
                    }
                )
    return issues


def _print_issue(issue: dict[str, Any]) -> None:
    path = issue.get("path", "-")
    code = issue.get("code", "issue")
    message = issue.get("message", "")
    print(f"FAIL {path}: {code}: {message}")


def _git_changed_headers(root: Path) -> list[str]:
    result = subprocess.run(
        ["git", "-C", str(root), "status", "--porcelain", "--untracked-files=all"],
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        check=False,
    )
    if result.returncode != 0:
        raise SystemExit(
            "FAIL --changed requires a Git repository: "
            + result.stderr.strip()
        )
    changed: set[str] = set()
    for line in result.stdout.splitlines():
        if len(line) < 4:
            continue
        path_text = line[3:]
        if " -> " in path_text:
            path_text = path_text.split(" -> ", 1)[1]
        path_text = path_text.strip('"')
        if path_text.startswith("cp/") and path_text.endswith(".hpp"):
            changed.add(path_text)
    graph = dependency_graph.build_graph(root)
    selected: set[str] = set()
    for header in changed:
        impacted = dependency_graph.affected(graph, header)
        selected.update(impacted or [header])
    return sorted(header for header in selected if (root / header).is_file())


def _selected_headers(
    root: Path,
    path: str | None,
    all_headers: bool,
    *,
    changed: bool = False,
) -> list[str]:
    selected_modes = sum((path is not None, all_headers, changed))
    if selected_modes > 1:
        raise SystemExit("FAIL choose only one of a header path, --all or --changed")
    if changed:
        return _git_changed_headers(root)
    if all_headers:
        return dependency_graph.discover_headers(root)
    if path is None:
        raise SystemExit("FAIL specify a header path or --all")
    relative = _relative(root, path)
    return [relative]


def command_doctor(args: argparse.Namespace) -> int:
    root = args.root.resolve()
    try:
        data = metadata_io.load_metadata(root)
    except metadata_io.MetadataError as exc:
        print(f"FAIL metadata: {exc}")
        data = {
            "toolchain": {
                "cxx": "g++",
                "judge_flags": ["-std=gnu++23", "-O2", "-pipe"],
            }
        }
        metadata_ok = False
    else:
        print(f"PASS metadata: {root / metadata_io.METADATA_FILENAME}")
        metadata_ok = True
    report = build_checks.doctor(_build_config(root, data))
    return 0 if _print_report(report) and metadata_ok else 1


def _module_paths(header: str) -> tuple[str, str]:
    relative = Path(header)
    module_tail = relative.relative_to("cp").with_suffix("")
    docs = (Path("docs") / module_tail).with_suffix(".md").as_posix()
    usage = (Path("tests/api") / module_tail).with_suffix(".cpp").as_posix()
    return docs, usage


def _render_docs(header: str) -> str:
    title = Path(header).stem.replace("_", " ").title()
    include = Path(header).relative_to("cp").as_posix()
    return (
        "---\n"
        f'title: "{title}"\n'
        f"documentation_of: //{header}\n"
        "---\n\n"
        f"# {title}\n\n"
        f"- Status: experimental\n"
        f"- Header: {header}\n\n"
        "## Include\n\n"
        "```cpp\n"
        f'#include "{include}"\n'
        "```\n\n"
        "## できること\n\n"
        "TODO\n\n"
        "## APIリファレンス\n\n"
        "公開するコンストラクタ・関数・メソッドごとに、宣言、動作、引数、返り値を書く。\n\n"
        "### `name`\n\n"
        "```cpp\n"
        "return_type name(arguments)\n"
        "```\n\n"
        "TODO: 動作、引数、返り値。\n\n"
        "制約\n\n"
        "- TODO\n\n"
        "計算量\n\n"
        "- TODO\n\n"
        "## 前提・頻出事故\n\n"
        "TODO\n\n"
        "## 実在問題での使用例\n\n"
        "TODO: 公開問題のURL、使用例、judge AC確認状態を書く。\n\n"
        "## 検証\n\n"
        "TODO\n"
    )


def _render_usage(header: str) -> str:
    include = Path(header).relative_to("cp").as_posix()
    return (
        f'#include "{include}"\n\n'
        "int main() {\n"
        "    // TODO: instantiate the public API before promotion.\n"
        "    return 0;\n"
        "}\n"
    )


def command_new_header(args: argparse.Namespace) -> int:
    root = args.root.resolve()
    data = _load_metadata(root)
    try:
        scaffold = header_rules.plan_new_header(
            root,
            Path(args.header),
            args.dep,
            require_dependencies=True,
        )
    except (header_rules.HeaderRuleError, OSError) as exc:
        print(f"FAIL new-header: {exc}")
        return 1

    header = scaffold.relative_path.as_posix()
    docs, usage = _module_paths(header)
    modules = data["modules"]
    if header in modules:
        print(f"FAIL new-header: metadata already exists for {header}")
        return 1

    planned_paths = [scaffold.path, root / docs]
    if args.with_smoke:
        planned_paths.append(root / usage)
    collisions = [path for path in planned_paths if path.exists()]
    if collisions:
        for path in collisions:
            print(f"FAIL refusing to overwrite: {_relative(root, path)}")
        return 1

    for dependency in scaffold.dependencies:
        entry = modules.get(dependency)
        if not isinstance(entry, dict):
            print(f"FAIL dependency has no metadata: {dependency}")
            return 1
        dependency_layer = entry.get("layer")
        if not isinstance(dependency_layer, int) or dependency_layer >= args.layer:
            print(
                "FAIL dependency layer: "
                f"{dependency} is layer {dependency_layer!r}, "
                f"new header is layer {args.layer}"
            )
            return 1

    if args.dry_run:
        print(f"DRY-RUN header: {header}")
        print(scaffold.content, end="")
        print(f"DRY-RUN docs: {docs}")
        if args.with_smoke:
            print(f"DRY-RUN usage: {usage}")
        print("DRY-RUN metadata: experimental")
        return 0

    created: list[Path] = []
    try:
        result = header_rules.write_header(scaffold)
        if result.written:
            created.append(scaffold.path)
        docs_path = root / docs
        docs_path.parent.mkdir(parents=True, exist_ok=True)
        docs_path.write_text(_render_docs(header), encoding="utf-8", newline="\n")
        created.append(docs_path)
        usage_value: str | None = None
        if args.with_smoke:
            usage_path = root / usage
            usage_path.parent.mkdir(parents=True, exist_ok=True)
            usage_path.write_text(
                _render_usage(header), encoding="utf-8", newline="\n"
            )
            created.append(usage_path)
            usage_value = usage
        modules[header] = metadata_io.module_entry(
            layer=args.layer,
            docs=docs,
            usage_test=usage_value,
        )
        metadata_io.save_metadata(root, data)
    except OSError as exc:
        for path in reversed(created):
            try:
                path.unlink()
            except OSError:
                pass
        print(f"FAIL new-header: {exc}")
        return 1

    for path in created:
        print(f"PASS created: {_relative(root, path)}")
    print(f"PASS registered: {header} (experimental, layer {args.layer})")
    return 0


def command_lint(args: argparse.Namespace) -> int:
    root = args.root.resolve()
    headers = _selected_headers(root, args.header, args.all)
    failures = 0
    if not headers:
        print("PASS lint: no headers")
        return 0
    for header in headers:
        issues = header_rules.lint_header(root, Path(header))
        if issues:
            failures += len(issues)
            for issue in issues:
                print(f"FAIL {issue}")
        else:
            print(f"PASS lint: {header}")
    return 1 if failures else 0


def command_deps(args: argparse.Namespace) -> int:
    root = args.root.resolve()
    data = _load_metadata(root)
    graph = dependency_graph.build_graph(root)
    if args.deps_command == "check":
        issues = _metadata_consistency_issues(root, data)
        issues.extend(dependency_graph.validate_graph(root, data["modules"]))
        if not issues:
            print("PASS deps: dependency graph is valid")
            return 0
        for issue in issues:
            _print_issue(issue)
        return 1
    if args.deps_command == "affected":
        values = dependency_graph.affected(graph, args.target)
        if not values:
            print(f"FAIL unknown dependency node: {args.target}")
            return 1
        for value in values:
            print(value)
        return 0
    if args.deps_command == "why":
        path = dependency_graph.why_path(graph, args.source, args.dependency)
        if path is None:
            print(f"FAIL no dependency path: {args.source} -> {args.dependency}")
            return 1
        print(" -> ".join(path))
        return 0
    raise AssertionError(args.deps_command)


def _usage_for(data: dict[str, Any], header: str) -> str | None:
    entry = data.get("modules", {}).get(header, {})
    tests = entry.get("tests", []) if isinstance(entry, dict) else []
    if not isinstance(tests, list):
        return None
    return next((test for test in tests if isinstance(test, str)), None)


def command_compile_header(args: argparse.Namespace) -> int:
    root = args.root.resolve()
    data = _load_metadata(root)
    headers = _selected_headers(root, args.header, args.all)
    config = _build_config(root, data, profile=args.profile)
    ok = True
    if not headers:
        print("PASS compile-header: no headers")
        return 0
    for header in headers:
        usage = args.usage or _usage_for(data, header)
        print(f"== {header} ({args.profile}) ==")
        report = build_checks.compile_header(
            header,
            config,
            usage_smoke=usage,
        )
        ok = _print_report(report) and ok
        if usage is None:
            print("WARN usage-smoke: no usage test is registered")
    return 0 if ok else 1


def command_bundle(args: argparse.Namespace) -> int:
    root = args.root.resolve()
    data = _load_metadata(root)
    config = _build_config(root, data)
    output = None if args.output is None else Path(args.output)
    if output is not None:
        resolved = output if output.is_absolute() else root / output
        resolved.parent.mkdir(parents=True, exist_ok=True)
    if args.header is not None:
        report = build_checks.bundle_header(
            args.header,
            config,
            output_path=output,
        )
    else:
        report = build_checks.bundle_source(
            args.source,
            config,
            output_path=output,
        )
    return 0 if _print_report(report) else 1


def _run_quick_check(
    root: Path,
    data: dict[str, Any],
    headers: list[str],
    *,
    full: bool,
) -> int:
    failed = False
    for header in headers:
        issues = header_rules.lint_header(root, Path(header))
        if issues:
            failed = True
            for issue in issues:
                print(f"FAIL {issue}")
        else:
            print(f"PASS lint: {header}")

    dependency_issues = _metadata_consistency_issues(root, data)
    dependency_issues.extend(
        dependency_graph.validate_graph(root, data.get("modules", {}))
    )
    for issue in dependency_issues:
        failed = True
        _print_issue(issue)
    if not dependency_issues:
        print("PASS deps: dependency graph is valid")

    profiles = ["judge", "sanitizer", "debug"] if full else ["judge"]
    for header in headers:
        usage = _usage_for(data, header)
        if usage is None:
            print(f"WARN {header}: no usage smoke test registered")
        for profile in profiles:
            print(f"== compile {header} ({profile}) ==")
            report = build_checks.compile_header(
                header,
                _build_config(root, data, profile=profile),
                usage_smoke=usage,
            )
            if not _print_report(report):
                failed = True
        print(f"== bundle {header} ==")
        bundle = build_checks.bundle_header(
            header,
            _build_config(root, data, profile="judge"),
        )
        if not _print_report(bundle):
            failed = True
    return 1 if failed else 0


def command_check(args: argparse.Namespace) -> int:
    root = args.root.resolve()
    data = _load_metadata(root)
    headers = _selected_headers(
        root,
        args.header,
        args.all,
        changed=args.changed,
    )
    if not headers:
        print("PASS check: no headers")
        return 0
    result = _run_quick_check(
        root,
        data,
        headers,
        full=args.profile == "full",
    )
    if args.profile == "full":
        print(
            "WARN full profile currently covers the compiler matrix only; "
            "stress, oj-verify and benchmark orchestration are not integrated yet"
        )
    return result


def command_stress(args: argparse.Namespace) -> int:
    root = args.root.resolve()
    try:
        report = stress_runner.run_stress(
            generator=args.generator,
            oracle=args.oracle,
            candidate=args.candidate,
            cases=args.cases,
            master_seed=args.seed,
            timeout=args.timeout,
            comparator=args.comparator,
            cwd=root,
            detect_nondeterminism=args.detect_nondeterminism,
        )
    except ValueError as exc:
        print(f"FAIL stress: {exc}")
        return 1
    if report.ok:
        print(
            f"PASS stress: {report.completed_cases}/{report.requested_cases} cases "
            f"(master seed {report.master_seed})"
        )
        return 0

    assert report.failure is not None
    failure = report.failure
    if not args.no_save:
        failure = stress_runner.save_failure(
            failure,
            root=root / args.regression_root,
            name=args.name,
            master_seed=report.master_seed,
        )
    print(
        f"FAIL stress: {failure.classification} at case {failure.case_index}, "
        f"case seed {failure.case_seed}"
    )
    print(f"     {failure.message}")
    if failure.saved_directory is not None:
        try:
            saved_display = failure.saved_directory.relative_to(root).as_posix()
        except ValueError:
            saved_display = failure.saved_directory.as_posix()
        print(
            "     saved: "
            f"{saved_display}"
        )
    else:
        print("----- failing input -----")
        print(failure.input_text, end="" if failure.input_text.endswith("\n") else "\n")
    return 1


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="Develop and validate the local competitive-programming library."
    )
    parser.add_argument(
        "--root",
        type=Path,
        default=DEFAULT_ROOT,
        help="repository root (defaults to the parent of tools/)",
    )
    commands = parser.add_subparsers(dest="command", required=True)

    doctor_parser = commands.add_parser("doctor", help="check local development tools")
    doctor_parser.set_defaults(handler=command_doctor)

    new_parser = commands.add_parser("new-header", help="create a module scaffold")
    new_parser.add_argument("header")
    new_parser.add_argument("--layer", type=int, required=True)
    new_parser.add_argument("--dep", action="append", default=[])
    new_parser.add_argument("--with-smoke", action="store_true")
    new_parser.add_argument("--dry-run", action="store_true")
    new_parser.set_defaults(handler=command_new_header)

    lint_parser = commands.add_parser("lint", help="lint fixed header format")
    lint_parser.add_argument("header", nargs="?")
    lint_parser.add_argument("--all", action="store_true")
    lint_parser.set_defaults(handler=command_lint)

    deps_parser = commands.add_parser("deps", help="inspect the include DAG")
    deps_commands = deps_parser.add_subparsers(dest="deps_command", required=True)
    deps_check = deps_commands.add_parser("check")
    deps_check.set_defaults(handler=command_deps)
    deps_affected = deps_commands.add_parser("affected")
    deps_affected.add_argument("target")
    deps_affected.set_defaults(handler=command_deps)
    deps_why = deps_commands.add_parser("why")
    deps_why.add_argument("source")
    deps_why.add_argument("dependency")
    deps_why.set_defaults(handler=command_deps)

    compile_parser = commands.add_parser(
        "compile-header", help="compile headers standalone and through usage tests"
    )
    compile_parser.add_argument("header", nargs="?")
    compile_parser.add_argument("--all", action="store_true")
    compile_parser.add_argument("--usage")
    compile_parser.add_argument(
        "--profile",
        choices=("judge", "sanitizer", "debug"),
        default="judge",
    )
    compile_parser.set_defaults(handler=command_compile_header)

    bundle_parser = commands.add_parser("bundle", help="bundle and compile a source")
    bundle_target = bundle_parser.add_mutually_exclusive_group(required=True)
    bundle_target.add_argument("--header")
    bundle_target.add_argument("--source")
    bundle_parser.add_argument("--output")
    bundle_parser.set_defaults(handler=command_bundle)

    check_parser = commands.add_parser("check", help="run the P0 validation gate")
    check_parser.add_argument("header", nargs="?")
    check_parser.add_argument("--all", action="store_true")
    check_parser.add_argument("--changed", action="store_true")
    check_parser.add_argument(
        "--profile", choices=("quick", "full"), default="quick"
    )
    check_parser.set_defaults(handler=command_check)

    stress_parser = commands.add_parser(
        "stress", help="run deterministic differential tests"
    )
    stress_parser.add_argument("--name", required=True)
    stress_parser.add_argument("--generator", required=True)
    stress_parser.add_argument("--oracle", required=True)
    stress_parser.add_argument("--candidate", required=True)
    stress_parser.add_argument("--cases", type=int, default=1000)
    stress_parser.add_argument("--seed", type=int, default=1)
    stress_parser.add_argument("--timeout", type=float, default=2.0)
    stress_parser.add_argument(
        "--comparator", choices=("tokens", "bytes"), default="tokens"
    )
    stress_parser.add_argument("--detect-nondeterminism", action="store_true")
    stress_parser.add_argument(
        "--regression-root", default="tests/regression"
    )
    stress_parser.add_argument("--no-save", action="store_true")
    stress_parser.set_defaults(handler=command_stress)
    return parser


def main(argv: Iterable[str] | None = None) -> int:
    parser = build_parser()
    args = parser.parse_args(list(argv) if argv is not None else None)
    if getattr(args, "layer", 0) < 0:
        parser.error("--layer must be non-negative")
    return int(args.handler(args))


if __name__ == "__main__":
    raise SystemExit(main())
