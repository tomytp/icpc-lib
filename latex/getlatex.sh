#!/bin/bash
#
# ICPC Library PDF Builder
#
# Usage: ./getlatex.sh [MODE]
#
# Modes:
#   full    - Build complete PDF (default)
#   finals  - Build finals PDF (25-page subset)
#   fast    - Quick single-pass build
#

set -euo pipefail

MODE="${1:-full}"

case "$MODE" in
    full|finals|fast)
        make -C .. "$MODE"
        ;;
    *)
        echo "Unknown mode: $MODE"
        echo "Valid modes: full, finals, fast"
        exit 1
        ;;
esac
