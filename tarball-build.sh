#!/bin/sh

rm -rf tuxanci_ng tuxanci_ng.tar tuxanci_ng.tar.bz2

mkdir tuxanci_ng
mkdir tuxanci_ng/image
mkdir tuxanci_ng/font
mkdir tuxanci_ng/arena
mkdir tuxanci_ng/data
mkdir tuxanci_ng/lang
mkdir tuxanci_ng/sound
mkdir tuxanci_ng/music
mkdir tuxanci_ng/include
mkdir tuxanci_ng/src


cp image/*.* tuxanci_ng/image
cp font/*.* tuxanci_ng/font
cp arena/*.* tuxanci_ng/arena
cp data/*.* tuxanci_ng/data
cp lang/*.* tuxanci_ng/lang
cp sound/*.* tuxanci_ng/sound
cp music/*.* tuxanci_ng/music
cp include/*.* tuxanci_ng/include
cp src/* tuxanci_ng/src
cp Makefile control deb-ver tuxanci_ng
cp *.sh tuxanci_ng

tar -cvf tuxanci_ng.tar tuxanci_ng
bzip2 tuxanci_ng.tar
