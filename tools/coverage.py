#!/usr/bin/env python3
import argparse
import csv
from collections import Counter
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
TABLE = ROOT / "coverage" / "problems.tsv"
REQUIRED_COLUMNS = [
    "contest",
    "task",
    "problem_url",
    "editorial_url",
    "reusable_parts",
    "recipe",
    "problem_specific_core",
    "tool",
    "current",
    "missing",
    "pitfalls",
    "reviewer",
    "reviewed_on",
    "status",
    "notes",
]
REVIEWED_COLUMNS = [
    "reusable_parts",
    "problem_specific_core",
    "current",
    "missing",
    "pitfalls",
    "reviewer",
    "reviewed_on",
]


def read_rows():
    with TABLE.open(encoding="utf-8", newline="") as file:
        reader = csv.DictReader(file, delimiter="\t")
        if reader.fieldnames != REQUIRED_COLUMNS:
            raise ValueError(f"列が不正です: {reader.fieldnames}")
        return list(reader)


def validate(rows):
    errors = []
    seen = set()
    for line, row in enumerate(rows, 2):
        key = (row["contest"], row["task"])
        if key in seen:
            errors.append(f"{line}: 重複しています: {key}")
        seen.add(key)
        expected = (
            f"https://atcoder.jp/contests/{row['contest']}/tasks/"
            f"{row['contest']}_{row['task']}"
        )
        if row["problem_url"] != expected:
            errors.append(f"{line}: problem_urlが規約と異なります")
        if row["status"] not in {"pending", "reviewed"}:
            errors.append(f"{line}: statusはpendingまたはreviewedにしてください")
        if row["status"] == "reviewed":
            for column in REVIEWED_COLUMNS:
                if not row[column].strip():
                    errors.append(f"{line}: reviewedですが{column}が空です")
    return errors


def main():
    parser = argparse.ArgumentParser(description="ABC過去問監査表を検査・集計する")
    parser.add_argument("command", choices=["validate", "summary"])
    args = parser.parse_args()
    rows = read_rows()
    errors = validate(rows)
    if errors:
        for error in errors:
            print(error)
        raise SystemExit(1)
    if args.command == "validate":
        print(f"OK: {len(rows)}問")
        return
    status = Counter(row["status"] for row in rows)
    missing = sum(
        row["status"] == "reviewed" and row["missing"].strip().lower() != "none"
        for row in rows
    )
    print(f"対象: {len(rows)}問")
    print(f"確認済み: {status['reviewed']}問")
    print(f"未確認: {status['pending']}問")
    print(f"不足あり: {missing}問")


if __name__ == "__main__":
    main()
