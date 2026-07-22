#!/usr/bin/env python3
"""oj-verify向けAPI文書のFront Matterと公開宣言一覧を同期・検査する。"""

from __future__ import annotations

import argparse
import concurrent.futures
import json
import logging
import os
import re
import shutil
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
BEGIN = "<!-- BEGIN AUTO-GENERATED API REFERENCE -->"
END = "<!-- END AUTO-GENERATED API REFERENCE -->"
COLLAPSED = "<!-- API REFERENCE: COLLAPSED -->"


def modules() -> list[tuple[str, dict]]:
    data = json.loads((ROOT / "library.json").read_text(encoding="utf-8"))
    return [
        (header, metadata)
        for header, metadata in sorted(data["modules"].items())
        if not header.endswith("/all.hpp")
    ]


def title_of(text: str, fallback: str) -> str:
    body = without_front_matter(text)
    match = re.search(r"^#\s+(.+?)\s*$", body, re.MULTILINE)
    title = match.group(1) if match else fallback
    # Front MatterのtitleはlayoutがMarkdown変換せず出力するため、inline code記法を除く。
    return re.sub(r"`([^`]*)`", r"\1", title)


def without_front_matter(text: str) -> str:
    if not text.startswith("---\n"):
        return text
    end = text.find("\n---\n", 4)
    return text[end + 5 :] if end != -1 else text


def remove_reader_irrelevant_verification_notes(text: str) -> str:
    """未提出・未確認という作業状況を利用者向け本文から除く。"""
    text = re.sub(
        r"judge[^。、]*?(?:未確認|未提出|未実施|未検証)(?:で|だが)?",
        "",
        text,
        flags=re.IGNORECASE,
    )
    text = text.replace("公開問題への提出によるAC確認はまだ行っていない", "")
    text = re.sub(
        r"[^。\n]*(?:verifyコード(?:は|が)?未作成|後にverifyを追加する)[^。\n]*。",
        "",
        text,
    )
    text = re.sub(r"[^。\n]*専用の使用例は置かない[^。\n]*。", "", text)
    text = re.sub(r"(?:、|，|,)+\s*。", "。", text)
    text = re.sub(r"(?:だが|であり|への|の)\s*。", "。", text)
    text = text.replace("。、", "。")
    while "。。" in text:
        text = text.replace("。。", "。")
    text = text.replace("いずれも。", "")
    text = text.replace("確認済み・。", "確認済み。")
    text = text.replace("公式sample確認済みで。", "公式sample確認済み。")
    text = text.replace("サンプル確認のみで。", "サンプルを確認済み。")
    text = text.replace("repo内ライブラリを使用しており。", "repo内ライブラリを使用している。")
    text = text.replace("最大sampleはlocal約22秒で。", "最大sampleはlocal約22秒である。")
    text = re.sub(r"(?m)^。", "", text)
    return text


def strip_cpp_line(line: str) -> str:
    line = re.sub(r'"(?:\\.|[^"\\])*"', '""', line)
    return line.split("//", 1)[0]


def excluded_lines(lines: list[str]) -> set[int]:
    """namespace detailとprivate memberの行を保守的に除外する。"""
    excluded: set[int] = set()
    detail_depth: int | None = None
    brace_depth = 0
    private_depths: list[int] = []
    for index, raw in enumerate(lines):
        code = strip_cpp_line(raw)
        before = brace_depth
        if re.search(r"\bnamespace\s+detail\s*\{", code):
            detail_depth = before
        if detail_depth is not None:
            excluded.add(index)
        if re.match(r"^\s*private\s*:\s*$", code):
            private_depths.append(before)
        if re.match(r"^\s*(?:public|protected)\s*:\s*$", code):
            while private_depths and private_depths[-1] == before:
                private_depths.pop()
        if private_depths:
            excluded.add(index)
        brace_depth += code.count("{") - code.count("}")
        if detail_depth is not None and brace_depth <= detail_depth:
            detail_depth = None
        while private_depths and brace_depth < private_depths[-1]:
            private_depths.pop()
    return excluded


