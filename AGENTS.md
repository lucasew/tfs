# Tree File System (TFS) - Operational Conventions

## Layout

This repository follows a strict directory structure grouped by domain and responsibility:
- `src/core` -> core filesystem structure, logic, and parsing (`tree.c`, `members.c`).
- `src/utils` -> cross-domain utilities such as error handling, command line argument parsing, and stack utilities (`log.c`, `args.c`, `string_stack.c`).
- `src/cli` -> primary entry point, the REPL loop, command handling logic.
- `tests` -> manual entry points and explicit test harnesses.

## Rules

- No `#include "file.c"`: Always separate definitions into header (`.h`) files and use them to link dependencies.
- Use `tfs_report_error(const char *msg)` from `src/utils/log.h` for unexpected errors, do not use simple `printf` or `fprintf` directly when errors are fatal or unknown.
- Use `./build.sh` for compiling.
