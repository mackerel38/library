"""Compiler and ``oj-bundle`` checks for the library development CLI.

This module deliberately has no knowledge of the CLI's presentation layer.  Every
operation returns immutable, structured results, including the exact subprocess
arguments and captured output.  Callers can therefore render the same result as
human-readable text or JSON without re-running a command.
"""

from __future__ import annotations

from dataclasses import asdict, dataclass, field
from enum import Enum
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile
import time
from typing import Any, Iterable, Sequence


class Severity(str, Enum):
    """Outcome of a check, ordered by how much attention it requires."""

    PASS = "PASS"
    WARN = "WARN"
    FAIL = "FAIL"


def _aggregate_status(checks: Iterable["CheckResult"]) -> Severity:
    statuses = {check.status for check in checks}
    if Severity.FAIL in statuses:
        return Severity.FAIL
    if Severity.WARN in statuses:
        return Severity.WARN
    return Severity.PASS


@dataclass(frozen=True, slots=True)
class CommandResult:
    """Captured result of one subprocess invocation."""

    command: tuple[str, ...]
    returncode: int | None
    stdout: str
    stderr: str
    elapsed_seconds: float
    timed_out: bool = False
    cwd: str | None = None

    @property
    def succeeded(self) -> bool:
        return not self.timed_out and self.returncode == 0

    @property
    def display_command(self) -> str:
        """A copyable representation for display (never passed to a shell)."""

        import shlex

        return shlex.join(self.command)

    def to_dict(self) -> dict[str, Any]:
        return asdict(self)


@dataclass(frozen=True, slots=True)
class CheckResult:
    """One logical check, possibly backed by more than one command."""

    name: str
    status: Severity
    message: str
    command_results: tuple[CommandResult, ...] = ()
    details: tuple[str, ...] = ()

    @property
    def commands(self) -> tuple[tuple[str, ...], ...]:
        return tuple(result.command for result in self.command_results)

    def to_dict(self) -> dict[str, Any]:
        return asdict(self)


@dataclass(frozen=True, slots=True)
class DoctorReport:
    """Result of :func:`doctor`."""

    repository_root: Path
    checks: tuple[CheckResult, ...]

    @property
    def status(self) -> Severity:
        return _aggregate_status(self.checks)

    @property
    def ok(self) -> bool:
        return self.status is not Severity.FAIL

    def to_dict(self) -> dict[str, Any]:
        return {
            "repository_root": str(self.repository_root),
            "status": self.status.value,
            "checks": [check.to_dict() for check in self.checks],
        }


@dataclass(frozen=True, slots=True)
class CompileHeaderReport:
    """Standalone, repeated-include, ODR and optional usage checks."""

    header: Path
    checks: tuple[CheckResult, ...]

    @property
    def status(self) -> Severity:
        return _aggregate_status(self.checks)

    @property
    def ok(self) -> bool:
        return self.status is not Severity.FAIL

    def to_dict(self) -> dict[str, Any]:
        return {
            "header": str(self.header),
            "status": self.status.value,
            "checks": [check.to_dict() for check in self.checks],
        }


@dataclass(frozen=True, slots=True)
class BundleReport:
    """Result of bundling and compiling one source (or a synthetic source)."""

    source: Path
    checks: tuple[CheckResult, ...]
    bundled_source: str | None = None
    output_path: Path | None = None

    @property
    def status(self) -> Severity:
        return _aggregate_status(self.checks)

    @property
    def ok(self) -> bool:
        return self.status is not Severity.FAIL

    def to_dict(self) -> dict[str, Any]:
        return {
            "source": str(self.source),
            "status": self.status.value,
            "bundled_source": self.bundled_source,
            "output_path": None if self.output_path is None else str(self.output_path),
            "checks": [check.to_dict() for check in self.checks],
        }


