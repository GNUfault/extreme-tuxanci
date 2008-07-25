#!/bin/sh

echo "*** GNU/Linux Binary Packager ***"

if [ ! $1 ]; then
	echo "usage: $0 version"
	exit 1
fi

VERSION=$1
INSTALL_DIR="pkgs/tuxanci-${VERSION}-linux"
BUILD_DIR="pkgs/tuxanci-${VERSION}-linux-build"
SVN='https://opensvn.csie.org/tuxanci_ng/'
log='linux.log'
error="Error! See pkgs/${log} for more iformation"

2&>1

rm -rfv $INSTALL_DIR $BUILD_DIR > pkgs/"${log}" || ( echo $error; exit 1 )

mkdir -pv pkgs >> pkgs/"${log}" || ( echo $error; exit 1 )

echo "* Fetching files from SVN"
svn export $SVN $BUILD_DIR >> pkgs/"${log}" || ( echo $error; exit 1 )

echo "* Configuring"
echo "#define TUXANCI_NG_VERSION \"${VERSION}\"" > $BUILD_DIR/config.h || ( echo $error; exit 1 )

echo "* Compiling"
prevdir=`pwd`
cd $BUILD_DIR
cmake . -DPREFIX=\\\".\\\" -DDATADIR=\\\".\\\" -DLIBDIR=\\\"./modules\\\" -DCMAKE_INSTALL_PREFIX:PATH=/ -DCMAKE_INSTALL_DATADIR=/ >> "${prevdir}"/pkgs/"${log}" || ( echo $error; exit 1 )
make >> "${prevdir}"/pkgs/"${log}" || ( echo $error; exit 1 )
make DESTDIR="${prevdir}/${INSTALL_DIR}" install >> "${prevdir}"/pkgs/"${log}" || ( echo $error; exit 1 )
cd $prevdir

echo "* Packaging"
tar cjf "${INSTALL_DIR}".tar.bz2 $INSTALL_DIR || ( echo $error; exit 1 )

echo "** GNU/Linux binary package is ready!"

