# Security Policy

## Supported Versions

ClubEngine is under active development. Only the latest commit on `main` receives fixes.

## Reporting a Vulnerability

Please **do not** open a public issue for security problems.

Instead, report it privately via GitHub's
[Report a vulnerability](../../security/advisories/new) button on the Security tab.

Include:
- What the issue is and where (file / function)
- Steps or a sample file to reproduce it
- What impact you think it has
- Logs from the run where it happened (found in `Saved/Logs/`)
- A crash dump or stack trace, if you have one
- Your OS, GPU/driver version, and the commit hash you built from

Logs can contain local file paths or your username, so check them before attaching.
Since reports go through private vulnerability reporting, they're only visible to maintainers.

We'll try to respond within 7 days. This is a student/club project maintained
in our free time, so fixes may take longer, and we'll keep you updated.

## Scope

In scope: memory safety bugs, crashes from malformed asset/scene files,
path traversal in file loading, and similar issues in the engine code.

Out of scope: bugs in third-party dependencies (please report those upstream),
and issues that require modifying the engine source yourself.