@dataclass(frozen=True, slots=True)
class BuildConfig:
    """Configuration shared by compiler and bundler checks.

    ``cxx_flags`` intentionally does not contain the language standard.  The
    module always adds ``-std=gnu++23`` so a custom flag set cannot accidentally
    turn a required GNU++23 check into a check of another dialect.
    """

    repository_root: Path = field(default_factory=Path.cwd)
    compiler: str = field(default_factory=lambda: os.environ.get("CXX", "g++"))
    cxx_flags: tuple[str, ...] = ("-O2", "-pipe")
    linker_flags: tuple[str, ...] = ()
    oj_bundle: str = "oj-bundle"
    compile_timeout: float = 120.0
    run_timeout: float = 5.0
    bundle_timeout: float = 30.0

    def __post_init__(self) -> None:
        object.__setattr__(self, "repository_root", Path(self.repository_root).resolve())
        object.__setattr__(self, "cxx_flags", tuple(self.cxx_flags))
        object.__setattr__(self, "linker_flags", tuple(self.linker_flags))
        for name in ("compile_timeout", "run_timeout", "bundle_timeout"):
            if getattr(self, name) <= 0:
                raise ValueError(f"{name} must be positive")


DEFAULT_REQUIRED_DIRECTORIES = (
    "cp",
    "docs",
    "tests",
    "verify",
    "tools",
)

_LOCAL_LIBRARY_INCLUDE_RE = re.compile(
    r'^\s*#\s*include\s*"(?:[a-z0-9_]+/)*[a-z][a-z0-9_]*\.hpp"', re.MULTILINE
)


def _run_command(
    command: Sequence[str | os.PathLike[str]],
    *,
    timeout: float,
    cwd: Path | None = None,
    stdin: str | None = None,
) -> CommandResult:
    argv = tuple(os.fspath(part) for part in command)
    started = time.perf_counter()
    try:
        completed = subprocess.run(
            argv,
            cwd=None if cwd is None else os.fspath(cwd),
            input=stdin,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            encoding="utf-8",
            errors="replace",
            timeout=timeout,
            check=False,
            shell=False,
        )
        return CommandResult(
            command=argv,
            returncode=completed.returncode,
            stdout=completed.stdout,
            stderr=completed.stderr,
            elapsed_seconds=time.perf_counter() - started,
            cwd=None if cwd is None else os.fspath(cwd),
        )
    except subprocess.TimeoutExpired as error:
        stdout = error.stdout or ""
        stderr = error.stderr or ""
        if isinstance(stdout, bytes):
            stdout = stdout.decode("utf-8", errors="replace")
        if isinstance(stderr, bytes):
            stderr = stderr.decode("utf-8", errors="replace")
        return CommandResult(
            command=argv,
            returncode=None,
            stdout=stdout,
            stderr=stderr,
            elapsed_seconds=time.perf_counter() - started,
            timed_out=True,
            cwd=None if cwd is None else os.fspath(cwd),
        )
    except OSError as error:
        return CommandResult(
            command=argv,
            returncode=127,
            stdout="",
            stderr=f"{type(error).__name__}: {error}",
            elapsed_seconds=time.perf_counter() - started,
            cwd=None if cwd is None else os.fspath(cwd),
        )


def _command_check(name: str, result: CommandResult, success: str) -> CheckResult:
    if result.succeeded:
        return CheckResult(name, Severity.PASS, success, (result,))
    if result.timed_out:
        message = f"timed out after {result.elapsed_seconds:.3f}s"
    else:
        message = f"command exited with status {result.returncode}"
    return CheckResult(name, Severity.FAIL, message, (result,))


def _compiler_command(
    config: BuildConfig,
    sources: Sequence[Path],
    output: Path,
    *,
    compile_only: bool = False,
) -> tuple[str, ...]:
    command: list[str] = [
        config.compiler,
        "-std=gnu++23",
        *config.cxx_flags,
        "-I",
        os.fspath(config.repository_root / "cp"),
    ]
    if compile_only:
        command.append("-c")
    command.extend(os.fspath(source) for source in sources)
    command.extend(("-o", os.fspath(output)))
    if not compile_only:
        command.extend(config.linker_flags)
    return tuple(command)


def _compile(
    config: BuildConfig,
    sources: Sequence[Path],
    output: Path,
    *,
    compile_only: bool = False,
) -> CommandResult:
    return _run_command(
        _compiler_command(config, sources, output, compile_only=compile_only),
        timeout=config.compile_timeout,
        cwd=config.repository_root if config.repository_root.is_dir() else None,
    )


