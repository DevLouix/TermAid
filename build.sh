#!/bin/bash
# TermAid Build Script
# Cross-platform build for Linux, macOS, Windows

set -e

echo "=========================================="
echo "  TermAid C++ Build Script"
echo "=========================================="
echo ""

# Colors
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Check for CMake
if ! command -v cmake &> /dev/null; then
    echo -e "${RED}✗ CMake not found${NC}"
    echo "Please install CMake: https://cmake.org/download/"
    exit 1
fi

echo -e "${GREEN}✓ CMake found: $(cmake --version | head -1)${NC}"

# Check for C++ compiler
if command -v g++ &> /dev/null; then
    echo -e "${GREEN}✓ G++ found: $(g++ --version | head -1)${NC}"
elif command -v clang++ &> /dev/null; then
    echo -e "${GREEN}✓ Clang++ found: $(clang++ --version | head -1)${NC}"
else
    echo -e "${RED}✗ No C++ compiler found${NC}"
    echo "Please install g++ or clang++"
    exit 1
fi

echo ""
echo -e "${BLUE}Building TermAid C++ version...${NC}"
echo ""

# Create build directory
mkdir -p build
cd build

# Configure
echo "Configuring..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
echo ""
echo "Building..."
cmake --build . --config Release

# Check if build was successful
if [ -f "termaid" ] || [ -f "termaid.exe" ]; then
    echo ""
    echo -e "${GREEN}=========================================="
    echo "  Build Successful!"
    echo "==========================================${NC}"
    echo ""
    echo "Executable location: build/termaid"
    echo ""
    echo "To install system-wide:"
    echo "  sudo cmake --install ."
    echo ""
    echo "To test:"
    echo "  ./termaid list"
    echo "  ./termaid search docker"
    echo ""
else
    echo -e "${RED}Build failed!${NC}"
    exit 1
fi