def declaration_after(lines: list[str], start: int) -> tuple[str, int]:
    parts: list[str] = []
    index = start
    while index < len(lines) and len(parts) < 12:
        stripped = lines[index].strip()
        if not stripped or stripped.startswith("//"):
            index += 1
            continue
        parts.append(stripped)
        joined = " ".join(parts)
        if "{" in stripped or ";" in stripped:
            joined = joined.split("{", 1)[0].split(";", 1)[0].strip()
            return re.sub(r"\s+", " ", joined), index
        index += 1
    return re.sub(r"\s+", " ", " ".join(parts)).strip(), index


def api_name(declaration: str) -> str:
    declaration = re.sub(r"^(?:template\s*<[^>]*>\s*)+", "", declaration)
    named = re.findall(r"\b(?:struct|class|concept|using)\s+([A-Za-z_]\w*)", declaration)
    if named:
        return named[-1]
    operator = re.search(r"\b(operator\s*(?:\[\]|\(\)|[^\s(]+))\s*\(", declaration)
    if operator:
        return operator.group(1).replace(" ", "")
    callables = re.findall(r"\b(~?[A-Za-z_]\w*)\s*\(", declaration)
    for callable_name in callables:
        if callable_name not in {"requires", "noexcept", "decltype", "sizeof"}:
            return callable_name
    before = declaration.split("(", 1)[0]
    words = re.findall(r"~?[A-Za-z_]\w*", before)
    return words[-1] if words else "宣言"


def public_api(header: Path) -> list[tuple[str, str, str]]:
    lines = header.read_text(encoding="utf-8").splitlines()
    excluded = excluded_lines(lines)
    result: list[tuple[str, str, str]] = []
    index = 0
    while index < len(lines):
        if index in excluded or not lines[index].lstrip().startswith("///"):
            index += 1
            continue
        comments: list[str] = []
        while index < len(lines) and lines[index].lstrip().startswith("///"):
            comments.append(lines[index].lstrip()[3:].strip())
            index += 1
        declaration, last = declaration_after(lines, index)
        if declaration and index not in excluded:
            description = "\n".join(comments).strip()
            result.append((api_name(declaration), declaration, description))
        index = max(index, last + 1)
    return result


def main_api_definition(header: str, metadata: dict) -> str:
    """主要symbolの補完コメントから、概要の次に置く厳密な説明を作る。"""
    expected = {
        symbol.rsplit("::", 1)[-1].split("<", 1)[0]
        for symbol in metadata.get("symbols", [])
    }
    descriptions: list[tuple[str, str]] = []
    seen: set[str] = set()
    for name, _, description in public_api(ROOT / header):
        if name not in expected or name in seen or not description:
            continue
        descriptions.append((name, description.replace("\n", " ")))
        seen.add(name)
    if descriptions:
        return "\n".join(
            f"- `{name}`: {description}" for name, description in descriptions
        )
    summary = metadata.get("summary_ja", Path(header).stem)
    return f"このheaderは、{summary}を扱う。公開APIの入出力と成立条件は以下の節で定める。"


