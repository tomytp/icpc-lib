#!/usr/bin/env python3
"""
Generate the finals LaTeX document from finals.yaml configuration.

Reads finals.yaml and generates icpclib-finals.tex containing only the
specified subset of files for the 25-page limit.

Usage: python3 generate_finals.py
"""
import sys
from pathlib import Path

try:
    import yaml
except ImportError:
    print("Error: PyYAML required. Install with: pip install pyyaml", file=sys.stderr)
    sys.exit(1)

from common import (
    SCRIPT_DIR, CONFIG_FILE, GENERATED_DIR,
    CHAPTER_NAMES,
    slug_to_display_name, find_theoretical_file, render_document,
)

OUTPUT_FILE = SCRIPT_DIR / "icpclib-finals.tex"


def load_config() -> dict:
    """Load and validate the finals configuration."""
    if not CONFIG_FILE.exists():
        print(f"Error: Config file not found: {CONFIG_FILE}", file=sys.stderr)
        sys.exit(1)

    with CONFIG_FILE.open('r', encoding='utf-8') as f:
        config = yaml.safe_load(f)

    for field in ('team', 'chapters'):
        if field not in config:
            print(f"Error: Missing required field '{field}' in config", file=sys.stderr)
            sys.exit(1)

    return config


def generate_code_body(chapters: dict) -> str:
    """Generate LaTeX for the code chapters listed in config."""
    lines = [r'\etocdepthtag.toc{code}', '']

    for chapter, files in chapters.items():
        if not files:
            continue

        display_name = CHAPTER_NAMES.get(chapter, chapter.title())
        lines.append(f'% === {chapter.upper()} ===')
        lines.append(f'\\section{{{display_name}}}')
        lines.append('')

        for filename in files:
            stem = Path(filename).stem
            tex_path = f'generated/{chapter}/{stem}.tex'

            full_path = SCRIPT_DIR / tex_path
            if not full_path.exists():
                print(f"  Warning: Generated file not found: {tex_path}", file=sys.stderr)

            lines.append(f'\\input{{{tex_path}}}')

        lines.append('')

    return '\n'.join(lines)


def generate_theoretical_body(theoretical: dict) -> str:
    """Generate LaTeX for the theoretical section."""
    if not theoretical:
        return ''

    lines = [
        '',
        r'% === THEORETICAL GUIDE ===',
        r'\etocdepthtag.toc{theoretical}',
        r'\renewcommand{\theHsection}{th.\arabic{section}}',
        r'\setcounter{section}{0}',
        '',
    ]

    for chapter_slug, subsections in theoretical.items():
        if not subsections:
            continue

        display_name = slug_to_display_name(chapter_slug)
        lines.append(f'\\section{{{display_name}}}')
        lines.append('')

        for subsection_slug in subsections:
            sub_file = find_theoretical_file(chapter_slug, subsection_slug)
            if sub_file is None:
                print(f'  Warning: theoretical subsection not found: {chapter_slug}/{subsection_slug}',
                      file=sys.stderr)
                continue
            rel = f'theoretical/content/{sub_file.parent.name}/{sub_file.stem}'
            lines.append(f'\\input{{{rel}}}')

        lines.append('')

    return '\n'.join(lines)


def main():
    print("=== Generating finals LaTeX document ===")

    config = load_config()
    chapters = config['chapters']
    theoretical = config.get('theoretical') or {}

    total_files = sum(len(files) for files in chapters.values())
    th_count = sum(len(s) for s in theoretical.values() if s)
    print(f"Team: {config['team'].get('name', '')}")
    print(f"Code files: {total_files}, Theoretical subsections: {th_count}")

    latex = render_document(
        team=config['team'],
        code_body=generate_code_body(chapters),
        theoretical_body=generate_theoretical_body(theoretical),
        edition="Finals",
        comment="Auto-generated from finals.yaml - DO NOT EDIT DIRECTLY",
    )

    with OUTPUT_FILE.open('w', encoding='utf-8') as f:
        f.write(latex)

    print(f"Generated: {OUTPUT_FILE}")


if __name__ == '__main__':
    main()
