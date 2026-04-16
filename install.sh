#!/usr/bin/env bash
set -exuo pipefail
./run-in-docker.sh env DESTDIR="${DESTDIR:-.}" /usr/src/tdesktop/Telegram/build/docker/centos_env/install.sh "$@"
