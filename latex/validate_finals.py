#!/usr/bin/env python3
"""
Validate that every file listed in finals.yaml actually exists in src/.

Exits with code 1 and prints an error if any referenced file is missing,
so it can be used both as a CI step and as a pre-commit hook check.

Usage: python3 latex/validate_finals.py
"""
import sys
from pathlib import Path

try:
    import yaml
except ImportError:
    print("Error: PyYAML required. Run: make install-dependencies", file=sys.stderr)
    sys.exit(1)

REPO_ROOT = Path(__file__).parent.parent.resolve()
SRC_DIR = REPO_ROOT / "src"


def main() -> int:
    # Allow an explicit path (e.g. a temp file with the staged contents) so
    # the pre-commit hook can validate what is staged rather than the
    # working-tree copy.
    if len(sys.argv) > 1:
        config_file = Path(sys.argv[1])
    else:
        config_file = REPO_ROOT / "latex" / "finals.yaml"

    if not config_file.exists():
        print(f"Error: {config_file} not found", file=sys.stderr)
        return 1

    with config_file.open() as f:
        config = yaml.safe_load(f)

    chapters = config.get("chapters") or {}
    missing = []

    for chapter, files in chapters.items():
        if not files:
            continue
        for filename in files:
            path = SRC_DIR / chapter / filename
            if not path.exists():
                missing.append(f"  src/{chapter}/{filename}")

    if missing:
        print("ERROR: finals.yaml references files that do not exist:", file=sys.stderr)
        for m in missing:
            print(m, file=sys.stderr)
        print(file=sys.stderr)
        print(
            "Update latex/finals.yaml to match the current filenames.",
            file=sys.stderr,
        )
        return 1

    print(f"OK: all {sum(len(f) for f in chapters.values() if f)} files in finals.yaml exist.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
