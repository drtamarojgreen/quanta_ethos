#!/bin/bash
set -e

# Sorrel SDD Checker Installer
# Usage: curl -sSL {repo}/install/sorrel_checker.sh | bash

echo "--- Sorrel SDD Checker Installation ---"

if [ ! -f "CMakeLists.txt" ]; then
    echo "Error: Must be run from the root of a project repository."
    exit 1
fi

echo "Building Sorrel binary..."
mkdir -p build_sorrel
cd build_sorrel
cmake .. > /dev/null
cmake --build . --target sorrel > /dev/null

echo "Installing Sorrel..."
cp sorrel ..
cd ..
rm -rf build_sorrel

echo "Bootstrapping SDD structure..."
./sorrel init .

echo "Installation complete. Run './sorrel check' to evaluate your project."
