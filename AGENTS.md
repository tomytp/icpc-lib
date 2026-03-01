# Repository Guidelines

## Project Structure & Module Organization
- `src/`: C++20 algorithms by topic (`extra/`, `graphs/`, `math/`, `geometry/`, `strings/`, `structures/`, `misc/`). Files use lower_snake_case, e.g., `sparse_table.cpp`.
- `latex/`: LaTeX scripts and templates for PDF generation.
- `pdf/`: Generated PDFs (`lib.pdf` and `lib_finals.pdf`).
- `tests/`: Stress testing framework and individual component tests.

## Build, Test, and Development Commands
- Build PDF library: `make full` (default) or `make finals` (25-page subset).
- Clean artifacts: `make clean`.
- Run stress tests: `make test` or `make test-<topic>/<algorithm>` (e.g., `make test-structures/dsu`).
- Compile a snippet: `g++ -std=c++20 -O2 -Wall -Wshadow src/graphs/dijkstra.cpp -o bin/dijkstra`
  - Use sanitizer flags (`-fsanitize=address,undefined`) during local checks.

## Coding Style & Naming Conventions
- Language: C++20.
- Indentation: 4 spaces; no tabs.
- Filenames: lower_snake_case (`dinitz.cpp`, `convex_hull.cpp`).

## Testing Guidelines
- Use the testing framework (`make test`). Each algorithm should have a stress test when possible.
- Ensure each snippet compiles standalone with `-std=c++20` and typical flags (`-O2 -Wall -Wshadow`).

## Commit & Pull Request Guidelines
- Commits: short, imperative summary (e.g., `graphs: add dijkstra`, `structures: fix fenwick bug`).
- Include scope (`graphs/`, `math/`, etc.) when helpful; reference issues if applicable.
- PRs: clear description, why the change, links to references/problems, and, if relevant, before/after notes for PDF output.

## Security & Configuration Tips
- Setup dependencies: `make install-dependencies`.
- Hook configuration: `make install-hooks`.
- No secrets required. Keep external links in docs explicit and verified.
