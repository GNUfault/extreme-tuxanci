#!/usr/bin/env bash
 
make clean
# remove folders
find ./ -type d -name CMakeFiles -print |xargs -i rm {} -rf
# remove cmake files
find ./ \( -type f -name \*.cmake -print , -type f -name CMakeCache.txt -print , -type f -name Makefile -print \) |grep -v "./cmake" |xargs -i rm {} -rf
find ./ -type f -name cmake_install.cmake |xargs -i rm {} -rf
# remove precompiled in files
find ./ -name \*.in -print |while read r; do rm -rf ${r/\.in/}; done

