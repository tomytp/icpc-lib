#!/usr/bin/env python3
"""
ICPC Library Preprocessor - Generates LaTeX code listings from C++ source files.

Based on KACTL's preprocessor.py, adapted for icpc-lib comment format:
  // Name [flags]
  //
  // Description line(s)
  //
  // complexity: O(...) time, O(...) memory

Outputs lstlisting blocks with optional hash prefixes for code integrity.
"""
import sys
import re
import hashlib
import subprocess
from pathlib import Path

HASH_LEN = 3
NO_HASH = "nohash"
NO_PRINT = "noprint"

CUSTOM_TYPES = {
    "ll", "ull", "ld", "i64", "u64",
    "p64", "pii", "pll", "v64", "vll",
    "vi", "vii", "vvi"
}


def is_ident_char(c: str) -> bool:
    return c == '_' or c.isalnum()


def inject_custom_type_markup(s: str) -> str:
    """Add LaTeX markup for custom types."""
    out = []
    i, n = 0, len(s)
    while i < n:
        if is_ident_char(s[i]):
            j = i
            while j < n and is_ident_char(s[j]):
                j += 1
            tok = s[i:j]
            if tok in CUSTOM_TYPES:
                out.append("@\\typ{" + tok + "}@")
            else:
                out.append(tok)
            i = j
        else:
            out.append(s[i])
            i += 1
    return ''.join(out)


def escape_latex(s: str) -> str:
    """Escape special LaTeX characters."""
    s = s.replace('\\', '\\textbackslash{}')
    s = s.replace('^', '\\^{}')
    s = s.replace('_', '\\_')
    s = s.replace('&', '\\&')
    s = s.replace('%', '\\%')
    s = s.replace('$', '\\$')
    s = s.replace('#', '\\#')
    s = s.replace('{', '\\{')
    s = s.replace('}', '\\}')
    s = s.replace('~', '\\textasciitilde{}')
    return s


def escape_title(s: str) -> str:
    """Escape title for LaTeX (less aggressive)."""
    s = s.replace('^', '\\^{}')
    s = s.replace('_', '\\_')
    s = s.replace('&', '\\&')
    s = s.replace('%', '\\%')
    s = s.replace('#', '\\#')
    return s


def split_flags(s: str) -> list:
    """Split comma-separated flags."""
    parts = []
    cur = []
    for ch in s + ',':
        if ch == ',':
            if cur:
                parts.append(''.join(cur).strip())
            cur = []
        else:
            cur.append(ch)
    return parts


def parse_header(path: Path) -> dict:
    """
    Parse the file header to extract metadata.

    Expected format:
        // Name [flags]
        //
        // Description line(s)
        //
        // complexity: O(...) time, O(...) memory

    Returns dict with: name, flags, description, complexity
    """
    result = {
        'name': path.stem,
        'flags': set(),
        'description': '',
        'complexity': ''
    }

    lines = []
    with path.open('r', encoding='utf-8') as f:
        for line in f:
            stripped = line.strip()
            if not stripped.startswith('//'):
                break
            lines.append(stripped[2:].strip() if len(stripped) > 2 else '')

    if not lines:
        return result

    # First line: name and flags
    first = lines[0]
    if '[' in first and first.endswith(']'):
        idx = first.find('[')
        result['name'] = first[:idx].strip()
        flags_str = first[idx+1:-1]
        result['flags'] = set(split_flags(flags_str))
    else:
        result['name'] = first.strip()

    # Parse remaining lines for description and complexity
    desc_lines = []
    for line in lines[1:]:
        if line.lower().startswith('complexity:'):
            result['complexity'] = line[11:].strip()
        elif line:
            desc_lines.append(line)

    result['description'] = ' '.join(desc_lines)

    return result


def preprocess_text(text: str) -> str:
    """Run C preprocessor on text to normalize for hashing."""
    # Remove lines starting with #w (KACTL convention)
    filtered = '\n'.join([ln for ln in text.splitlines() if not ln.startswith('#w')]) + '\n'

    try:
        proc = subprocess.run(
            ["clang", "-E", "-x", "c++", "-dD", "-P", "-"],
            input=filtered.encode('utf-8'),
            stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL,
            check=True,
        )
        out = proc.stdout.decode('utf-8', errors='ignore')
    except Exception:
        try:
            proc = subprocess.run(
                ["cpp", "-dD", "-P", "-fpreprocessed", "-"],
                input=filtered.encode('utf-8'),
                stdout=subprocess.PIPE,
                stderr=subprocess.DEVNULL,
                check=True,
            )
            out = proc.stdout.decode('utf-8', errors='ignore')
        except Exception:
            out = filtered

    return out