def documentation_style(header: str, metadata: dict, body: str) -> str:
    """利用者向け本文を「概要、厳密な定義」の順へ統一する。"""
    body = re.sub(
        r"(?m)^- (?:Header|Symbol|Status):[^\n]*\n?",
        "",
        body,
    )
    summary_text = metadata.get("summary_ja", Path(header).stem).rstrip("。")
    if re.search(r"(?:る|判定)$", summary_text):
        summary = summary_text + "。"
    else:
        summary = summary_text + "を扱う。"
    old_generated_summary = summary_text + "。"
    body = body.replace(
        "## 概要\n\n" + old_generated_summary,
        "## 概要\n\n" + summary,
        1,
    )
    if "## 概要" in body and "## 厳密な定義" in body:
        return body
    definition = main_api_definition(header, metadata)
    legacy_heading = next(
        (
            heading
            for heading in ("## 概要", "## どんな問題に使えるか", "## できること")
            if heading in body
        ),
        None,
    )
    if legacy_heading is not None:
        start = body.index(legacy_heading)
        content_start = start + len(legacy_heading)
        next_heading = re.search(r"^##\s+", body[content_start:], re.MULTILINE)
        end = content_start + next_heading.start() if next_heading else len(body)
        content = body[content_start:end].strip()
        blocks = re.split(r"\n\n+", content, maxsplit=1) if content else []
        if blocks and not blocks[0].startswith(("```", "- ", "| ")):
            overview = blocks[0]
            detail = blocks[1] if len(blocks) == 2 else ""
        else:
            overview = summary
            detail = content
        strict = definition + ("\n\n" + detail if detail else "")
        replacement = (
            "## 概要\n\n"
            + overview
            + "\n\n## 厳密な定義\n\n"
            + strict
            + "\n\n"
        )
        return body[:start] + replacement + body[end:].lstrip("\n")

    title = re.search(r"^#\s+.+$", body, re.MULTILINE)
    if title is None:
        return body
    insert = title.end()
    following = body[insert:]
    marker = re.match(r"\n+" + re.escape(COLLAPSED), following)
    if marker:
        insert += marker.end()
    sections = (
        "\n\n## 概要\n\n"
        + summary
        + "\n\n## 厳密な定義\n\n"
        + definition
        + "\n"
    )
    return body[:insert] + sections + body[insert:]


def normalize_include_section(header: str, body: str) -> str:
    """Include節を厳密な定義の直後へ置く。"""
    if "## Include" not in body:
        include = re.search(
            r"^```cpp\n(?:(?!^```).)*?#include[^\n]*\n(?:(?!^```).)*?^```\n?",
            body,
            re.MULTILINE | re.DOTALL,
        )
        if include is not None:
            body = body[: include.start()] + "## Include\n\n" + body[include.start() :]
        else:
            include_path = Path(header).relative_to("cp").as_posix()
            body += (
                "\n\n## Include\n\n```cpp\n"
                f'#include "{include_path}"\n'
                "```\n"
            )
    if "## 厳密な定義" not in body:
        return body
    include_start = body.index("## Include")
    strict_start = body.index("## 厳密な定義")
    first_after_strict = re.search(r"^##\s+", body[strict_start + 1 :], re.MULTILINE)
    if first_after_strict is not None:
        first_after_strict_start = strict_start + 1 + first_after_strict.start()
        if first_after_strict_start == include_start:
            return body
    if include_start == strict_start:
        return body
    next_heading = re.search(r"^##\s+", body[include_start + 1 :], re.MULTILINE)
    include_end = (
        include_start + 1 + next_heading.start() if next_heading is not None else len(body)
    )
    section = body[include_start:include_end].strip()
    body = (body[:include_start] + body[include_end:]).strip() + "\n"
    strict_start = body.index("## 厳密な定義")
    next_heading = re.search(r"^##\s+", body[strict_start + 1 :], re.MULTILINE)
    insert = strict_start + 1 + next_heading.start() if next_heading else len(body)
    return body[:insert].rstrip() + "\n\n" + section + "\n\n" + body[insert:].lstrip()


def reference_block(header: str, *, collapsed: bool = False) -> str:
    entries = public_api(ROOT / header)
    lines = [BEGIN]
    if collapsed:
        lines += [
            '<details class="api-reference" markdown="1">',
            "<summary>すべてのAPI宣言を表示</summary>",
            "",
        ]
    lines += [
        "### 完全なAPIリファレンス" if collapsed else "## APIリファレンス",
        "",
        (
            "headerの公開補完コメントと宣言から自動生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。"
            if collapsed
            else "この節はheaderの公開補完コメントと宣言から生成している。引数の区間は、個別に断らない限り半開区間`[left, right)`である。"
        ),
        "",
    ]
    if not entries:
        lines += ["このheaderには自動抽出できる公開宣言がない。", ""]
    for name, declaration, description in entries:
        lines += [f"### `{name}`", "", "```cpp", declaration, "```", ""]
        if description:
            lines += [description, ""]
    if collapsed:
        lines += ["</details>", ""]
    lines += [END, ""]
    return "\n".join(lines)


