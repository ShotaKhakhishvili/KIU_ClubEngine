#!/usr/bin/env bash
set -e

if [ -z "$CLUBENGINE_ROOT" ]; then
    echo "ERROR: CLUBENGINE_ROOT is not set."
    echo "Run club-init.sh first."
    exit 1
fi

if [ -z "$1" ]; then
    echo "Usage: club-new-project.sh <ProjectPath> <EngineAssociation>"
    echo "Example: club-new-project.sh Game1 0.1"
    exit 1
fi

if [ -z "$2" ]; then
    echo "ERROR: Missing EngineAssociation."
    echo "Example: club-new-project.sh Game1 0.1"
    exit 1
fi

PROJECT_ROOT="$(realpath -m "$1")"
PROJECT_NAME="$(basename "$PROJECT_ROOT")"
ENGINE_ASSOCIATION="$2"

TEMPLATE_ROOT="$CLUBENGINE_ROOT/Templates/Project"

if [ ! -d "$TEMPLATE_ROOT" ]; then
    echo "ERROR: Project template not found:"
    echo "$TEMPLATE_ROOT"
    exit 1
fi

if [ -e "$PROJECT_ROOT" ]; then
    echo "ERROR: Project folder already exists:"
    echo "$PROJECT_ROOT"
    exit 1
fi

PLATFORM="Linux64"
CONFIG="Debug"
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

echo "Creating project '$PROJECT_NAME'"
echo "Path:              $PROJECT_ROOT"
echo "EngineAssociation: $ENGINE_ASSOCIATION"
echo "Template:          $TEMPLATE_ROOT"
echo "Found engine:      $FOUND_ENGINE"
echo

cp -R "$TEMPLATE_ROOT" "$PROJECT_ROOT"

find "$PROJECT_ROOT" -depth -name "*[[]PROJ_NAME[]]*" | while read -r path; do
    new_path="${path//\[PROJ_NAME\]/$PROJECT_NAME}"
    mv "$path" "$new_path"
done

find "$PROJECT_ROOT" -type f | while read -r file; do
    sed -i \
        -e "s/\[PROJ_NAME\]/$PROJECT_NAME/g" \
        -e "s/\[ENGINE_ASSOCIATION\]/$ENGINE_ASSOCIATION/g" \
        "$file"
done

(
    cd "$PROJECT_ROOT"
    "$CLUBENGINE_ROOT/Tools/Unix/club-register-project.sh"
)

echo
echo "Project created and registered."
echo "To build:"
echo "  cd \"$PROJECT_ROOT\""
echo "  club-build-project.sh Debug"