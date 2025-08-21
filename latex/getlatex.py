#!/usr/bin/env python3
import os
import re
import sys
import hashlib
import subprocess
from pathlib import Path

HASH_LEN = 3
LOOK_FOR_HASH_IN_FILE = False
NO_HASH = "nohash"
NO_PRINT = "noprint"
SRC_PATH = Path("../src")

CUSTOM_TYPES = {
    "ll", "ull", "ld", "i64", "u64",
    "p64", "pii", "pll", "v64", "vll"
}

def is_ident_char(c: str) -> bool:
    return c == '_' or c.isalnum()

def inject_custom_type_markup(s: str) -> str:
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

def read_first_line(path: Path) -> str:
    with path.open('r', encoding='utf-8') as f:
        return f.readline().rstrip('\n')

def split_flags(s: str) -> list[str]:
    parts = []
    cur = []
    # emulate simple split by ',' ignoring empties
    for ch in s + ',':
        if ch == ',':
            if cur:
                parts.append(''.join(cur))
            cur = []
        else:
            cur.append(ch)
    return parts

def get_flags(path: Path) -> set[str]:
    line = read_first_line(path)
    # find first '['; require ']' at end
    idx = line.find('[')
    if idx == -1 or not line.endswith(']'):
        return set()
    inner = line[idx+1:-1]
    return set(split_flags(inner))

def remove_flags(line: str) -> str:
    has_l = False
    has_r = False
    for c in line:
        if not has_l and c == '[':
            has_l = True
        elif has_l and c == ']':
            has_r = True
    if not (has_l and has_r):
        return line
    idx = line.find('[')
    trimmed = line[:idx]
    while trimmed.endswith(' '):
        trimmed = trimmed[:-1]
    return trimmed

def get_name(path: Path) -> str:
    line = read_first_line(path)
    line = remove_flags(line)
    if len(line) >= 3 and line[2] == ' ':
        return line[3:]
    return line[2:]

def is_comment(line: str) -> bool:
    t = line.lstrip(' \t')
    if t == '':
        return True
    if t.startswith('//'):
        return True
    if t.startswith('/*'):
        return True
    return False

def preprocess_text(text: str) -> str:
    # remove lines that start with #w
    filtered = '\n'.join([ln for ln in text.splitlines() if not ln.startswith('#w')]) + '\n'
    # prefer clang, else cpp
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
        # fallback to cpp
        proc = subprocess.run(
            ["cpp", "-dD", "-P", "-fpreprocessed", "-"],
            input=filtered.encode('utf-8'),
            stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL,
            check=True,
        )
        out = proc.stdout.decode('utf-8', errors='ignore')
    return out

def hash_region(path: Path, l: int = 0, r: int = 10**9) -> str:
    # read lines l..r inclusive
    lines = []
    with path.open('r', encoding='utf-8') as f:
        for i, ln in enumerate(f):
            if i > r:
                break
            if i >= l:
                lines.append(ln.rstrip('\n'))
    pre = preprocess_text('\n'.join(lines) + '\n')
    # delete all whitespace characters
    compact = re.sub(r"\s+", "", pre)
    h = hashlib.md5(compact.encode('utf-8')).hexdigest()
    return h[:HASH_LEN]

def printa_cuidado(s: str) -> str:
    # escape ^ by prepending \\ and appending {}
    out = []
    for c in s:
        if c == '^':
            out.append('\\')
            out.append('^')
            out.append('{}')
        else:
            out.append(c)
    return ''.join(out)

def printa_arquivo(path: Path, extra: bool = False):
    count = 0
    with path.open('r', encoding='utf-8') as f:
        for line in f:
            if count < 2 and not extra:
                count += 1
                continue
            count += 1
            sys.stdout.write(line)

def printa_arquivo_codigo(path: Path, extra: bool = False):
    print("\\begin{minted}{cpp}")
    count = 0
    started_code = False
    depth = 0
    st: list[int] = []
    with path.open('r', encoding='utf-8') as f:
        for line_idx, raw in enumerate(f):
            start_line = line_idx
            if count < 2 and not extra:
                count += 1
                continue
            count += 1

            for c in raw:
                if c == '{':
                    depth += 1
                    st.append(line_idx)
                elif c == '}':
                    depth -= 1
                    if st:
                        start_line = st[-1]
                        st.pop()

            comment = is_comment(raw.rstrip('\n'))
            if not comment:
                started_code = True

            if not extra and started_code:
                if comment:
                    if depth != 0:
                        sys.stdout.write("@\\hashprefix{")
                        sys.stdout.write(" " * (HASH_LEN + 1))
                        sys.stdout.write("}@")
                else:
                    h = hash_region(path, start_line, line_idx)
                    sys.stdout.write(f"@\\hashprefix{{{h} }}@")

            printed = raw.rstrip('\n')
            if not comment:
                printed = inject_custom_type_markup(printed)
            sys.stdout.write(printed + "\n")

    print("\\end{minted}\n")

def printa_listing(sub: str, path: Path, extra: bool = False) -> bool:
    flags = get_flags(path)
    if NO_PRINT in flags and not os.environ.get('PRINT_ALL'):
        return False

    if LOOK_FOR_HASH_IN_FILE and not extra and NO_HASH not in flags:
        h = hash_region(path)
        if h not in path.read_text(encoding='utf-8', errors='ignore'):
            print(f"\033[0;31mWARNING\033[0m: hash nao encontrado para: {str(path)[10:]}", file=sys.stderr)

    print("\\subsection{", end='')
    if not extra:
        print(printa_cuidado(get_name(path)), end='')
    else:
        print(printa_cuidado(sub), end='')
    print("}")

    printa_arquivo_codigo(path, extra)
    return True

def lower_ascii(s: str) -> str:
    return s.lower()

def dfs_collect(files: list[tuple[str, Path]], base: Path, extra: bool = False):
    if not base.exists():
        return
    for name in sorted(os.listdir(base)):
        if name.startswith('.'):
            continue
        p = base / name
        if p.is_dir():
            dfs_collect(files, p, extra)
        else:
            if not extra:
                files.append((name, p))
            else:
                # Special-case vimrc: pass extra False to print hashes for it
                printa_listing(name, p, extra and name != "vimrc")

def printa_section(s: str):
    print("\n\n")
    print("%" * 20)
    print("%\n% " + s + "\n%")
    print("%" * 20)
    print("\n")
    print(f"\\section{{{s}}}\n")

def main():
    print_all = (len(sys.argv) > 1 and sys.argv[1] == "--printall")
    if print_all:
        os.environ['PRINT_ALL'] = '1'
        print("Printing all files...\n", file=sys.stderr)

    # Preamble
    printa_arquivo(Path("preamble.tex"), True)

    # Top-level sections for all subdirs except "extra"
    for entry in sorted(os.listdir(SRC_PATH)):
        if entry.startswith('.'):
            continue
        if entry == 'extra':
            continue
        d = SRC_PATH / entry
        if not d.is_dir():
            continue
        printa_section(entry)

        files: list[tuple[str, Path]] = []
        dfs_collect(files, d)
        files.sort(key=lambda t: lower_ascii(get_name(t[1])))

        print(f"=== {entry} ===", file=sys.stderr)
        for fname, fpath in files:
            printed = printa_listing(fname[:-4], fpath)
            if printed:
                print(get_name(fpath), file=sys.stderr)
        print("", file=sys.stderr)

    print("\\pagebreak")
    printa_section("extra")
    files: list[tuple[str, Path]] = []
    dfs_collect(files, SRC_PATH / "extra", True)

    print("\\end{document}")

if __name__ == '__main__':
    main()

