#!/usr/bin/env python3
"""
Generate the finals LaTeX document from finals.yaml configuration.

This script reads finals.yaml and generates icpclib-finals.tex containing
only the specified subset of files for the 25-page limit.

Usage: python3 generate_finals.py
"""
import sys
from pathlib import Path

try:
    import yaml
except ImportError:
    print("Error: PyYAML required. Install with: pip install pyyaml", file=sys.stderr)
    sys.exit(1)

SCRIPT_DIR = Path(__file__).parent.resolve()
CONFIG_FILE = SCRIPT_DIR / "finals.yaml"
OUTPUT_FILE = SCRIPT_DIR / "icpclib-finals.tex"
GENERATED_DIR = SCRIPT_DIR / "generated"
THEORETICAL_CONTENT_DIR = SCRIPT_DIR / "theoretical" / "content"

# Chapter display names
CHAPTER_NAMES = {
    'structures': 'Data Structures',
    'graphs': 'Graphs',
    'math': 'Math',
    'strings': 'Strings',
    'dp': 'Dynamic Programming',
    'geometry': 'Geometry',
    'extra': 'Extra',
}


def strip_numeric_prefix(name: str) -> str:
    """Strip leading 'NN_' numeric prefix from a directory/file name."""
    parts = name.split('_', 1)
    if len(parts) == 2 and parts[0].isdigit():
        return parts[1]
    return name


def slug_to_display_name(slug: str) -> str:
    """Convert an underscore slug to a Title Case display name."""
    return ' '.join(word.capitalize() for word in slug.split('_'))


def find_theoretical_file(chapter_slug: str, subsection_slug: str) -> Path | None:
    """Find the .tex file for a theoretical chapter+subsection by slug."""
    for chapter_dir in sorted(THEORETICAL_CONTENT_DIR.iterdir()):
        if not chapter_dir.is_dir():
            continue
        if strip_numeric_prefix(chapter_dir.name) != chapter_slug:
            continue
        for sub_file in sorted(chapter_dir.iterdir()):
            if sub_file.suffix == '.tex' and strip_numeric_prefix(sub_file.stem) == subsection_slug:
                return sub_file
    return None


