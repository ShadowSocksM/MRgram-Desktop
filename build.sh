#!/usr/bin/env bash
set -exuo pipefail
./run-in-docker.sh \
    /usr/src/tdesktop/Telegram/build/docker/centos_env/build.sh \
    -D TDESKTOP_API_ID=611335 \
    -D TDESKTOP_API_HASH=d524b414d21f4d37f08684c1df41ac9c
