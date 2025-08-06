#!/bin/bash
#
# This script will run unit and integration tests for each module.
# It will use mock components where necessary to ensure isolated and
# repeatable testing.

set -e

echo "Running PrismQuanta Test Suite..."

# Source environment variables
if [ -f prismquanta.env ]; then
  export $(grep -v '^#' prismquanta.env | grep -v '^$' | xargs)
else
  echo "Error: prismquanta.env not found."
  exit 1
fi

# Run C++ tests
echo "Running C++ tests..."
cd build
ctest
cd ..

echo "All tests passed."
