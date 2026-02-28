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
THEORETICAL_CONTENT_DIR = REPO_ROOT / "latex" / "theoretical" / "content"


def strip_numeric_prefix(name: str) -> str:
    """Strip leading 'NN_' numeric prefix from a directory/file name."""
    parts = name.split('_', 1)
    if len(parts) == 2 and parts[0].isdigit():
        return parts[1]
    return name


def find_theoretical_file(chapter_slug: str, subsection_slug: str) -> Path | None:
    """Find the .tex file for a theoretical chapter+subsection by slug."""
    for chapter_dir in THEORETICAL_CONTENT_DIR.iterdir():
        if not chapter_dir.is_dir():
            continue
        if strip_numeric_prefix(chapter_dir.name) != chapter_slug:
            continue
        for sub_file in chapter_dir.iterdir():
            if sub_file.suffix == '.tex' and strip_numeric_prefix(sub_file.stem) == subsection_slug:
                return sub_file
    return None


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

    missing = []

    # Validate code chapters
    chapters = config.get("chapters") or {}
    for chapter, files in chapters.items():
        if not files:
            continue
        for filename in files:
            path = SRC_DIR / chapter / filename
            if not path.exists():
                missing.append(f"  src/{chapter}/{filename}")

    # Validate theoretical chapters
    theoretical = config.get("theoretical") or {}
    for chapter_slug, subsections in theoretical.items():
        if not subsections:
            continue
        for subsection_slug in subsections:
            path = find_theoretical_file(chapter_slug, subsection_slug)
            if path is None:
                missing.append(f"  theoretical/content/{chapter_slug}/{subsection_slug}.tex")

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

    code_count = sum(len(f) for f in chapters.values() if f)
    th_count = sum(len(s) for s in theoretical.values() if s)
    print(f"OK: {code_count} code files and {th_count} theoretical subsections in finals.yaml exist.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
