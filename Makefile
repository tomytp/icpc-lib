# ICPC Library Build System
#
# Targets:
#   full    - Build complete PDF with all code (default)
#   finals  - Build finals PDF (25-page subset from finals.yaml)
#   fast    - Quick single-pass build (no TOC update)
#   clean   - Remove generated files
#

.PHONY: all full finals fast clean clean-generated clean-aux help install-dependencies

# Directories
LATEX_DIR := latex
PDF_DIR := pdf
SRC_DIR := src

# Output files
FULL_PDF := $(PDF_DIR)/lib.pdf
FINALS_PDF := $(PDF_DIR)/lib_finals.pdf

# LaTeX compiler
PDFLATEX := pdflatex -shell-escape -interaction=nonstopmode -halt-on-error

# Default target
all: full

# ============================================================
# Full build - complete library PDF
# ============================================================
full: generate-tex
	@echo "=== Building full PDF ==="
	@mkdir -p $(PDF_DIR)
	cd $(LATEX_DIR) && $(PDFLATEX) icpclib.tex
	cd $(LATEX_DIR) && $(PDFLATEX) icpclib.tex
	mv $(LATEX_DIR)/icpclib.pdf $(FULL_PDF)
	@$(MAKE) clean-aux
	@echo "=== Full PDF created: $(FULL_PDF) ==="

# ============================================================
# Finals build - 25-page subset
# ============================================================
finals: generate-tex
	@echo "=== Building finals PDF ==="
	@mkdir -p $(PDF_DIR)
	cd $(LATEX_DIR) && python3 generate_finals.py
	cd $(LATEX_DIR) && $(PDFLATEX) icpclib-finals.tex
	cd $(LATEX_DIR) && $(PDFLATEX) icpclib-finals.tex
	mv $(LATEX_DIR)/icpclib-finals.pdf $(FINALS_PDF)
	@$(MAKE) clean-aux
	@echo "=== Finals PDF created: $(FINALS_PDF) ==="

# ============================================================
# Fast build - single pass, no TOC update
# ============================================================
fast: generate-tex
	@echo "=== Fast build (single pass) ==="
	@mkdir -p $(PDF_DIR)
	cd $(LATEX_DIR) && $(PDFLATEX) icpclib.tex
	mv $(LATEX_DIR)/icpclib.pdf $(FULL_PDF)
	@$(MAKE) clean-aux
	@echo "=== PDF created: $(FULL_PDF) ==="

# ============================================================
# Generate .tex files from source code
# ============================================================
.PHONY: generate-tex

generate-tex:
	@echo "=== Generating .tex files from source ==="
	cd $(LATEX_DIR) && python3 generate_tex.py

# ============================================================
# Clean targets
# ============================================================
clean-aux:
	@rm -f $(LATEX_DIR)/*.aux $(LATEX_DIR)/*.log $(LATEX_DIR)/*.out
	@rm -f $(LATEX_DIR)/*.toc $(LATEX_DIR)/*.fdb_latexmk $(LATEX_DIR)/*.fls
	@rm -f $(LATEX_DIR)/*.synctex.gz

clean-generated:
	@echo "=== Cleaning generated .tex files and chapters ==="
	@rm -rf $(LATEX_DIR)/generated
	@rm -f $(LATEX_DIR)/chapters/*.tex

clean: clean-aux clean-generated
	@echo "=== Cleaning build artifacts ==="
	@rm -f $(LATEX_DIR)/icpclib.pdf $(LATEX_DIR)/icpclib-finals.pdf
	@rm -f $(LATEX_DIR)/icpclib-finals.tex
	@rm -f $(PDF_DIR)/lib.pdf $(PDF_DIR)/lib_finals.pdf

# ============================================================
# Install dependencies
# ============================================================
install-dependencies:
	@echo "=== Installing dependencies ==="
	sudo apt-get update
	sudo apt-get install -y \
		texlive-latex-base \
		texlive-latex-recommended \
		texlive-latex-extra \
		texlive-fonts-recommended \
		texlive-fonts-extra \
		python3-yaml

# ============================================================
# Help
# ============================================================
help:
	@echo "ICPC Library Build System"
	@echo ""
	@echo "Targets:"
	@echo "  make full                 Build complete PDF (default)"
	@echo "  make finals               Build 25-page finals subset"
	@echo "  make fast                 Quick single-pass build"
	@echo "  make clean                Remove LaTeX build artifacts"
	@echo "  make clean-all            Remove all generated files"
	@echo "  make install-dependencies Install required packages"
	@echo ""
	@echo "Output:"
	@echo "  pdf/lib.pdf               Full library"
	@echo "  pdf/lib_finals.pdf        Finals subset"
