"""Read and update development metadata without duplicating include dependencies."""

from __future__ import annotations

import json
import os
import tempfile
from pathlib import Path
from typing import Any


METADATA_FILENAME = "library.json"
VALID_STATUSES = {"experimental", "beta", "stable", "deprecated"}


class MetadataError(ValueError):
    """Raised when library metadata is missing or malformed."""


def load_metadata(repo_root: Path) -> dict[str, Any]:
    """Load and minimally validate library.json from repo_root."""
    path = repo_root / METADATA_FILENAME
    if not path.is_file():
        raise MetadataError(f"metadata file not found: {path}")
    try:
        data = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        raise MetadataError(f"failed to read {path}: {exc}") from exc
    if not isinstance(data, dict):
        raise MetadataError("metadata root must be a JSON object")
    if data.get("schema_version") != 1:
        raise MetadataError("unsupported metadata schema_version")
    modules = data.get("modules")
    if not isinstance(modules, dict):
        raise MetadataError("metadata.modules must be a JSON object")
    for module_path, entry in modules.items():
        if not isinstance(module_path, str) or not isinstance(entry, dict):
            raise MetadataError("each module entry must map a path to an object")
        layer = entry.get("layer")
        status = entry.get("status")
        if not isinstance(layer, int) or layer < 0:
            raise MetadataError(f"{module_path}: layer must be a non-negative integer")
        if status not in VALID_STATUSES:
            raise MetadataError(f"{module_path}: invalid status {status!r}")
    return data


def save_metadata(repo_root: Path, data: dict[str, Any]) -> None:
    """Atomically write metadata in a deterministic, human-reviewable form."""
    path = repo_root / METADATA_FILENAME
    payload = json.dumps(data, ensure_ascii=False, indent=2, sort_keys=True) + "\n"
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp_name: str | None = None
    try:
        with tempfile.NamedTemporaryFile(
            mode="w",
            encoding="utf-8",
            dir=path.parent,
            prefix=f".{path.name}.",
            delete=False,
        ) as tmp:
            tmp.write(payload)
            tmp_name = tmp.name
        os.replace(tmp_name, path)
    finally:
        if tmp_name is not None and os.path.exists(tmp_name):
            os.unlink(tmp_name)


def module_entry(
    *,
    layer: int,
    docs: str,
    usage_test: str | None,
) -> dict[str, Any]:
    """Build the initial metadata entry for an experimental module."""
    entry: dict[str, Any] = {
        "benchmark": [],
        "docs": docs,
        "layer": layer,
        "status": "experimental",
        "summary_ja": "",
        "symbols": [],
        "tags": [],
        "tests": [],
        "verify": [],
    }
    if usage_test is not None:
        entry["tests"].append(usage_test)
    return entry


def toolchain(data: dict[str, Any]) -> tuple[str, list[str]]:
    """Return the configured C++ compiler and judge flags."""
    config = data.get("toolchain", {})
    if not isinstance(config, dict):
        raise MetadataError("metadata.toolchain must be a JSON object")
    cxx = config.get("cxx", "g++")
    flags = config.get("judge_flags", ["-std=gnu++23", "-O2", "-pipe"])
    if not isinstance(cxx, str) or not isinstance(flags, list) or not all(
        isinstance(flag, str) for flag in flags
    ):
        raise MetadataError("invalid toolchain compiler or judge_flags")
    return cxx, list(flags)
