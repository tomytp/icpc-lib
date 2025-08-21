#!/bin/bash

g++ -std=c++17 -o getlatex getlatex.cpp -O2
./getlatex $1 > biblioteca.tex

# Make local custom Pygments style importable
export PYTHONPATH="$(pwd)/pygments:${PYTHONPATH}"
rubber --unsafe -d biblioteca
mv biblioteca.pdf ../pdf
rm tmp.cpp
rm getlatex biblioteca.aux biblioteca.toc biblioteca.out
rm -f biblioteca.rubbercache
