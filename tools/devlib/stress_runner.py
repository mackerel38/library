"""Deterministic differential-test runner with concrete failure preservation."""

from __future__ import annotations

from dataclasses import asdict, dataclass
from datetime import datetime, timezone
import hashlib
import json
import os
from pathlib import Path
import random
import shlex
import subprocess
import time
from typing import Any, Sequence


@dataclass(frozen=True, slots=True)
class ProcessRun:
    command: tuple[str, ...]
    returncode: int | None
    stdout: str
    stderr: str
    elapsed_seconds: float
    timed_out: bool = False

    @property
    def succeeded(self) -> bool:
        return not self.timed_out and self.returncode == 0

    def to_dict(self) -> dict[str, Any]:
        return asdict(self)


@dataclass(frozen=True, slots=True)
class StressFailure:
    classification: str
    message: str
    case_index: int
    case_seed: int
    input_text: str
    generator: ProcessRun
    oracle: ProcessRun | None
    candidate: ProcessRun | None
    saved_directory: Path | None = None


@dataclass(frozen=True, slots=True)
class StressReport:
    master_seed: int
    requested_cases: int
    completed_cases: int
    failure: StressFailure | None

    @property
    def ok(self) -> bool:
        return self.failure is None


def parse_command(command: str | Sequence[str]) -> tuple[str, ...]:
    """Normalize a command while never invoking a shell."""
    if isinstance(command, str):
        parts = tuple(shlex.split(command))
    else:
        parts = tuple(str(part) for part in command)
    if not parts:
        raise ValueError("command must not be empty")
    return parts


def _run(
    command: tuple[str, ...],
    *,
    timeout: float,
    stdin: str | None,
    cwd: Path,
    env: dict[str, str] | None = None,
) -> ProcessRun:
    started = time.perf_counter()
    try:
        completed = subprocess.run(
            command,
            cwd=cwd,
            input=stdin,
            text=True,
            encoding="utf-8",
            errors="replace",
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            timeout=timeout,
            check=False,
            shell=False,
            env=env,
        )
        return ProcessRun(
            command,
            completed.returncode,
            completed.stdout,
            completed.stderr,
            time.perf_counter() - started,
        )
    except subprocess.TimeoutExpired as exc:
        stdout = exc.stdout or ""
        stderr = exc.stderr or ""
        if isinstance(stdout, bytes):
            stdout = stdout.decode("utf-8", errors="replace")
        if isinstance(stderr, bytes):
            stderr = stderr.decode("utf-8", errors="replace")
        return ProcessRun(
            command,
            None,
            stdout,
            stderr,
            time.perf_counter() - started,
            timed_out=True,
        )
    except OSError as exc:
        return ProcessRun(
            command,
            127,
            "",
            f"{type(exc).__name__}: {exc}",
            time.perf_counter() - started,
        )


def _equal(expected: str, actual: str, comparator: str) -> bool:
    if comparator == "bytes":
        return expected == actual
    if comparator == "tokens":
        return expected.split() == actual.split()
    raise ValueError(f"unknown comparator: {comparator}")


def _failure(
    *,
    classification: str,
    message: str,
    case_index: int,
    case_seed: int,
    input_text: str,
    generator: ProcessRun,
    oracle: ProcessRun | None,
    candidate: ProcessRun | None,
) -> StressFailure:
    return StressFailure(
        classification,
        message,
        case_index,
        case_seed,
        input_text,
        generator,
        oracle,
        candidate,
    )


