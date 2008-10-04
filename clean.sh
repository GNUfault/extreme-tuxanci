#!/usr/bin/env bash
 
# remove compiled files
[ -e "[Mm]akefile" ] && make clean

# remove folders
find ./ -type d -name CMakeFiles -print | xargs -i rm {} -rf

# remove cmake files
find ./ \( -type f -name \*.cmake -print , -type f -name CMakeCache.txt -print , -type f -name Makefile -print \) | grep -v "./cmake" | xargs -i rm {} -f
find ./ \( -type f -name install_manifest.txt -print , -type f -name cmake_install.cmake -print \) | xargs -i rm {} -f

# remove precompiled in files
find ./ -name \*.in -print | while read r; do rm -f ${r/\.in/}; done

