# ICPC Library Build System
#
# Targets:
#   full    - Build complete PDF with all code (default)
#   finals  - Build finals PDF (25-page subset from finals.yaml)
#   clean   - Remove generated files
#

.PHONY: all full finals clean clean-generated clean-aux help install-dependencies install-hooks validate-finals

# Directories
LATEX_DIR := latex
LATEX_BUILD_DIR := $(LATEX_DIR)/build
PDF_DIR := pdf
SRC_DIR := src

# Output files
FULL_PDF := $(PDF_DIR)/lib.pdf
FINALS_PDF := $(PDF_DIR)/lib_finals.pdf

# LaTeX compiler
PDFLATEX := pdflatex -output-directory=build -shell-escape -interaction=nonstopmode -halt-on-error

# Default target
all: full

# ============================================================
# Full build - complete library PDF
# ============================================================
full: generate-tex
	@echo "=== Building full PDF ==="
	@mkdir -p $(PDF_DIR)
	@mkdir -p $(LATEX_BUILD_DIR)
	@cd $(LATEX_DIR) && python3 generate_full.py
	@cd $(LATEX_DIR) && $(PDFLATEX) icpclib-full.tex > /dev/null
	@cd $(LATEX_DIR) && $(PDFLATEX) icpclib-full.tex > /dev/null
	@mv $(LATEX_BUILD_DIR)/icpclib-full.pdf $(FULL_PDF)
	@$(MAKE) --no-print-directory clean-aux
	@echo "=== Full PDF created: $(FULL_PDF) ==="

# ============================================================
# Finals build - 25-page subset
# ============================================================
finals: generate-tex
	@echo "=== Building finals PDF ==="
	@mkdir -p $(PDF_DIR)
	@mkdir -p $(LATEX_BUILD_DIR)
	@cd $(LATEX_DIR) && python3 generate_finals.py
	@cd $(LATEX_DIR) && $(PDFLATEX) icpclib-finals.tex > /dev/null
	@cd $(LATEX_DIR) && $(PDFLATEX) icpclib-finals.tex > /dev/null
	@mv $(LATEX_BUILD_DIR)/icpclib-finals.pdf $(FINALS_PDF)
	@$(MAKE) --no-print-directory clean-aux
	@echo "=== Finals PDF created: $(FINALS_PDF) ==="

# ============================================================
# Generate .tex files from source code
# ============================================================
.PHONY: generate-tex

generate-tex:
	@echo "=== Generating .tex files from source ==="
	@cd $(LATEX_DIR) && python3 generate_tex.py

# ============================================================
# Clean targets
# ============================================================
clean-aux:
	@rm -rf $(LATEX_BUILD_DIR)

clean-generated:
	@rm -rf $(LATEX_DIR)/generated

clean: clean-aux clean-generated
	@echo "=== Cleaning build artifacts ==="
	@rm -f $(LATEX_DIR)/icpclib-finals.pdf $(LATEX_DIR)/icpclib-full.pdf
	@rm -f $(LATEX_DIR)/icpclib-finals.tex $(LATEX_DIR)/icpclib-full.tex

# ============================================================
# Install dependencies
# ============================================================
validate-finals:
	@python3 $(LATEX_DIR)/validate_finals.py

install-hooks:
	@echo "=== Installing git hooks ==="
	git config core.hooksPath .githooks
	@echo "=== Git hooks installed (using .githooks/) ==="

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
# Tests
# ============================================================
.PHONY: test

test:
	python3 tests/run.py

# Run a specific test: make test-structures/dsu
test-%:
	python3 tests/run.py "$*"

# ============================================================
# Help
# ============================================================
help:
	@echo "ICPC Library Build System"
	@echo ""
	@echo "Targets:"
	@echo "  make full                 Build complete PDF (default)"
	@echo "  make finals               Build 25-page finals subset"
	@echo "  make clean                Remove LaTeX build artifacts"
	@echo "  make clean-all            Remove all generated files"
	@echo "  make validate-finals      Check finals.yaml references exist"
	@echo "  make install-hooks        Configure git to use .githooks/ (local)"
	@echo "  make install-dependencies Install required packages"
	@echo "  make test                 Run all stress tests"
	@echo "  make test-structures/dsu  Run a specific test"
	@echo ""
	@echo "Output:"
	@echo "  pdf/lib.pdf               Full library"
	@echo "  pdf/lib_finals.pdf        Finals subset"
