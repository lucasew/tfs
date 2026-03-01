# Repository Conventions and Guidelines

## Project Overview
This repository contains `tfs`, a Tree FileSystem implementation in C.

## Locations
- entrypoints -> `main.c` (main CLI entrypoint)
- routing/controllers -> `cli.c` (handles command line options and commands execution)
- services/logic -> `tree.c`, `tree_prettyprint.c`, `string_stack.c` (handles internal operations and structures)
- build/compilation -> `build.sh` (bash script to configure and compile the binary)
- tools/ci -> `mise.toml` (task configurations and tools like workspaced), `.github/workflows/autorelease.yml` (CI workflow)
