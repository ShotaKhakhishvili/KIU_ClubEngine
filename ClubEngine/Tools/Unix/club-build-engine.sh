#!/usr/bin/env bash
set -e

if [ -z "$CLUBENGINE_ROOT" ]; then
    echo "ERROR: CLUBENGINE_ROOT is not set."
    echo "Run club-init.sh first."
    exit 1
fi

CONFIG="${1:-Debug}"
PLATFORM="Linux64"

CMAKE_FILE="$CLUBENGINE_ROOT/CMakeLists.txt"

ENGINE_VERSION="$(grep -oP 'project\s*\(\s*ClubEngine\s+VERSION\s+\K[0-9]+\.[0-9]+\.[0-9]+' "$CMAKE_FILE" | head -n 1)"

if [ -z "$ENGINE_VERSION" ]; then
    echo "ERROR: Could not read ClubEngine version from CMakeLists.txt."
    exit 1
fi

BUILD_DIR="$CLUBENGINE_ROOT/Build/$PLATFORM/$CONFIG/$ENGINE_VERSION"

echo "Building ClubEngine"
echo "Config:  $CONFIG"
echo "Version: $ENGINE_VERSION"
echo "Build:   $BUILD_DIR"
echo

if command -v ninja >/dev/null 2>&1; then
    cmake -S "$CLUBENGINE_ROOT" -B "$BUILD_DIR" -G Ninja -DCMAKE_BUILD_TYPE="$CONFIG"
else
    cmake -S "$CLUBENGINE_ROOT" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$CONFIG"
fi

cmake --build "$BUILD_DIR"

echo
echo "Engine build complete."
echo "Output:"
echo "$CLUBENGINE_ROOT/Binaries/$PLATFORM/$CONFIG/$ENGINE_VERSION"