def _resolve_existing_file(
    path: str | os.PathLike[str], config: BuildConfig
) -> tuple[Path, str | None]:
    candidate = Path(path)
    if not candidate.is_absolute():
        candidate = config.repository_root / candidate
    candidate = candidate.resolve()
    if not candidate.exists():
        return candidate, "file does not exist"
    if not candidate.is_file():
        return candidate, "path is not a regular file"
    return candidate, None


def _repository_include(header: Path, root: Path) -> str | None:
    try:
        return header.relative_to(root / "cp").as_posix()
    except ValueError:
        return None


def doctor(
    config: BuildConfig | None = None,
    *,
    required_directories: Sequence[str] = DEFAULT_REQUIRED_DIRECTORIES,
) -> DoctorReport:
    """Check the local GNU++23 and ``oj-bundle`` development environment.

    Missing project directories are warnings so this remains useful while a new
    repository is being bootstrapped.  Compiler and bundler probes still run and
    are failures if the required tools cannot perform a real compile/bundle.
    """

    config = config or BuildConfig()
    root = config.repository_root
    checks: list[CheckResult] = []

    if root.is_dir():
        checks.append(
            CheckResult("repository-root", Severity.PASS, f"repository root: {root}")
        )
    else:
        checks.append(
            CheckResult(
                "repository-root", Severity.FAIL, f"not an existing directory: {root}"
            )
        )

    missing = tuple(name for name in required_directories if not (root / name).is_dir())
    if missing:
        checks.append(
            CheckResult(
                "repository-directories",
                Severity.WARN,
                "project is not fully scaffolded yet",
                details=tuple(f"missing directory: {name}/" for name in missing),
            )
        )
    else:
        checks.append(
            CheckResult(
                "repository-directories",
                Severity.PASS,
                "all expected project directories exist",
            )
        )

    python_version = sys.version_info
    python_message = (
        f"{sys.executable}: "
        f"{python_version.major}.{python_version.minor}.{python_version.micro}"
    )
    checks.append(
        CheckResult(
            "python-version",
            Severity.PASS if python_version >= (3, 12) else Severity.FAIL,
            python_message
            if python_version >= (3, 12)
            else f"Python 3.12 or newer is required; found {python_message}",
        )
    )

    compiler_path = shutil.which(config.compiler)
    if compiler_path is None:
        checks.append(
            CheckResult(
                "compiler-found",
                Severity.FAIL,
                f"compiler is not executable or not on PATH: {config.compiler}",
            )
        )
    else:
        checks.append(
            CheckResult(
                "compiler-found", Severity.PASS, f"compiler: {compiler_path}"
            )
        )
        version = _run_command(
            (config.compiler, "--version"), timeout=config.compile_timeout
        )
        checks.append(_command_check("compiler-version", version, "compiler started"))

    oj_bundle_path = shutil.which(config.oj_bundle)
    if oj_bundle_path is None:
        checks.append(
            CheckResult(
                "oj-bundle-found",
                Severity.FAIL,
                f"oj-bundle is not executable or not on PATH: {config.oj_bundle}",
            )
        )
    else:
        checks.append(
            CheckResult(
                "oj-bundle-found", Severity.PASS, f"oj-bundle: {oj_bundle_path}"
            )
        )

    with tempfile.TemporaryDirectory(prefix="cp-dev-doctor-") as temporary:
        temp_root = Path(temporary)
        compile_source = temp_root / "gnu23_probe.cpp"
        compile_source.write_text(
            "constexpr int f(int x) {\n"
            "    if consteval { return x + 1; }\n"
            "    else { return x; }\n"
            "}\n"
            "static_assert(f(22) == 23);\n"
            "int main() { return 0; }\n",
            encoding="utf-8",
        )
        compile_output = temp_root / "gnu23_probe"
        compile_result = _compile(config, (compile_source,), compile_output)
        checks.append(
            _command_check(
                "gnu++23-compile",
                compile_result,
                "GNU++23 language probe compiled",
            )
        )

        probe_include_root = temp_root / "bundle_root"
        probe_header = probe_include_root / "probe.hpp"
        probe_header.parent.mkdir(parents=True)
        probe_header.write_text(
            "#pragma once\n"
            "#include <bits/stdc++.h>\n"
            "\n"
            "namespace poe { inline int bundle_probe() { return 0; } }\n",
            encoding="utf-8",
        )
        probe_source = temp_root / "bundle_probe.cpp"
        probe_source.write_text(
            '#include "probe.hpp"\n'
            "int main() { return poe::bundle_probe(); }\n",
            encoding="utf-8",
        )
        bundle_result = _run_command(
            (
                config.oj_bundle,
                "-I",
                os.fspath(probe_include_root),
                os.fspath(probe_source),
            ),
            timeout=config.bundle_timeout,
            cwd=temp_root,
        )
        checks.append(
            _command_check(
                "oj-bundle-probe", bundle_result, "oj-bundle expanded a local header"
            )
        )
        if bundle_result.succeeded:
            unresolved = _LOCAL_LIBRARY_INCLUDE_RE.findall(bundle_result.stdout)
            if unresolved:
                checks.append(
                    CheckResult(
                        "oj-bundle-local-includes",
                        Severity.FAIL,
                        "bundled probe retains unresolved library includes",
                        (bundle_result,),
                        tuple(unresolved),
                    )
                )
            else:
                checks.append(
                    CheckResult(
                        "oj-bundle-local-includes",
                        Severity.PASS,
                        "bundled probe has no unresolved library includes",
                    )
                )
            bundled_probe = temp_root / "bundle_probe.bundled.cpp"
            bundled_probe.write_text(bundle_result.stdout, encoding="utf-8")
            bundled_output = temp_root / "bundle_probe.bundled"
            bundled_compile = _compile(
                config, (bundled_probe,), bundled_output
            )
            checks.append(
                _command_check(
                    "oj-bundle-compile",
                    bundled_compile,
                    "bundled probe compiled as GNU++23",
                )
            )

    return DoctorReport(root, tuple(checks))


