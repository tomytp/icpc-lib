"""
Shared utilities for the ICPC library LaTeX generators.

Contains common constants, helper functions, and the LaTeX document
skeleton used by both generate_full.py and generate_finals.py.
"""
from pathlib import Path

try:
    import yaml
    HAS_YAML = True
except ImportError:
    HAS_YAML = False

SCRIPT_DIR = Path(__file__).parent.resolve()
CONFIG_FILE = SCRIPT_DIR / "finals.yaml"
GENERATED_DIR = SCRIPT_DIR / "generated"
THEORETICAL_CONTENT_DIR = SCRIPT_DIR / "theoretical" / "content"

# Chapter display names (single source of truth)
CHAPTER_NAMES = {
    'structures': 'Data Structures',
    'graphs': 'Graph Algorithms',
    'math': 'Mathematics',
    'strings': 'String Algorithms',
    'dp': 'Dynamic Programming',
    'geometry': 'Computational Geometry',
    'extra': 'Extra (Templates and Utilities)',
}

# Preferred chapter order
CHAPTER_ORDER = ['structures', 'graphs', 'math', 'strings', 'dp', 'geometry', 'extra']


def strip_numeric_prefix(name: str) -> str:
    """Strip leading 'NN_' numeric prefix from a directory/file name."""
    parts = name.split('_', 1)
    if len(parts) == 2 and parts[0].isdigit():
        return parts[1]
    return name


def slug_to_display_name(slug: str) -> str:
    """Convert an underscore slug to a Title Case display name."""
    return ' '.join(word.capitalize() for word in slug.split('_'))


def load_team_info() -> dict:
    """Load team info from finals.yaml if available."""
    if not HAS_YAML or not CONFIG_FILE.exists():
        return {}
    with CONFIG_FILE.open('r', encoding='utf-8') as f:
        config = yaml.safe_load(f)
    return config.get('team', {})


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


def render_document(
    team: dict,
    code_body: str,
    theoretical_body: str,
    edition: str = "Full",
    comment: str = "",
) -> str:
    """
    Render a complete LaTeX document with cover page, dual TOC, and content.

    Args:
        team: dict with 'name', 'university', 'members' keys
        code_body: LaTeX string for the code chapters (inside multicols)
        theoretical_body: LaTeX string for theoretical sections (inside multicols)
        edition: Edition label for the header comment
        comment: Extra comment line (e.g. generator script name)
    """
    teamname = team.get('name', 'ICPC Team')
    university = team.get('university', '')
    members = team.get('members', [])
    author = ', '.join(str(m) for m in members) if members else teamname

    parts = []

    # Header
    parts.append(f'% ICPC Library - {edition} Edition')
    if comment:
        parts.append(f'% {comment}')
    parts.append('%')
    parts.append('')
    parts.append(r'\documentclass[9pt,a4paper,landscape]{extarticle}')
    parts.append(r'\usepackage{icpclibpkg}')
    parts.append('')
    parts.append(f'\\teamname{{{teamname}}}')
    parts.append(f'\\universityname{{{university}}}')
    parts.append(f'\\author{{{author}}}')
    parts.append(r'\date{\today}')
    parts.append('')
    parts.append(r'\begin{document}')
    parts.append('')

    # Cover page
    parts.append(r'% Cover page')
    parts.append(r'\makecover')
    parts.append(r'\thispagestyle{fancy}')
    parts.append('')
    parts.append(r'\vspace{2mm}')
    parts.append('')

    # Dual TOC
    parts.append(r'% Table of Contents')
    parts.append(r'\begingroup')
    parts.append(r'\footnotesize')
    parts.append(r'\setlength{\columnsep}{3mm}')
    parts.append(r'\etocsettocstyle{}{}')
    parts.append('')
    parts.append(r'% Code Contents (4 columns)')
    parts.append(r'{\small\bfseries Code Contents}\par\vspace{0.5mm}')
    parts.append(r'\etocsettagdepth{code}{subsection}')
    parts.append(r'\etocsettagdepth{theoretical}{none}')
    parts.append(r'\begin{multicols}{4}')
    parts.append(r'\tableofcontents')
    parts.append(r'\end{multicols}')
    parts.append('')
    parts.append(r'\vspace{1mm}')
    parts.append('')
    parts.append(r'% Theoretical Contents (4 columns)')
    parts.append(r'{\small\bfseries Theoretical Contents}\par\vspace{0.5mm}')
    parts.append(r'\etocsettagdepth{code}{none}')
    parts.append(r'\etocsettagdepth{theoretical}{subsection}')
    parts.append(r'\begin{multicols}{4}')
    parts.append(r'\tableofcontents')
    parts.append(r'\end{multicols}')
    parts.append('')
    parts.append(r'\endgroup')
    parts.append('')
    parts.append(r'\vspace{2mm}')
    parts.append(r'\hrule')
    parts.append(r'\vspace{2mm}')
    parts.append('')

    # Main content
    parts.append(r'% Main content')
    parts.append(r'\begin{multicols*}{3}')
    parts.append('')
    parts.append(code_body)

    if theoretical_body:
        parts.append(theoretical_body)

    parts.append(r'\end{multicols*}')
    parts.append('')
    parts.append(r'\end{document}')

    return '\n'.join(parts)
