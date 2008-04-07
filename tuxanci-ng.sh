#!/bin/bash
export LD_LIBRARY_PATH="/usr/debug/lib/"
ldd -u build/tuxanci-ng
ulimit -c unlimited
build/tuxanci-ng