def compile_header(
    header: str | os.PathLike[str],
    config: BuildConfig | None = None,
    *,
    usage_smoke: str | os.PathLike[str] | None = None,
    usage_input: str = "",
    expected_output: str | None = None,
) -> CompileHeaderReport:
    """Compile a header standalone, twice in one TU and from two linked TUs.

    When ``usage_smoke`` is supplied it is also compiled and run.  The smoke
    program is expected to return zero; ``expected_output`` optionally requires
    an exact stdout match as well.
    """

    config = config or BuildConfig()
    header_path, error = _resolve_existing_file(header, config)
    if error is not None:
        return CompileHeaderReport(
            header_path,
            (CheckResult("header-path", Severity.FAIL, f"{header_path}: {error}"),),
        )
    include_path = _repository_include(header_path, config.repository_root)
    if include_path is None:
        return CompileHeaderReport(
            header_path,
            (
                CheckResult(
                    "header-path",
                    Severity.FAIL,
                    f"header is outside repository root: {header_path}",
                ),
            ),
        )

    checks: list[CheckResult] = []
    with tempfile.TemporaryDirectory(prefix="cp-dev-header-") as temporary:
        temp_root = Path(temporary)

        standalone = temp_root / "standalone.cpp"
        standalone.write_text(
            f'#include "{include_path}"\nint main() {{ return 0; }}\n',
            encoding="utf-8",
        )
        standalone_result = _compile(
            config, (standalone,), temp_root / "standalone"
        )
        checks.append(
            _command_check(
                "standalone-include",
                standalone_result,
                "header compiled when included by itself",
            )
        )

        repeated = temp_root / "repeated.cpp"
        repeated.write_text(
            f'#include "{include_path}"\n'
            f'#include "{include_path}"\n'
            "int main() { return 0; }\n",
            encoding="utf-8",
        )
        repeated_result = _compile(config, (repeated,), temp_root / "repeated")
        checks.append(
            _command_check(
                "double-include",
                repeated_result,
                "header compiled when included twice in one translation unit",
            )
        )

        odr_a = temp_root / "odr_a.cpp"
        odr_b = temp_root / "odr_b.cpp"
        odr_a.write_text(
            f'#include "{include_path}"\n'
            "int cp_dev_odr_anchor() { return 0; }\n",
            encoding="utf-8",
        )
        odr_b.write_text(
            f'#include "{include_path}"\n'
            "int cp_dev_odr_anchor();\n"
            "int main() { return cp_dev_odr_anchor(); }\n",
            encoding="utf-8",
        )
        odr_result = _compile(config, (odr_a, odr_b), temp_root / "odr")
        checks.append(
            _command_check(
                "two-tu-odr-link",
                odr_result,
                "two translation units including the header linked successfully",
            )
        )

        if usage_smoke is not None:
            usage_path, usage_error = _resolve_existing_file(usage_smoke, config)
            if usage_error is not None:
                checks.append(
                    CheckResult(
                        "usage-smoke-path",
                        Severity.FAIL,
                        f"{usage_path}: {usage_error}",
                    )
                )
            else:
                usage_executable = temp_root / "usage_smoke"
                usage_compile = _compile(config, (usage_path,), usage_executable)
                checks.append(
                    _command_check(
                        "usage-smoke-compile",
                        usage_compile,
                        "usage smoke program compiled",
                    )
                )
                if usage_compile.succeeded:
                    usage_run = _run_command(
                        (os.fspath(usage_executable),),
                        timeout=config.run_timeout,
                        cwd=config.repository_root,
                        stdin=usage_input,
                    )
                    if not usage_run.succeeded:
                        checks.append(
                            _command_check(
                                "usage-smoke-run", usage_run, "usage smoke program passed"
                            )
                        )
                    elif expected_output is not None and usage_run.stdout != expected_output:
                        checks.append(
                            CheckResult(
                                "usage-smoke-run",
                                Severity.FAIL,
                                "usage smoke stdout differs from expected output",
                                (usage_run,),
                                (
                                    f"expected: {expected_output!r}",
                                    f"actual: {usage_run.stdout!r}",
                                ),
                            )
                        )
                    else:
                        checks.append(
                            CheckResult(
                                "usage-smoke-run",
                                Severity.PASS,
                                "usage smoke program exited successfully",
                                (usage_run,),
                            )
                        )

    return CompileHeaderReport(header_path, tuple(checks))