def compute_hash(text: str) -> str:
    """Compute MD5 hash of preprocessed code."""
    pre = preprocess_text(text)
    # Remove all whitespace
    compact = re.sub(r"\s+", "", pre)
    h = hashlib.md5(compact.encode('utf-8')).hexdigest()
    return h[:HASH_LEN]


def hash_region(lines: list, l: int = 0, r: int = 10**9) -> str:
    """Compute hash for a region of lines."""
    region = '\n'.join(lines[l:min(r+1, len(lines))]) + '\n'
    return compute_hash(region)


def is_comment_line(line: str) -> bool:
    """Check if line is a comment or blank."""
    t = line.strip()
    if not t:
        return True
    if t.startswith('//'):
        return True
    if t.startswith('/*'):
        return True
    return False


def get_code_lines(path: Path) -> list:
    """Extract code lines (skipping header comments)."""
    lines = []
    in_header = True

    with path.open('r', encoding='utf-8') as f:
        for line in f:
            stripped = line.rstrip('\n')
            if in_header:
                # Skip initial comment block
                if stripped.strip().startswith('//') or not stripped.strip():
                    continue
                in_header = False
            lines.append(stripped)

    return lines


def generate_listing(path: Path, custom_title: str = None,
                     show_hash: bool = True, show_desc: bool = True) -> str:
    """
    Generate LaTeX listing for a source file.

    Args:
        path: Path to source file
        custom_title: Override the title from file header
        show_hash: Include per-line hashes
        show_desc: Include description and complexity

    Returns:
        LaTeX code as string
    """
    header = parse_header(path)

    # Check for noprint flag
    if NO_PRINT in header['flags']:
        return ""

    # Determine if we should show hashes
    if NO_HASH in header['flags']:
        show_hash = False

    output = []

    # Title
    title = custom_title if custom_title else header['name']
    title = escape_title(title)

    # File hash for integrity
    code_lines = get_code_lines(path)
    file_hash = compute_hash('\n'.join(code_lines))

    output.append(f"\\codesection{{{title}}}{{{file_hash}}}")

    # Description
    if show_desc and header['description']:
        desc = escape_latex(header['description'])
        output.append(f"\\codedesc{{{desc}}}")

    # Complexity
    if show_desc and header['complexity']:
        complexity = escape_latex(header['complexity'])
        output.append(f"\\complexity{{{complexity}}}")

    # Code listing
    output.append("\\begin{lstlisting}")

    # Track brace depth for hash computation
    depth = 0
    st = []  # Stack of line indices where braces opened
    started_code = False

    for line_idx, raw in enumerate(code_lines):
        start_line = line_idx

        # Track braces
        for c in raw:
            if c == '{':
                depth += 1
                st.append(line_idx)
            elif c == '}':
                depth -= 1
                if st:
                    start_line = st[-1]
                    st.pop()

        comment = is_comment_line(raw)
        if not comment:
            started_code = True

        # Add hash prefix if enabled
        if show_hash and started_code:
            if comment:
                if depth != 0:
                    output.append(f"@\\hashprefix{{{' ' * (HASH_LEN + 1)}}}@{raw}")
                else:
                    output.append(raw)
            else:
                h = hash_region(code_lines, start_line, line_idx)
                prefix = f"@\\hashprefix{{{h} }}@"
                # Apply custom type markup
                marked = inject_custom_type_markup(raw)
                output.append(f"{prefix}{marked}")
        else:
            if not comment:
                marked = inject_custom_type_markup(raw)
                output.append(marked)
            else:
                output.append(raw)

    output.append("\\end{lstlisting}")
    output.append("")

    return '\n'.join(output)


def main():
    """
    Main entry point.

    Usage: preprocessor.py <file_path> [options]

    Options are passed as a single string:
        -c "Custom Title"  - Override title
        --no-hash          - Disable per-line hashes
        --no-desc          - Disable description output
    """
    if len(sys.argv) < 2:
        print("Usage: preprocessor.py <file_path> [options]", file=sys.stderr)
        sys.exit(1)

    file_path = Path(sys.argv[1])
    options = sys.argv[2] if len(sys.argv) > 2 else ""

    # Parse options
    custom_title = None
    show_hash = True
    show_desc = True

    if '-c' in options or '-c ' in options:
        # Extract custom title
        match = re.search(r'-c\s*"([^"]*)"', options)
        if match:
            custom_title = match.group(1)
        else:
            match = re.search(r"-c\s*'([^']*)'", options)
            if match:
                custom_title = match.group(1)

    if '--no-hash' in options:
        show_hash = False

    if '--no-desc' in options:
        show_desc = False

    if not file_path.exists():
        print(f"% Error: File not found: {file_path}", file=sys.stderr)
        sys.exit(1)

    latex = generate_listing(file_path, custom_title, show_hash, show_desc)
    print(latex)


if __name__ == '__main__':
    main()
