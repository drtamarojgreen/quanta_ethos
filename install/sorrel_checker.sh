#!/bin/bash
set -e

# Sorrel SDD Checker Installer
# Usage: curl -sSL {repo}/install/sorrel_checker.sh | bash

echo "--- Sorrel SDD Checker Installation ---"

TARGET_REPO=$(pwd)
TEMP_DIR=$(mktemp -d)
PRISM_QUANTA_URL="https://github.com/PrecisionPower/PrismQuanta.git"

echo "Cloning Sorrel source from $PRISM_QUANTA_URL..."
git clone --depth 1 "$PRISM_QUANTA_URL" "$TEMP_DIR" > /dev/null

echo "Building Sorrel binary..."
cd "$TEMP_DIR"
mkdir build
cd build
cmake .. > /dev/null
cmake --build . --target sorrel > /dev/null

echo "Installing Sorrel to $TARGET_REPO..."
cp sorrel "$TARGET_REPO/"
mkdir -p "$TARGET_REPO/data"
cp ../data/sdd_scoring_rules.xml "$TARGET_REPO/data/"

cd "$TARGET_REPO"
rm -rf "$TEMP_DIR"

echo "Bootstrapping SDD structure..."
./sorrel init .

echo "Installation complete. Run './sorrel check' to evaluate your project."