def bundle_source(
    source: str | os.PathLike[str],
    config: BuildConfig | None = None,
    *,
    output_path: str | os.PathLike[str] | None = None,
    run_input: str | None = None,
    expected_output: str | None = None,
) -> BundleReport:
    """Bundle one C++ source with ``-I REPOSITORY_ROOT/cp`` and compile it.

    Passing ``run_input`` (including an empty string) additionally executes the
    bundled binary.  ``expected_output`` is only meaningful with ``run_input``.
    The bundled text is returned even when no persistent output path is requested.
    """

    config = config or BuildConfig()
    source_path, error = _resolve_existing_file(source, config)
    resolved_output = None if output_path is None else Path(output_path)
    if resolved_output is not None and not resolved_output.is_absolute():
        resolved_output = (config.repository_root / resolved_output).resolve()
    if error is not None:
        return BundleReport(
            source_path,
            (CheckResult("bundle-source-path", Severity.FAIL, f"{source_path}: {error}"),),
            output_path=resolved_output,
        )

    checks: list[CheckResult] = []
    bundle_result = _run_command(
        (
            config.oj_bundle,
            "-I",
            os.fspath(config.repository_root / "cp"),
            os.fspath(source_path),
        ),
        timeout=config.bundle_timeout,
        cwd=config.repository_root if config.repository_root.is_dir() else None,
    )
    checks.append(
        _command_check(
            "oj-bundle", bundle_result, "oj-bundle completed successfully"
        )
    )
    if not bundle_result.succeeded:
        return BundleReport(
            source_path,
            tuple(checks),
            bundled_source=bundle_result.stdout or None,
            output_path=resolved_output,
        )

    bundled_source = bundle_result.stdout
    unresolved = tuple(_LOCAL_LIBRARY_INCLUDE_RE.findall(bundled_source))
    if unresolved:
        checks.append(
            CheckResult(
                "bundled-local-includes",
                Severity.FAIL,
                "bundled source retains unresolved library includes",
                details=unresolved,
            )
        )
    else:
        checks.append(
            CheckResult(
                "bundled-local-includes",
                Severity.PASS,
                "bundled source has no unresolved library includes",
            )
        )

    if resolved_output is not None:
        try:
            resolved_output.write_text(bundled_source, encoding="utf-8")
        except OSError as write_error:
            checks.append(
                CheckResult(
                    "bundle-output",
                    Severity.FAIL,
                    f"could not write {resolved_output}: {write_error}",
                )
            )
        else:
            checks.append(
                CheckResult(
                    "bundle-output",
                    Severity.PASS,
                    f"wrote bundled source: {resolved_output}",
                )
            )

    with tempfile.TemporaryDirectory(prefix="cp-dev-bundle-") as temporary:
        temp_root = Path(temporary)
        temporary_source = temp_root / "bundled.cpp"
        temporary_source.write_text(bundled_source, encoding="utf-8")
        bundled_executable = temp_root / "bundled"
        compile_result = _compile(
            config, (temporary_source,), bundled_executable
        )
        checks.append(
            _command_check(
                "bundled-gnu++23-compile",
                compile_result,
                "bundled source compiled as GNU++23",
            )
        )
        if compile_result.succeeded and run_input is not None:
            run_result = _run_command(
                (os.fspath(bundled_executable),),
                timeout=config.run_timeout,
                cwd=config.repository_root,
                stdin=run_input,
            )
            if not run_result.succeeded:
                checks.append(
                    _command_check(
                        "bundled-run", run_result, "bundled program passed"
                    )
                )
            elif expected_output is not None and run_result.stdout != expected_output:
                checks.append(
                    CheckResult(
                        "bundled-run",
                        Severity.FAIL,
                        "bundled program stdout differs from expected output",
                        (run_result,),
                        (
                            f"expected: {expected_output!r}",
                            f"actual: {run_result.stdout!r}",
                        ),
                    )
                )
            else:
                checks.append(
                    CheckResult(
                        "bundled-run",
                        Severity.PASS,
                        "bundled program exited successfully",
                        (run_result,),
                    )
                )

    return BundleReport(
        source_path,
        tuple(checks),
        bundled_source=bundled_source,
        output_path=resolved_output,
    )


