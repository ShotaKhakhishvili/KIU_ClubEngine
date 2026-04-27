#!/usr/bin/env bash
set -e

RC_FILE="$HOME/.bashrc"
if [ -n "$ZSH_VERSION" ]; then
    RC_FILE="$HOME/.zshrc"
fi

START="# >>> ClubEngine init >>>"
END="# <<< ClubEngine init <<<"

if [ -f "$RC_FILE" ]; then
    sed -i.bak "/$START/,/$END/d" "$RC_FILE"
    echo "Cleaned $RC_FILE"
else
    echo "No rc file found."
fi

echo "Restart terminal or run: source $RC_FILE"