def run_stress(
    *,
    generator: str | Sequence[str],
    oracle: str | Sequence[str],
    candidate: str | Sequence[str],
    cases: int,
    master_seed: int,
    timeout: float,
    comparator: str = "tokens",
    cwd: Path | str = Path.cwd(),
    detect_nondeterminism: bool = False,
) -> StressReport:
    """Run deterministic differential tests and stop at the first failure.

    The generator receives CP_SEED and CP_CASE_INDEX environment variables and
    must print one complete test case to stdout.
    """
    if cases < 1:
        raise ValueError("cases must be positive")
    if timeout <= 0:
        raise ValueError("timeout must be positive")
    if comparator not in {"tokens", "bytes"}:
        raise ValueError("comparator must be tokens or bytes")

    generator_command = parse_command(generator)
    oracle_command = parse_command(oracle)
    candidate_command = parse_command(candidate)
    working_directory = Path(cwd).resolve()
    rng = random.Random(master_seed)

    for case_index in range(cases):
        case_seed = rng.getrandbits(64)
        environment = os.environ.copy()
        environment["CP_SEED"] = str(case_seed)
        environment["CP_CASE_INDEX"] = str(case_index)
        generated = _run(
            generator_command,
            timeout=timeout,
            stdin=None,
            cwd=working_directory,
            env=environment,
        )
        input_text = generated.stdout
        if generated.timed_out:
            failure = _failure(
                classification="GENERATOR_TLE",
                message="generator timed out",
                case_index=case_index,
                case_seed=case_seed,
                input_text=input_text,
                generator=generated,
                oracle=None,
                candidate=None,
            )
            return StressReport(master_seed, cases, case_index, failure)
        if generated.returncode != 0:
            failure = _failure(
                classification="GENERATOR_RE",
                message=f"generator exited with status {generated.returncode}",
                case_index=case_index,
                case_seed=case_seed,
                input_text=input_text,
                generator=generated,
                oracle=None,
                candidate=None,
            )
            return StressReport(master_seed, cases, case_index, failure)

        oracle_run = _run(
            oracle_command,
            timeout=timeout,
            stdin=input_text,
            cwd=working_directory,
        )
        if oracle_run.timed_out or oracle_run.returncode != 0:
            classification = "ORACLE_TLE" if oracle_run.timed_out else "ORACLE_RE"
            failure = _failure(
                classification=classification,
                message=(
                    "oracle timed out"
                    if oracle_run.timed_out
                    else f"oracle exited with status {oracle_run.returncode}"
                ),
                case_index=case_index,
                case_seed=case_seed,
                input_text=input_text,
                generator=generated,
                oracle=oracle_run,
                candidate=None,
            )
            return StressReport(master_seed, cases, case_index, failure)

        candidate_run = _run(
            candidate_command,
            timeout=timeout,
            stdin=input_text,
            cwd=working_directory,
        )
        if candidate_run.timed_out or candidate_run.returncode != 0:
            classification = "TLE" if candidate_run.timed_out else "RE"
            failure = _failure(
                classification=classification,
                message=(
                    "candidate timed out"
                    if candidate_run.timed_out
                    else f"candidate exited with status {candidate_run.returncode}"
                ),
                case_index=case_index,
                case_seed=case_seed,
                input_text=input_text,
                generator=generated,
                oracle=oracle_run,
                candidate=candidate_run,
            )
            return StressReport(master_seed, cases, case_index, failure)

        if detect_nondeterminism:
            repeated = _run(
                candidate_command,
                timeout=timeout,
                stdin=input_text,
                cwd=working_directory,
            )
            if (
                not repeated.succeeded
                or repeated.stdout != candidate_run.stdout
                or repeated.stderr != candidate_run.stderr
            ):
                failure = _failure(
                    classification="NONDETERMINISTIC",
                    message="candidate did not reproduce the same result",
                    case_index=case_index,
                    case_seed=case_seed,
                    input_text=input_text,
                    generator=generated,
                    oracle=oracle_run,
                    candidate=repeated,
                )
                return StressReport(master_seed, cases, case_index, failure)

        if not _equal(oracle_run.stdout, candidate_run.stdout, comparator):
            failure = _failure(
                classification="WA",
                message=f"{comparator} comparison failed",
                case_index=case_index,
                case_seed=case_seed,
                input_text=input_text,
                generator=generated,
                oracle=oracle_run,
                candidate=candidate_run,
            )
            return StressReport(master_seed, cases, case_index, failure)

    return StressReport(master_seed, cases, cases, None)


def save_failure(
    failure: StressFailure,
    *,
    root: Path | str,
    name: str,
    master_seed: int,
) -> StressFailure:
    """Persist a failure input and process evidence in a unique directory."""
    if not name or any(char not in "abcdefghijklmnopqrstuvwxyz0123456789_-" for char in name):
        raise ValueError("name must use lowercase letters, digits, underscore or hyphen")
    digest = hashlib.sha256(failure.input_text.encode("utf-8")).hexdigest()[:12]
    timestamp = datetime.now(timezone.utc).strftime("%Y%m%dT%H%M%SZ")
    directory = Path(root) / name / f"{timestamp}-{digest}"
    suffix = 1
    while directory.exists():
        directory = Path(root) / name / f"{timestamp}-{digest}-{suffix}"
        suffix += 1
    directory.mkdir(parents=True)

    (directory / "input.txt").write_text(failure.input_text, encoding="utf-8")
    if failure.oracle is not None:
        (directory / "oracle.out").write_text(
            failure.oracle.stdout, encoding="utf-8"
        )
        (directory / "oracle.err").write_text(
            failure.oracle.stderr, encoding="utf-8"
        )
    if failure.candidate is not None:
        (directory / "candidate.out").write_text(
            failure.candidate.stdout, encoding="utf-8"
        )
        (directory / "candidate.err").write_text(
            failure.candidate.stderr, encoding="utf-8"
        )
    evidence = {
        "case_index": failure.case_index,
        "case_seed": failure.case_seed,
        "classification": failure.classification,
        "generator": failure.generator.to_dict(),
        "master_seed": master_seed,
        "message": failure.message,
        "oracle": None if failure.oracle is None else failure.oracle.to_dict(),
        "candidate": (
            None if failure.candidate is None else failure.candidate.to_dict()
        ),
    }
    (directory / "failure.json").write_text(
        json.dumps(evidence, ensure_ascii=False, indent=2, sort_keys=True) + "\n",
        encoding="utf-8",
    )
    return StressFailure(
        classification=failure.classification,
        message=failure.message,
        case_index=failure.case_index,
        case_seed=failure.case_seed,
        input_text=failure.input_text,
        generator=failure.generator,
        oracle=failure.oracle,
        candidate=failure.candidate,
        saved_directory=directory,
    )


__all__ = [
    "ProcessRun",
    "StressFailure",
    "StressReport",
    "parse_command",
    "run_stress",
    "save_failure",
]
