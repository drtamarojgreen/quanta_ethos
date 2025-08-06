#!/bin/bash
#
# This script will orchestrate the build process for the entire ecosystem.
# - Run cmake and make for all C++ components.
# - Create Python virtual environments and install dependencies.
# - Run R dependency installation scripts.

set -e

echo "Building PrismQuanta Ecosystem..."

# Source environment variables
if [ -f prismquanta.env ]; then
  export $(grep -v '^#' prismquanta.env | grep -v '^$' | xargs)
else
  echo "Error: prismquanta.env not found."
  exit 1
fi

# Build C++ components
echo "Building C++ components..."
if [ ! -d "build" ]; then
  mkdir build
fi
cd build
cmake ..
cmake --build .
cd ..

echo "Build complete."