def generate_theoretical_tex(theoretical: dict) -> str:
    """Generate LaTeX for the theoretical section."""
    if not theoretical:
        return ''

    lines = [
        '',
        r'% === THEORETICAL GUIDE ===',
        r'\etocdepthtag.toc{theoretical}',
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
            # Path relative to latex/
            rel = f'theoretical/content/{sub_file.parent.name}/{sub_file.stem}'
            lines.append(f'\\input{{{rel}}}')

        lines.append('')

    return '\n'.join(lines)


def load_config() -> dict:
    """Load and validate the finals configuration."""
    if not CONFIG_FILE.exists():
        print(f"Error: Config file not found: {CONFIG_FILE}", file=sys.stderr)
        sys.exit(1)

    with CONFIG_FILE.open('r', encoding='utf-8') as f:
        config = yaml.safe_load(f)

    # Validate required fields
    required = ['team', 'chapters']
    for field in required:
        if field not in config:
            print(f"Error: Missing required field '{field}' in config", file=sys.stderr)
            sys.exit(1)

    return config


def generate_chapter_tex(chapter: str, files: list) -> str:
    """Generate LaTeX for a chapter."""
    lines = []

    # Section header
    display_name = CHAPTER_NAMES.get(chapter, chapter.title())
    lines.append(f"\\section{{{display_name}}}")
    lines.append("")

    # Include each file
    for filename in files:
        # Convert .cpp to .tex path
        stem = Path(filename).stem
        tex_path = f"generated/{chapter}/{stem}.tex"

        # Check if generated file exists
        full_path = SCRIPT_DIR / tex_path.replace('/', '/')
        if not full_path.exists():
            print(f"  Warning: Generated file not found: {tex_path}", file=sys.stderr)

        lines.append(f"\\input{{{tex_path}}}")

    lines.append("")
    return '\n'.join(lines)


def generate_finals_tex(config: dict) -> str:
    """Generate the complete finals LaTeX document."""
    team = config['team']
    chapters = config['chapters']

    teamname = team.get('name', 'Team')
    university = team.get('university', '')

    # Build team author string
    members = team.get('members', [])
    if members:
        author = ', '.join(members)
    else:
        author = teamname

    lines = []

    # Document header
    lines.append(r"% ICPC Library - Finals Edition")
    lines.append(r"% Auto-generated from finals.yaml - DO NOT EDIT DIRECTLY")
    lines.append(r"%")
    lines.append(r"")
    lines.append(r"\documentclass[9pt,a4paper,landscape]{extarticle}")
    lines.append(r"\usepackage{icpclibpkg}")
    lines.append(r"")
    lines.append(f"\\teamname{{{teamname}}}")
    lines.append(f"\\universityname{{{university}}}")
    lines.append(f"\\author{{{author}}}")
    lines.append(r"\date{\today}")
    lines.append(r"")
    lines.append(r"\begin{document}")
    lines.append(r"")

    # Cover page
    lines.append(r"% Cover page")
    lines.append(r"\makecover")
    lines.append(r"")
    lines.append(r"\vspace{2mm}")
    lines.append(r"")

    theoretical = config.get('theoretical') or {}

    # Two TOC blocks: code lib and theoretical guide
    lines.append(r"% Table of Contents")
    lines.append(r"\begingroup")
    lines.append(r"\footnotesize")
    lines.append(r"\setlength{\columnsep}{3mm}")
    lines.append(r"\etocsettocstyle{}{}")
    lines.append(r"")
    lines.append(r"% Code Contents (4 columns)")
    lines.append(r"{\small\bfseries Code Contents}\par\vspace{0.5mm}")
    lines.append(r"\etocsettagdepth{code}{subsection}")
    lines.append(r"\etocsettagdepth{theoretical}{none}")
    lines.append(r"\begin{multicols}{4}")
    lines.append(r"\tableofcontents")
    lines.append(r"\end{multicols}")
    lines.append(r"")
    lines.append(r"\vspace{1mm}")
    lines.append(r"")
    lines.append(r"% Theoretical Contents (4 columns)")
    lines.append(r"{\small\bfseries Theoretical Contents}\par\vspace{0.5mm}")
    lines.append(r"\etocsettagdepth{code}{none}")
    lines.append(r"\etocsettagdepth{theoretical}{subsection}")
    lines.append(r"\begin{multicols}{4}")
    lines.append(r"\tableofcontents")
    lines.append(r"\end{multicols}")
    lines.append(r"")
    lines.append(r"\endgroup")
    lines.append(r"")
    lines.append(r"\vspace{2mm}")
    lines.append(r"\hrule")
    lines.append(r"\vspace{2mm}")
    lines.append(r"")

    # Main content
    lines.append(r"% Main content")
    lines.append(r"\begin{multicols*}{3}")
    lines.append(r"")

    # Tag code entries
    lines.append(r"\etocdepthtag.toc{code}")
    lines.append(r"")

    # Generate chapters in order
    for chapter, files in chapters.items():
        if files:
            lines.append(f"% === {chapter.upper()} ===")
            lines.append(generate_chapter_tex(chapter, files))

    # Theoretical section (if any)
    if theoretical:
        lines.append(generate_theoretical_tex(theoretical))

    lines.append(r"\end{multicols*}")
    lines.append(r"")
    lines.append(r"\end{document}")

    return '\n'.join(lines)


def main():
    print("=== Generating finals LaTeX document ===")

    config = load_config()

    # Count files
    total_files = sum(len(files) for files in config['chapters'].values())
    theoretical = config.get('theoretical') or {}
    th_count = sum(len(s) for s in theoretical.values() if s)
    print(f"Team: {config['team'].get('name', '')}")
    print(f"Code files: {total_files}, Theoretical subsections: {th_count}")

    # Generate document
    latex = generate_finals_tex(config)

    # Write output
    with OUTPUT_FILE.open('w', encoding='utf-8') as f:
        f.write(latex)

    print(f"Generated: {OUTPUT_FILE}")


if __name__ == '__main__':
    main()
