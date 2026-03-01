#! /usr/bin/bash

# Script pra compilar o programa

# FIXME: E se não tiver o git instalado?
pkgver() {
    # https://wiki.archlinux.org/index.php/VCS_package_guidelines
    ( set -o pipefail
        git describe --long 2>/dev/null | sed 's/\([^-]*-g\)/r\1/;s/-/./g' ||
        printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
    )
}
# Coloca o resultado de versão no VERSION
VERSION=$(pkgver || echo UNDEFINED)

cat version.h.in | sed "s/__VERSION__/$VERSION/" > version.h

clang src/cli/cli.c src/core/tree.c src/core/members.c src/core/tree_prettyprint.c src/utils/string_stack.c src/utils/args.c src/utils/log.c -o main -g -O0 && echo COMPILADO!
