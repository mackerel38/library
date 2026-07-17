from __future__ import annotations

import importlib.util
import tempfile
import unittest
from pathlib import Path


REPOSITORY_ROOT = Path(__file__).resolve().parents[2]
SPEC = importlib.util.spec_from_file_location("poe_docs_tool", REPOSITORY_ROOT / "tools/docs.py")
assert SPEC is not None and SPEC.loader is not None
DOCS_TOOL = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(DOCS_TOOL)


class LiquidProtectionTest(unittest.TestCase):
    def test_wraps_front_matter_markdown_body(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            page = root / "example.md"
            page.write_text(
                "---\ntitle: example\n---\n\n```cpp\nvector<int> a{{1, 2}};\n```\n",
                encoding="utf-8",
            )
            plain = root / "plain.md"
            plain.write_text("no Front Matter\n", encoding="utf-8")

            self.assertEqual(
                DOCS_TOOL.protect_generated_markdown_from_liquid(root), 1
            )
            text = page.read_text(encoding="utf-8")
            self.assertIn("---\n{% raw %}\n", text)
            self.assertIn("vector<int> a{{1, 2}};", text)
            self.assertTrue(text.endswith("{% endraw %}\n"))
            self.assertEqual(plain.read_text(encoding="utf-8"), "no Front Matter\n")


class TitleTest(unittest.TestCase):
    def test_removes_inline_code_markers_from_front_matter_title(self) -> None:
        self.assertEqual(
            DOCS_TOOL.title_of("# `modint`\n", "fallback"),
            "modint",
        )
        self.assertEqual(
            DOCS_TOOL.title_of(
                "# `directed_graph` / `undirected_graph`\n", "fallback"
            ),
            "directed_graph / undirected_graph",
        )

    def test_mathjax_uses_tex_delimiters_without_asciimath(self) -> None:
        mathjax = (
            REPOSITORY_ROOT
            / ".verify-helper/docs/static/_includes/mathjax.html"
        ).read_text(encoding="utf-8")
        self.assertIn("TeX-AMS_CHTML", mathjax)
        self.assertNotIn("TeX-MML-AM_CHTML", mathjax)
        self.assertIn('inlineMath: [["$", "$"], ["\\\\(", "\\\\)"]]', mathjax)
        self.assertIn('displayMath: [["$$", "$$"]', mathjax)
        self.assertIn('"pre", "code"', mathjax)

    def test_tropical_convolution_uses_jekyll_safe_tex_delimiters(self) -> None:
        document = (
            REPOSITORY_ROOT / "docs/math/tropicalconvolution.md"
        ).read_text(encoding="utf-8")
        self.assertIn("$$\nC_k = \\min_{i+j=k}(A_i+B_j)\n$$", document)
        self.assertIn("$C_k=\\min_{i+j=k}(A_i+B_j)$", document)

    def test_pages_workflow_verifies_before_generating_documents(self) -> None:
        workflow = (
            REPOSITORY_ROOT / ".github/workflows/docs.yml"
        ).read_text(encoding="utf-8")
        self.assertIn("pull_request:", workflow)
        self.assertLess(
            workflow.index("oj-verify run"),
            workflow.index("python3 tools/docs.py build"),
        )
        self.assertIn("if: github.event_name == 'push'", workflow)


class ApiReferenceTest(unittest.TestCase):
    def test_constructor_name_ignores_leading_requires_clause(self) -> None:
        declaration = (
            "template<graph_type Graph> requires (Graph::is_directed) "
            "explicit matrix_graph(const Graph& source)"
        )
        self.assertEqual(DOCS_TOOL.api_name(declaration), "matrix_graph")

    def test_collapsed_reference_is_closed_inside_generated_markers(self) -> None:
        block = DOCS_TOOL.reference_block(
            "cp/graph/matrixgraph.hpp", collapsed=True
        )
        self.assertLess(block.index(DOCS_TOOL.BEGIN), block.index("<details"))
        self.assertLess(block.index("</details>"), block.index(DOCS_TOOL.END))
        self.assertIn("<summary>すべてのAPI宣言を表示</summary>", block)

    def test_removes_negative_judge_status_but_keeps_useful_evidence(self) -> None:
        text = (
            "公式sampleを確認済み、judge ACは未確認。\n"
            "property test済み。judge未提出。\n"
        )
        cleaned = DOCS_TOOL.remove_reader_irrelevant_verification_notes(text)
        self.assertEqual(cleaned, "公式sampleを確認済み。\nproperty test済み。\n")

    def test_documentation_style_puts_summary_before_exact_definition(self) -> None:
        metadata = {
            "summary_ja": "単調述語の境界探索",
            "symbols": ["poe::first_true"],
        }
        body = "# 二分探索\n\n## どんな問題に使えるか\n\n簡単な説明。\n\n詳しい条件。\n\n## Include\n"
        styled = DOCS_TOOL.documentation_style(
            "cp/algorithm/binarysearch.hpp", metadata, body
        )
        self.assertLess(styled.index("## 概要"), styled.index("## 厳密な定義"))
        self.assertLess(styled.index("## 厳密な定義"), styled.index("## Include"))
        self.assertIn("簡単な説明。", styled)
        self.assertIn("詳しい条件。", styled)

    def test_include_section_is_inserted_after_exact_definition(self) -> None:
        body = "# Example\n\n## 概要\n\n概要。\n\n## 厳密な定義\n\n定義。\n"
        styled = DOCS_TOOL.normalize_include_section("cp/math/example.hpp", body)
        self.assertLess(styled.index("## 厳密な定義"), styled.index("## Include"))
        self.assertIn('#include "math/example.hpp"', styled)


if __name__ == "__main__":
    unittest.main()
