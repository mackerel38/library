from __future__ import annotations

import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest


REPOSITORY_ROOT = Path(__file__).resolve().parents[2]
TOOLS_ROOT = REPOSITORY_ROOT / "tools"
if str(TOOLS_ROOT) not in sys.path:
    sys.path.insert(0, str(TOOLS_ROOT))

from devlib import dependency_graph  # noqa: E402
from devlib import header_rules  # noqa: E402
from devlib import stress_runner  # noqa: E402


class HeaderRulesTest(unittest.TestCase):
    def test_aggregate_header_contains_only_local_includes(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            dependency = root / "cp/core/base.hpp"
            dependency.parent.mkdir(parents=True)
            dependency.write_text(header_rules.render_header(), encoding="utf-8")
            aggregate = root / "cp/all.hpp"
            content = '#pragma once\n#include "core/base.hpp"\n'
            aggregate.write_text(content, encoding="utf-8")

            self.assertEqual(header_rules.lint_header(root, aggregate), [])
            self.assertNotIn("bits/stdc++.h", content)
            self.assertNotIn("namespace poe", content)

    def test_rendered_header_uses_poe_and_allows_assert(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            dependency = root / "cp/core/base.hpp"
            dependency.parent.mkdir(parents=True)
            dependency.write_text(header_rules.render_header(), encoding="utf-8")
            target = root / "cp/structure/example.hpp"
            target.parent.mkdir(parents=True)
            content = header_rules.render_header(["cp/core/base.hpp"]).replace(
                "namespace poe {\n}",
                "namespace poe {\ninline void check() { assert(true); }\n}",
            )
            target.write_text(content, encoding="utf-8")

            self.assertEqual(header_rules.lint_header(root, target), [])
            self.assertIn("namespace poe", content)
            self.assertIn('#include "core/base.hpp"', content)
            self.assertNotIn('#include "cp/', content)

    def test_macro_definition_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            target = root / "cp/core/bad.hpp"
            target.parent.mkdir(parents=True)
            target.write_text(
                "#pragma once\n"
                "#include <bits/stdc++.h>\n"
                "\n"
                "#define BAD_MACRO 1\n"
                "namespace poe {}\n",
                encoding="utf-8",
            )

            codes = {issue.code for issue in header_rules.lint_header(root, target)}
            self.assertIn("macro_directive", codes)

    def test_include_with_cp_prefix_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            target = root / "cp/structure/bad.hpp"
            target.parent.mkdir(parents=True)
            target.write_text(
                "#pragma once\n"
                "#include <bits/stdc++.h>\n"
                '#include "cp/core/base.hpp"\n'
                "\n"
                "namespace poe {}\n",
                encoding="utf-8",
            )

            codes = {issue.code for issue in header_rules.lint_header(root, target)}
            self.assertIn("include_root_prefix", codes)


class DependencyGraphTest(unittest.TestCase):
    def test_layers_and_affected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            base = root / "cp/core/base.hpp"
            user = root / "cp/structure/user.hpp"
            base.parent.mkdir(parents=True)
            user.parent.mkdir(parents=True)
            base.write_text(header_rules.render_header(), encoding="utf-8")
            user.write_text(
                header_rules.render_header(["cp/core/base.hpp"]),
                encoding="utf-8",
            )
            metadata = {
                "cp/core/base.hpp": {"layer": 0, "status": "stable"},
                "cp/structure/user.hpp": {"layer": 1, "status": "experimental"},
            }

            graph = dependency_graph.build_graph(root)
            self.assertEqual(dependency_graph.validate_graph(root, metadata), [])
            self.assertEqual(
                dependency_graph.affected(graph, "cp/core/base.hpp"),
                ["cp/core/base.hpp", "cp/structure/user.hpp"],
            )
            self.assertEqual(
                dependency_graph.why_path(
                    graph, "cp/structure/user.hpp", "cp/core/base.hpp"
                ),
                ["cp/structure/user.hpp", "cp/core/base.hpp"],
            )


class StressRunnerTest(unittest.TestCase):
    def test_pass_and_preserve_concrete_failure(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            generator = root / "generator.py"
            oracle = root / "oracle.py"
            correct = root / "correct.py"
            wrong = root / "wrong.py"
            generator.write_text(
                "import os\nprint(os.environ['CP_CASE_INDEX'])\n",
                encoding="utf-8",
            )
            oracle.write_text(
                "import sys\nprint(int(sys.stdin.read()) * 2)\n",
                encoding="utf-8",
            )
            correct.write_text(
                "import sys\nprint(int(sys.stdin.read()) * 2)\n",
                encoding="utf-8",
            )
            wrong.write_text(
                "import sys\nprint(int(sys.stdin.read()) * 2 + 1)\n",
                encoding="utf-8",
            )

            passed = stress_runner.run_stress(
                generator=[sys.executable, generator],
                oracle=[sys.executable, oracle],
                candidate=[sys.executable, correct],
                cases=3,
                master_seed=7,
                timeout=2,
                cwd=root,
            )
            self.assertTrue(passed.ok)
            self.assertEqual(passed.completed_cases, 3)

            failed = stress_runner.run_stress(
                generator=[sys.executable, generator],
                oracle=[sys.executable, oracle],
                candidate=[sys.executable, wrong],
                cases=3,
                master_seed=7,
                timeout=2,
                cwd=root,
            )
            self.assertFalse(failed.ok)
            self.assertEqual(failed.failure.classification, "WA")
            saved = stress_runner.save_failure(
                failed.failure,
                root=root / "regression",
                name="example",
                master_seed=failed.master_seed,
            )
            self.assertTrue((saved.saved_directory / "input.txt").is_file())
            evidence = json.loads(
                (saved.saved_directory / "failure.json").read_text(
                    encoding="utf-8"
                )
            )
            self.assertEqual(evidence["classification"], "WA")
            self.assertEqual(evidence["master_seed"], 7)


@unittest.skipUnless(
    shutil.which("g++") and shutil.which("oj-bundle"),
    "GNU++23 compiler and oj-bundle are required",
)
class CliIntegrationTest(unittest.TestCase):
    def run_dev(self, root: Path, *arguments: str) -> subprocess.CompletedProcess[str]:
        return subprocess.run(
            [
                sys.executable,
                str(TOOLS_ROOT / "dev.py"),
                "--root",
                str(root),
                *arguments,
            ],
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            timeout=120,
            check=False,
        )

    def test_new_lint_deps_compile_bundle_and_check(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            metadata = json.loads(
                (REPOSITORY_ROOT / "library.json").read_text(encoding="utf-8")
            )
            metadata["modules"] = {}
            metadata["toolchain"]["judge_flags"] = ["-std=gnu++23", "-O0"]
            (root / "library.json").write_text(
                json.dumps(metadata, indent=2) + "\n",
                encoding="utf-8",
            )

            create = self.run_dev(
                root,
                "new-header",
                "cp/core/example.hpp",
                "--layer",
                "0",
                "--with-smoke",
            )
            self.assertEqual(create.returncode, 0, create.stdout)
            header = root / "cp/core/example.hpp"
            self.assertIn("namespace poe", header.read_text(encoding="utf-8"))

            for arguments in (
                ("lint", "--all"),
                ("deps", "check"),
                ("check", "--all", "--profile", "quick"),
            ):
                result = self.run_dev(root, *arguments)
                self.assertEqual(
                    result.returncode,
                    0,
                    f"{' '.join(arguments)} failed:\n{result.stdout}",
                )


if __name__ == "__main__":
    unittest.main()
