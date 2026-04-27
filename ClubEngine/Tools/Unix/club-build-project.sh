#!/usr/bin/env bash
set -e

if [ -z "$CLUBENGINE_ROOT" ]; then
    echo "ERROR: CLUBENGINE_ROOT is not set."
    echo "Run club-init.sh first."
    exit 1
fi

CONFIG="${1:-Debug}"
PROJECT_ROOT="$(pwd)"
PROJECT_NAME="$(basename "$PROJECT_ROOT")"
PLATFORM="Linux64"

if [ ! -f "$PROJECT_ROOT/CMakeLists.txt" ]; then
    echo "ERROR: No CMakeLists.txt found. Run this from project root."
    exit 1
fi

CLUBPROJECT_FILE="$(find "$PROJECT_ROOT" -maxdepth 1 -name "*.clubproject" | head -n 1)"

if [ -z "$CLUBPROJECT_FILE" ]; then
    echo "ERROR: No .clubproject file found in project root."
    exit 1
fi

ENGINE_ASSOCIATION="$(grep -oP '"EngineAssociation"\s*:\s*"\K[^"]+' "$CLUBPROJECT_FILE" | head -n 1)"

if [ -z "$ENGINE_ASSOCIATION" ]; then
    echo "ERROR: EngineAssociation is empty or missing."
    exit 1
fi

ENGINE_BIN_ROOT="$CLUBENGINE_ROOT/Binaries/$PLATFORM/$CONFIG"

if [ ! -d "$ENGINE_BIN_ROOT" ]; then
    echo "ERROR: No built engines found for $PLATFORM/$CONFIG."
    echo "Run: club-build-engine.sh $CONFIG"
    exit 1
fi

FOUND_ENGINE="$(find "$ENGINE_BIN_ROOT" -maxdepth 1 -type d -name "$ENGINE_ASSOCIATION.*" | sort -V | tail -n 1)"

if [ -z "$FOUND_ENGINE" ]; then
    echo "ERROR: No built engine matching EngineAssociation '$ENGINE_ASSOCIATION' found."
    echo "Expected something like:"
    echo "  $ENGINE_BIN_ROOT/$ENGINE_ASSOCIATION.x"
    exit 1
fi

ENGINE_VERSION="$(basename "$FOUND_ENGINE")"
BUILD_DIR="$PROJECT_ROOT/Build/$PLATFORM/$CONFIG/$ENGINE_VERSION"

echo "Building project '$PROJECT_NAME'"
echo "Config:            $CONFIG"
echo "EngineAssociation: $ENGINE_ASSOCIATION"
echo "EngineVersion:     $ENGINE_VERSION"
echo "Build:             $BUILD_DIR"
echo

if command -v ninja >/dev/null 2>&1; then
    cmake -S "$PROJECT_ROOT" -B "$BUILD_DIR" -G Ninja -DCMAKE_BUILD_TYPE="$CONFIG"
else
    cmake -S "$PROJECT_ROOT" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$CONFIG"
fi

cmake --build "$BUILD_DIR" --target "$PROJECT_NAME"

echo
echo "Project build complete."
echo "Output:"
echo "$PROJECT_ROOT/Binaries/$PLATFORM/$CONFIG/$ENGINE_VERSION"