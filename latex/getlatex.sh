#!/bin/bash

set -euo pipefail

python3 getlatex.py "${1:-}" > biblioteca.tex

rubber --unsafe -d biblioteca
mv biblioteca.pdf ../pdf
rm -f biblioteca.aux biblioteca.toc biblioteca.out biblioteca.log biblioteca.rubbercache
rm -rf _minted-biblioteca
