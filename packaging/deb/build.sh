#!/bin/sh

rm -rf tmp

mkdir tmp
mkdir tmp/DEBIAN

mkdir tmp/usr/
mkdir tmp/usr/games
mkdir tmp/usr/games/lib
mkdir tmp/usr/games/share
mkdir tmp/usr/share/
mkdir tmp/usr/share/applications/

echo "2.0" > tmp/DEBIAN/debian-binary
cp control tmp/DEBIAN
cp tuxanci.desktop tmp/usr/share/applications/

cd ../..

rm CMakeCache.txt
make clean
cmake . -DDebug=1 -DCMAKE_INSTALL_PREFIX=/usr/games -DCMAKE_DATA_PATH=/usr/games/share -DCMAKE_CONF_PATH=/etc -DLIB_INSTALL_DIR=/usr/games/lib
make DESTDIR=./packaging/deb/tmp/ install

cd ./packaging/deb

cd tmp
md5sum `find . -type f | awk '/.\// { print substr($0, 3) }'` > DEBIAN/md5sums
cd ..

dpkg-deb -b tmp tuxanci_svn-1_i386.deb
