#!/bin/bash

set -e

. $(dirname $0)/config.sh

cd "$SOURCE_ROOT"

LAST_VERSION=$(git tag | cut -d'.' -f 3 | sort -g | tail -n 1)
VERSION_PATCH=$(( $LAST_VERSION + 1 ))

set_shell_variable VERSION_PATCH $VERSION_PATCH
set_shell_variable VERSION_FULL 0.$VERSION_PATCH
