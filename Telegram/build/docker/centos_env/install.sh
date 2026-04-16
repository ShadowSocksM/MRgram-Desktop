#!/usr/bin/env bash
set -exuo pipefail

cd Telegram
DESTDIR="../$DESTDIR" cmake --install ../out --config "${CONFIG:-Release}" "$@"
