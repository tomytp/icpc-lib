# Repository Guidelines

## Project Structure & Module Organization
- `src/`: C++17 algorithms by topic (`extra/`, `graphs/`, `math/`, `geometry/`, `strings/`, `structures/`, `misc/`). Files use lower_snake_case, e.g., `sparse_table.cpp`.
- `latex/`: PDF generator (`getlatex.cpp`, `getlatex.sh`, `preamble.tex`). Output is placed in `pdf/`.
- `pdf/`: Generated PDFs.

## Build, Test, and Development Commands
- Build PDF library: `cd latex && ./getlatex.sh` (requires TeX and Rubber; see below).
- Compile a snippet: `g++ -std=c++17 -O2 -Wall -Wshadow src/graphs/dijkstra.cpp -o bin/dijkstra`
  - Use `src/extra/makefile` flags for sanitizers during local checks.

## Coding Style & Naming Conventions
- Language: C++17.
- Indentation: 4 spaces; no tabs.
- Filenames: lower_snake_case (`dinitz.cpp`, `convex_hull.cpp`).
- Headers: prefer single-file snippets with `#include <bits/stdc++.h>` for contests.
- Macros/constants uppercase; variables and functions lower_snake_case.
- Keep top-of-file comments concise describing purpose/complexity.

## Testing Guidelines
- Ensure each snippet compiles standalone with `-std=c++17` and typical flags (`-O2 -Wall -Wshadow`).
- Provide a minimal `main()` when relevant; otherwise ensure the snippet is usable as-is.

## Commit & Pull Request Guidelines
- Commits: short, imperative summary (e.g., `graphs: add dijkstra`, `structures: fix fenwick bug`).
- Include scope (`graphs/`, `math/`, etc.) when helpful; reference issues if applicable.
- PRs: clear description, why the change, links to references/problems, and, if relevant, before/after notes for PDF output.

## Security & Configuration Tips
- PDF build requires TeX Live and Rubber: `sudo apt-get install texlive-full rubber`.
- No secrets required. Keep external links in docs explicit and verified.
