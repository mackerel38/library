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
        self.assertIn('inlineMath: [["\\\\(", "\\\\)"]]', mathjax)
        self.assertIn('"pre", "code"', mathjax)


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


if __name__ == "__main__":
    unittest.main()