def bundle_header(
    header: str | os.PathLike[str],
    config: BuildConfig | None = None,
    *,
    usage_code: str | None = None,
    output_path: str | os.PathLike[str] | None = None,
    run_input: str | None = None,
    expected_output: str | None = None,
) -> BundleReport:
    """Bundle a synthetic solution which includes exactly one project header.

    ``usage_code`` is the complete text following the include.  Supplying a main
    that instantiates representative APIs gives a stronger check than the default
    empty main and is recommended for stable headers.
    """

    config = config or BuildConfig()
    header_path, error = _resolve_existing_file(header, config)
    include_path = (
        None
        if error is not None
        else _repository_include(header_path, config.repository_root)
    )
    resolved_output = None if output_path is None else Path(output_path)
    if resolved_output is not None and not resolved_output.is_absolute():
        resolved_output = (config.repository_root / resolved_output).resolve()
    if error is not None or include_path is None:
        message = (
            f"{header_path}: {error}"
            if error is not None
            else f"header is outside repository root: {header_path}"
        )
        return BundleReport(
            header_path,
            (CheckResult("bundle-header-path", Severity.FAIL, message),),
            output_path=resolved_output,
        )

    body = usage_code if usage_code is not None else "int main() { return 0; }\n"
    if not body.endswith("\n"):
        body += "\n"
    with tempfile.TemporaryDirectory(prefix="cp-dev-bundle-header-") as temporary:
        synthetic_source = Path(temporary) / "header_usage.cpp"
        synthetic_source.write_text(
            f'#include "{include_path}"\n\n{body}', encoding="utf-8"
        )
        report = bundle_source(
            synthetic_source,
            config,
            output_path=resolved_output,
            run_input=run_input,
            expected_output=expected_output,
        )
    return BundleReport(
        header_path,
        report.checks,
        bundled_source=report.bundled_source,
        output_path=report.output_path,
    )


__all__ = [
    "BuildConfig",
    "BundleReport",
    "CheckResult",
    "CommandResult",
    "CompileHeaderReport",
    "DEFAULT_REQUIRED_DIRECTORIES",
    "DoctorReport",
    "Severity",
    "bundle_header",
    "bundle_source",
    "compile_header",
    "doctor",
]
