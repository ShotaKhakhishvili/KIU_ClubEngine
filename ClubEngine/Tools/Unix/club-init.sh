#!/usr/bin/env bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ENGINE_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
TOOLS_DIR="$ENGINE_ROOT/Tools/Unix"

mkdir -p "$ENGINE_ROOT/Saved"

if [ ! -f "$ENGINE_ROOT/Saved/ProjectRegistry.json" ]; then
cat > "$ENGINE_ROOT/Saved/ProjectRegistry.json" <<EOF
{
    "projects": []
}
EOF
fi

RC_FILE="$HOME/.bashrc"
if [ -n "$ZSH_VERSION" ]; then
    RC_FILE="$HOME/.zshrc"
fi

START="# >>> ClubEngine init >>>"
END="# <<< ClubEngine init <<<"

if [ -f "$RC_FILE" ]; then
    sed -i.bak "/$START/,/$END/d" "$RC_FILE"
fi

cat >> "$RC_FILE" <<EOF

$START
export CLUBENGINE_ROOT="$ENGINE_ROOT"
export PATH="$TOOLS_DIR:\$PATH"
$END
EOF

echo "ClubEngine initialized."
echo "Root: $ENGINE_ROOT"
echo "Tools: $TOOLS_DIR"
echo "Run: source $RC_FILE"
