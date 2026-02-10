#!/usr/bin/env python3
"""
Generate .tex files from source code using the preprocessor.

This script scans the src/ directory and generates corresponding .tex files
in the generated/ directory using preprocessor.py.

Usage: python3 generate_tex.py [--clean]
"""
import os
import sys
import subprocess
from pathlib import Path

SCRIPT_DIR = Path(__file__).parent.resolve()
SRC_DIR = SCRIPT_DIR.parent / "src"
GENERATED_DIR = SCRIPT_DIR / "generated"

# File extensions to process
CODE_EXTENSIONS = {'.cpp', '.c', '.h', '.hpp'}


def clean_generated():
    """Remove all generated .tex files."""
    import shutil
    if GENERATED_DIR.exists():
        shutil.rmtree(GENERATED_DIR)
    print(f"Cleaned {GENERATED_DIR}")


def generate_tex_for_file(src_path: Path, out_path: Path) -> bool:
    """
    Generate .tex file for a source file.

    Returns True if successful, False otherwise.
    """
    # Ensure output directory exists
    out_path.parent.mkdir(parents=True, exist_ok=True)

    # Run preprocessor
    try:
        result = subprocess.run(
            [sys.executable, str(SCRIPT_DIR / "preprocessor.py"), str(src_path)],
            capture_output=True,
            text=True,
            cwd=str(SCRIPT_DIR)
        )

        if result.returncode != 0:
            print(f"  ERROR processing {src_path}: {result.stderr}", file=sys.stderr)
            return False

        # Write output
        with out_path.open('w', encoding='utf-8') as f:
            f.write(result.stdout)

        return True

    except Exception as e:
        print(f"  ERROR processing {src_path}: {e}", file=sys.stderr)
        return False


def generate_all():
    """Generate .tex files for all source files."""
    if not SRC_DIR.exists():
        print(f"Error: Source directory not found: {SRC_DIR}", file=sys.stderr)
        sys.exit(1)

    # Track statistics
    total = 0
    success = 0
    skipped = 0

    # Process each category directory
    for category_dir in sorted(SRC_DIR.iterdir()):
        if not category_dir.is_dir():
            continue
        if category_dir.name.startswith('.'):
            continue

        print(f"\n=== {category_dir.name} ===")

        # Process each file in the category
        for src_file in sorted(category_dir.iterdir()):
            if src_file.suffix not in CODE_EXTENSIONS:
                continue

            total += 1

            # Determine output path
            out_file = GENERATED_DIR / category_dir.name / f"{src_file.stem}.tex"

            # Check if regeneration needed (compare timestamps)
            if out_file.exists():
                if out_file.stat().st_mtime >= src_file.stat().st_mtime:
                    # Output is newer than source, skip
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
