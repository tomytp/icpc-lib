# Repository Guidelines

## Project Structure & Module Organization
- `Codigo/`: C++17 algorithms by topic (`basic/`, `graphs/`, `math/`, `geometry/`, `strings/`, `structures/`, `misc/`, `extra/`). Files use lower_snake_case, e.g., `sparse_table.cpp`.
- `content/`, `assets/`, `static/`, `data/`: Hugo site content and assets.
- `docs/`: Scripts to generate docs from `Codigo/` into `content/docs/` (`gendocs.cpp`, `gendocs.sh`).
- `latex/`: PDF generator (`getlatex.cpp`, `getlatex.sh`). Output is placed in `pdf/`.
- `hugo.toml`: Site configuration. Go modules (`go.mod`, `go.sum`) support Hugo modules.

## Build, Test, and Development Commands
- Serve docs locally: `hugo server -D` (watches `content/`, `assets/`, `Codigo/`-generated pages).
- Build site: `hugo --minify` (outputs to `public/`).
- Generate/refresh docs from code:
  - Update existing pages: `cd docs && ./gendocs.sh --update`
  - Recreate all pages: `cd docs && ./gendocs.sh --overwrite`
- Build PDF library: `cd latex && ./getlatex.sh` (requires TeX and Rubber; see below).
- Compile a snippet: `g++ -std=c++17 -O2 -Wall -Wshadow Codigo/graphs/dijkstra.cpp -o bin/dijkstra`
  - Use `Codigo/basic/makefile` flags for sanitizers during local checks.

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
- After changes, run `docs/gendocs.sh --update` and check rendering via `hugo server`.

## Commit & Pull Request Guidelines
- Commits: short, imperative summary (e.g., `graphs: add dijkstra`, `structures: fix fenwick bug`).
- Include scope (`graphs/`, `math/`, etc.) when helpful; reference issues if applicable.
- PRs: clear description, why the change, links to references/problems, and before/after notes. Attach screenshots of rendered docs if UI-affecting.

## Security & Configuration Tips
- PDF build requires TeX Live and Rubber: `sudo apt-get install texlive-full rubber`.
- No secrets required. Keep external links in docs explicit and verified.
