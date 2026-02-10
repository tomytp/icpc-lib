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


def load_config() -> dict:
    """Load and validate the finals configuration."""
    if not CONFIG_FILE.exists():
        print(f"Error: Config file not found: {CONFIG_FILE}", file=sys.stderr)
        sys.exit(1)

    with CONFIG_FILE.open('r', encoding='utf-8') as f:
        config = yaml.safe_load(f)

    # Validate required fields
    required = ['title', 'team', 'chapters']
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
    title = config['title']
    team = config['team']
    chapters = config['chapters']

    # Build team author string
    members = team.get('members', [])
    if members:
        author = ', '.join(members)
    else:
        author = team.get('name', 'Team')

    lines = []

    # Document header
    lines.append(r"% ICPC Library - Finals Edition")
    lines.append(r"% Auto-generated from finals.yaml - DO NOT EDIT DIRECTLY")
    lines.append(r"%")
    lines.append(r"")
    lines.append(r"\documentclass[9pt,a4paper,landscape]{extarticle}")
    lines.append(r"\usepackage{icpclibpkg}")
    lines.append(r"")
    lines.append(f"\\title{{{title}}}")
    lines.append(f"\\author{{{author}}}")
    lines.append(r"\date{\today}")
    lines.append(r"")
    lines.append(r"\begin{document}")
    lines.append(r"")

    # Title
    lines.append(r"% Title")
    lines.append(r"\begin{center}")
    lines.append(f"{{\\LARGE\\textbf{{{title}}}}}\\\\[1ex]")
    lines.append(f"{{\\large {author}}}\\\\[0.5ex]")
    lines.append(r"{\small\today}")
    lines.append(r"\end{center}")
    lines.append(r"")
    lines.append(r"\vspace{2mm}")
    lines.append(r"")

    # Compact TOC
    lines.append(r"% Table of Contents")
    lines.append(r"\begingroup")
    lines.append(r"\footnotesize")
    lines.append(r"\setlength{\columnsep}{3mm}")
    lines.append(r"\begin{multicols}{4}")
    lines.append(r"\tableofcontents")
    lines.append(r"\end{multicols}")
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

    # Generate chapters in order
    for chapter, files in chapters.items():
        if files:
            lines.append(f"% === {chapter.upper()} ===")
            lines.append(generate_chapter_tex(chapter, files))

    lines.append(r"\end{multicols*}")
    lines.append(r"")
    lines.append(r"\end{document}")

    return '\n'.join(lines)


def main():
    print("=== Generating finals LaTeX document ===")

    config = load_config()

    # Count files
    total_files = sum(len(files) for files in config['chapters'].values())
    print(f"Title: {config['title']}")
    print(f"Files: {total_files}")

    # Generate document
    latex = generate_finals_tex(config)

    # Write output
    with OUTPUT_FILE.open('w', encoding='utf-8') as f:
        f.write(latex)

    print(f"Generated: {OUTPUT_FILE}")


if __name__ == '__main__':
    main()
