#!/bin/sh

rm -rf tuxanci_ng-i386-bin tuxanci_ng-i386-bin.tar tuxanci_ng-i386-bin.tar.bz2

mkdir tuxanci_ng-i386-bin
mkdir tuxanci_ng-i386-bin/image
mkdir tuxanci_ng-i386-bin/font
mkdir tuxanci_ng-i386-bin/arena
mkdir tuxanci_ng-i386-bin/data
mkdir tuxanci_ng-i386-bin/lang
mkdir tuxanci_ng-i386-bin/sound
mkdir tuxanci_ng-i386-bin/music


cp image/*.* tuxanci_ng-i386-bin/image
cp font/*.* tuxanci_ng-i386-bin/font
cp arena/*.* tuxanci_ng-i386-bin/arena
cp data/*.* tuxanci_ng-i386-bin/data
cp lang/*.* tuxanci_ng-i386-bin/lang
cp sound/*.* tuxanci_ng-i386-bin/sound
cp music/*.* tuxanci_ng-i386-bin/music
cp src/tuxanci-ng tuxanci_ng-i386-bin

tar -cvf tuxanci_ng-i386-bin.tar tuxanci_ng-i386-bin
bzip2 tuxanci_ng-i386-bin.tar
