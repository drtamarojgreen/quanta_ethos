#!/bin/bash
set -e

# Sorrel SDD Checker Installer
# Usage: curl -sSL {repo}/install/sorrel_checker.sh | bash

echo "--- Sorrel SDD Checker Installation ---"

TARGET_REPO=$(pwd)
TEMP_DIR=$(mktemp -d)
BASE_URL="https://raw.githubusercontent.com/drtamarojgreen/quanta_ethos/main"

echo "Downloading necessary source files..."
mkdir -p "$TEMP_DIR/src" "$TEMP_DIR/include/dev" "$TEMP_DIR/data"

curl -sSL "$BASE_URL/include/dev/sdd_engine.h" -o "$TEMP_DIR/include/dev/sdd_engine.h"
curl -sSL "$BASE_URL/src/dev/sdd_engine.cpp" -o "$TEMP_DIR/src/sdd_engine.cpp"
curl -sSL "$BASE_URL/src/dev/sorrel_main.cpp" -o "$TEMP_DIR/src/sorrel_main.cpp"
curl -sSL "$BASE_URL/data/sdd_scoring_rules.xml" -o "$TEMP_DIR/data/sdd_scoring_rules.xml"

cat > "$TEMP_DIR/CMakeLists.txt" <<EOF
cmake_minimum_required(VERSION 3.16)
project(sorrel_bootstrap LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
add_executable(sorrel src/sorrel_main.cpp src/sdd_engine.cpp)
target_include_directories(sorrel PRIVATE include)
EOF

echo "Building Sorrel binary..."
cd "$TEMP_DIR"
mkdir build && cd build
cmake .. > /dev/null
cmake --build . > /dev/null

echo "Installing Sorrel to $TARGET_REPO..."
cp sorrel "$TARGET_REPO/"
mkdir -p "$TARGET_REPO/data"
cp ../data/sdd_scoring_rules.xml "$TARGET_REPO/data/"

cd "$TARGET_REPO"
rm -rf "$TEMP_DIR"

echo "Bootstrapping SDD structure..."
./sorrel init .

echo "Installation complete. Run './sorrel check' to evaluate your project."
