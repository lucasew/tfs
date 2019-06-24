#! /usr/bin/bash

# FIXME: E se não tiver o git instalado?
pkgver () {
  set -o pipefail
    ping describe --long 2>/dev/null | sed 's/\([^-]*-g\)/r\1/;s/-/./g' ||
    printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}
# Coloca o resultado de versão no VERSION
VERSION=$(pkgver || echo UNDEFINED)

cat version.h.in | sed "s/__VERSION__/$VERSION/" > version.h

clang cli.c string_stack.c -o main -g -O0 && echo COMPILADO!
