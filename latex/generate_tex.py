#!/usr/bin/env python3
"""
Generate .tex files from source code using the preprocessor.

Scans the src/ directory and generates corresponding .tex files
in the generated/ directory using preprocessor.py.

Usage: python3 generate_tex.py [--clean | --force]
"""
import sys
import subprocess
from pathlib import Path

from common import SCRIPT_DIR
import preprocessor

SRC_DIR = SCRIPT_DIR.parent / "src"
GENERATED_DIR = SCRIPT_DIR / "generated"

# File extensions to process (empty string for extensionless files like makefile)
CODE_EXTENSIONS = {'.cpp', '.c', '.h', '.hpp', '.sh', '.py', ''}


def clean_generated():
    """Remove all generated .tex files."""
    import shutil
    if GENERATED_DIR.exists():
        shutil.rmtree(GENERATED_DIR)
    print(f"Cleaned {GENERATED_DIR}")


def generate_tex_for_file(src_path: Path, out_path: Path) -> bool:
    """Generate .tex file for a source file. Returns True if successful."""
    out_path.parent.mkdir(parents=True, exist_ok=True)

    try:
        # Import directly to avoid starting >60 python processes
        latex_content = preprocessor.generate_listing(src_path)

        with out_path.open('w', encoding='utf-8') as f:
            f.write(latex_content)

        return True

    except Exception as e:
        print(f"  ERROR processing {src_path}: {e}", file=sys.stderr)
        return False


def generate_all():
    """Generate .tex files for all source files."""
    if not SRC_DIR.exists():
        print(f"Error: Source directory not found: {SRC_DIR}", file=sys.stderr)
        sys.exit(1)

    total = 0
    success = 0
    skipped = 0

    for category_dir in sorted(SRC_DIR.iterdir()):
        if not category_dir.is_dir() or category_dir.name.startswith('.'):
            continue

        print(f"\n=== {category_dir.name} ===")

        for src_file in sorted(category_dir.iterdir()):
            if not src_file.is_file() or src_file.suffix not in CODE_EXTENSIONS:
                continue

            total += 1
            out_file = GENERATED_DIR / category_dir.name / f"{src_file.stem}.tex"

            # Skip if output is newer than source AND newer than preprocessor.py
            preprocessor_mtime = (SCRIPT_DIR / "preprocessor.py").stat().st_mtime
            if out_file.exists() and out_file.stat().st_mtime >= src_file.stat().st_mtime and out_file.stat().st_mtime >= preprocessor_mtime:
                print(f"  [skip] {src_file.name}")
                skipped += 1
                success += 1
                continue

            print(f"  [gen]  {src_file.name} -> {out_file.relative_to(SCRIPT_DIR)}")

            if generate_tex_for_file(src_file, out_file):
                success += 1

    print(f"\n--- Summary ---")
    print(f"Total: {total}, Generated: {success - skipped}, Skipped: {skipped}, Errors: {total - success}")


def main():
    if len(sys.argv) > 1 and sys.argv[1] == '--clean':
        clean_generated()
        return

    if len(sys.argv) > 1 and sys.argv[1] == '--force':
        clean_generated()

    generate_all()


if __name__ == '__main__':
    main()
