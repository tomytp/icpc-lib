# Biblioteca de Algoritmos — PUC‑Rio

Biblioteca de algoritmos, estruturas de dados e primitivas para Maratonas de Programação/ICPC da PUC‑Rio. Os códigos são em C++17, organizados em arquivos autocontidos (snippets) prontos para uso em competições.

## Escopo
- Conteúdo mantido: `src/` (snippets em C++) e `latex/` (gerador de PDF).
- Saídas: PDFs gerados em `pdf/`.

## Estrutura
- `src/`: algoritmos por tópico (`graphs/`, `math/`, `geometry/`, `strings/`, `structures/`, `misc/`, `extra/`).
- `latex/`: gerador (`getlatex.cpp`, `getlatex.sh`, `preamble.tex`).
- `pdf/`: PDFs gerados (ex.: [`pdf/biblioteca.pdf`](./pdf/biblioteca.pdf)).

## Build
- Gerar PDF: `cd latex && ./getlatex.sh`

## Navegação rápida
- Código: [`src/`](./src/)
  - Exemplos: [`src/graphs/dijkstra.cpp`](./src/graphs/dijkstra.cpp), [`src/structures/fenwick_tree.cpp`](./src/structures/fenwick_tree.cpp), [`src/geometry/convex_hull.cpp`](./src/geometry/convex_hull.cpp)
- Artefatos de build: [`pdf/`](./pdf/)
