#!/usr/bin/env bash
set -exuo pipefail
TTYFLAGS=
if tty -s; then
    TTYFLAGS=-it
fi
USERFLAGS=0:0
if ! docker info -f "{{println .SecurityOptions}}" | grep rootless; then
    USERFLAGS=$(id -u):$(id -g)
fi
docker run --rm $TTYFLAGS \
    -u $USERFLAGS \
    -v "$PWD:/usr/src/tdesktop" \
    -e CONFIG=${CONFIG:-Release} \
    ghcr.io/telegramdesktop/tdesktop/centos_env@sha256:e2fe8063c7cddf0c0bd039032d0be7b69e4f01fd75e3b637f00d9df8d7e8a0ea \
    "$@"
