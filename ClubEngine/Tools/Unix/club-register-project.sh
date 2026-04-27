#!/usr/bin/env bash
set -e

if [ -z "$CLUBENGINE_ROOT" ]; then
    echo "ERROR: CLUBENGINE_ROOT is not set. Run club-init.sh first."
    exit 1
fi

PROJECT_ROOT="$(pwd)"
PROJECT_NAME="$(basename "$PROJECT_ROOT")"
REGISTRY="$CLUBENGINE_ROOT/Saved/ProjectRegistry.json"

mkdir -p "$CLUBENGINE_ROOT/Saved"

python3 - "$REGISTRY" "$PROJECT_NAME" "$PROJECT_ROOT" <<'PY'
import json, sys
from pathlib import Path

registry = Path(sys.argv[1])
name = sys.argv[2]
root = sys.argv[3].replace('\\', '/')
project_id = f'{name}@{root}'

data = json.loads(registry.read_text()) if registry.exists() else {}
projects = data.get('projects', [])
projects = [p for p in projects if p.get('rootPath') != root]
projects.append({'id': project_id, 'name': name, 'rootPath': root})
data['projects'] = projects
registry.write_text(json.dumps(data, indent=4))
print(f"Registered project '{name}'")
print(f"Path: {root}")
PY