def front_matter(header: str, title: str) -> str:
    safe_title = title.replace('"', '\\"')
    return (
        "---\n"
        f'title: "{safe_title}"\n'
        f"documentation_of: //{header}\n"
        "---\n\n"
    )


def synchronized_text(header: str, metadata: dict, docs: Path, original: str) -> str:
    title = title_of(original, Path(header).stem)
    body = without_front_matter(original).lstrip("\n")
    if BEGIN in body and END in body:
        left, rest = body.split(BEGIN, 1)
        _, right = rest.split(END, 1)
        body = left.rstrip() + "\n\n" + right.lstrip("\n")
    body = remove_reader_irrelevant_verification_notes(body)
    body = documentation_style(header, metadata, body)
    body = normalize_include_section(header, body)
    block = reference_block(header, collapsed=COLLAPSED in body)
    marker = "## 実在問題での使用例"
    if marker in body:
        before, after = body.split(marker, 1)
        body = before.rstrip() + "\n\n" + block + "\n" + marker + after
    else:
        body = body.rstrip() + "\n\n" + block
    return front_matter(header, title) + body.rstrip() + "\n"


def sync() -> int:
    changed = 0
    for header, metadata in modules():
        docs = ROOT / metadata["docs"]
        original = docs.read_text(encoding="utf-8")
        updated = synchronized_text(header, metadata, docs, original)
        if updated != original:
            docs.write_text(updated, encoding="utf-8", newline="\n")
            changed += 1
    print(f"PASS docs sync: {changed} files updated")
    return 0


def check() -> int:
    issues: list[str] = []
    for header, metadata in modules():
        docs = ROOT / metadata["docs"]
        text = docs.read_text(encoding="utf-8")
        expected = synchronized_text(header, metadata, docs, text)
        if text != expected:
            issues.append(f"{metadata['docs']}: run `python3 tools/docs.py sync`")
        for symbol in metadata.get("symbols", []):
            short = symbol.rsplit("::", 1)[-1]
            if short not in text:
                issues.append(f"{metadata['docs']}: public symbol is undocumented: {short}")
        for required in ("## 概要", "## 厳密な定義", "## Include", "#include"):
            if required not in text:
                issues.append(f"{metadata['docs']}: missing required section: {required}")
        if all(section in text for section in ("## 概要", "## 厳密な定義", "## Include")):
            if not (
                text.index("## 概要")
                < text.index("## 厳密な定義")
                < text.index("## Include")
            ):
                issues.append(
                    f"{metadata['docs']}: sections must be ordered as overview, exact definition, include"
                )
        if "## APIリファレンス" not in text and "### 完全なAPIリファレンス" not in text:
            issues.append(f"{metadata['docs']}: missing required API reference section")
    if issues:
        for issue in issues:
            print(f"FAIL docs: {issue}")
        return 1
    print(f"PASS docs: {len(modules())} header pages are synchronized")
    return 0


def protect_generated_markdown_from_liquid(directory: Path) -> int:
    """Markdown本文をraw blockで囲み、C++の`{{...}}`をLiquidから守る。"""
    protected = 0
    for path in sorted(directory.rglob("*.md")):
        text = path.read_text(encoding="utf-8")
        if not text.startswith("---\n"):
            continue
        front_matter_end = text.find("\n---\n", 4)
        if front_matter_end == -1:
            raise RuntimeError(f"generated Markdown has invalid Front Matter: {path}")
        content_start = front_matter_end + 5
        front_matter = text[:content_start]
        content = text[content_start:]
        if "{% raw %}" in content or "{% endraw %}" in content:
            raise RuntimeError(f"generated Markdown already contains Liquid raw tags: {path}")
        wrapped = (
            front_matter
            + "{% raw %}\n"
            + content.rstrip("\n")
            + "\n{% endraw %}\n"
        )
        path.write_text(wrapped, encoding="utf-8", newline="\n")
        protected += 1
    return protected


