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


if __name__ == "__main__":
    unittest.main()
