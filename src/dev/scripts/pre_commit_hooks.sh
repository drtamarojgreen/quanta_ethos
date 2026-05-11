#!/bin/bash
echo "Running pre-commit hooks..."
./src/dev/scripts/static_analysis.sh
./src/dev/scripts/coverage.sh
