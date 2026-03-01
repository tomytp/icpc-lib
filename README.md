# Biblioteca de Algoritmos — PUC‑Rio

Biblioteca de algoritmos, estruturas de dados e primitivas para Maratonas de Programação/ICPC da PUC‑Rio. Os códigos são em C++20, organizados em arquivos autocontidos (snippets) prontos para uso em competições.

## Escopo
- Conteúdo mantido: `src/` (snippets em C++) e `latex/` (gerador de PDF).
- Saídas: PDFs gerados em `pdf/`.

## Estrutura
- `src/`: algoritmos por tópico (`graphs/`, `math/`, `geometry/`, `strings/`, `structures/`, `misc/`, `extra/`).
- `latex/`: scripts de geração e templates LaTeX (`generate_full.py`, etc.).
- `pdf/`: PDFs gerados (ex.: [`pdf/lib.pdf`](./pdf/lib.pdf), [`pdf/lib_finals.pdf`](./pdf/lib_finals.pdf)).
- `tests/`: framework de stress testing.

## Build e Testes
- Gerar PDF completo: `make full` (ou apenas `make`)
- Gerar PDF para a final (25 páginas): `make finals`
- Limpar arquivos gerados: `make clean`
- Rodar todos os testes: `make test`
- Rodar teste específico (exemplo): `make test-structures/dsu`

## Navegação rápida
- Código: [`src/`](./src/)
  - Exemplos: [`src/graphs/dijkstra.cpp`](./src/graphs/dijkstra.cpp), [`src/structures/fenwick_tree.cpp`](./src/structures/fenwick_tree.cpp), [`src/geometry/convex_hull.cpp`](./src/geometry/convex_hull.cpp)
- Artefatos de build: [`pdf/`](./pdf/)