def build(jobs: int) -> int:
    """repository上の公開対象からoj-verify形式のMarkdownを生成する。"""
    if check() != 0:
        return 1

    # pipxで導入されたoj-verifyのmoduleを、その実行fileと同じvenvから読む。
    executable = shutil.which("oj-verify")
    if executable is None:
        raise RuntimeError("`oj-verify` is not installed")
    environment = Path(executable).resolve().parents[1]
    for site_packages in (environment / "lib").glob("python*/site-packages"):
        sys.path.insert(0, str(site_packages))
    import onlinejudge_verify.documentation.configure as configure
    import onlinejudge_verify.documentation.main as documentation
    import onlinejudge_verify.languages.cplusplus as cplusplus
    import onlinejudge_verify.languages.list as language_list
    import onlinejudge_verify.utils as verify_utils

    cpp_language = language_list.get(Path("library.hpp"))
    assert isinstance(cpp_language, cplusplus.CPlusPlusLanguage)
    original_bundle = cpp_language.bundle

    def bundle_from_cp_root(path, *, basedir=Path.cwd(), options):
        adjusted = dict(options)
        adjusted["include_paths"] = [basedir / "cp", basedir]
        return original_bundle(path, basedir=basedir, options=adjusted)

    cpp_language.bundle = bundle_from_cp_root

    # exclude設定はoj-verifyの依存解析後に適用されるため、公開対象をここで絞る。
    # verify以下はoj-verifyが検証fileとして扱う`*.test.cpp`だけを含める。
    def find_public_source_paths(*, basedir: Path) -> list[Path]:
        paths: list[Path] = []
        for path in (basedir / "cp").rglob("*"):
            if path.is_file() and language_list.get(path) is not None:
                paths.append(path)
        for path in (basedir / "verify").rglob("*.test.cpp"):
            if path.is_file() and language_list.get(path) is not None:
                paths.append(path)
        return paths

    def build_dependency_graph(paths: list[Path], *, basedir: Path):
        depends_on = {(basedir / path).resolve(): [] for path in paths}
        required_by = {(basedir / path).resolve(): [] for path in paths}
        verified_with = {(basedir / path).resolve(): [] for path in paths}

        def dependencies(path: Path) -> tuple[Path, list[Path]]:
            language = language_list.get(path)
            assert language is not None
            try:
                return path, language.list_dependencies(path, basedir=basedir)
            except Exception as error:
                logging.getLogger(__name__).exception(
                    "failed to list dependencies of %s: %s", path, error
                )
                return path, []

        with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as executor:
            results = executor.map(dependencies, paths)
            for source, dependencies_of_source in results:
                absolute_source = (basedir / source).resolve()
                relative_source = absolute_source.relative_to(basedir)
                for destination in dependencies_of_source:
                    absolute_destination = (basedir / destination).resolve()
                    if absolute_source == absolute_destination:
                        continue
                    if absolute_destination not in depends_on:
                        continue
                    relative_destination = absolute_destination.relative_to(basedir)
                    depends_on[absolute_source].append(relative_destination)
                    if verify_utils.is_verification_file(relative_source, basedir=basedir):
                        verified_with[absolute_destination].append(relative_source)
                    else:
                        required_by[absolute_destination].append(relative_source)
        return depends_on, required_by, verified_with

    configure._find_source_code_paths = find_public_source_paths
    configure._build_dependency_graph = build_dependency_graph

    destination = ROOT / ".verify-helper" / "markdown"
    if destination.exists():
        shutil.rmtree(destination)
    previous = Path.cwd()
    os.chdir(ROOT)
    try:
        documentation.main(jobs=jobs)
    finally:
        os.chdir(previous)

    protected = protect_generated_markdown_from_liquid(destination)

    print(
        f"PASS docs build: generated {destination} "
        f"({protected} Markdown files protected from Liquid)"
    )
    return 0


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("command", choices=("sync", "check", "build"))
    parser.add_argument("-j", "--jobs", type=int, default=2)
    args = parser.parse_args()
    if args.command == "sync":
        return sync()
    if args.command == "check":
        return check()
    return build(args.jobs)


if __name__ == "__main__":
    raise SystemExit(main())
