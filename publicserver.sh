#!/bin/bash
export LD_LIBRARY_PATH="/usr/debug/lib/"
ldd -u build-server/publicserver
ulimit -c unlimited
nohup ./build-server/publicserver &
