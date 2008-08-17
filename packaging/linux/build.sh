#!/bin/sh
# Created by Tomas Chvatal (Scarabeus_IV)
###############################################################################
echo "<******************************>"
echo "<GNU/Linux TUXANCI Binary Packager>"
echo "<******************************>"
# This tool is only for tuxanci game developers, don't use it for yourself
###############################################################################
# HELP
###############################################################################
if [[ $1 == "--help" ]]; then
	echo "usage: $0 -v version"
	echo "-v 0.2.5 \"compile version 0.2.5\""
	echo "!if no version is specified svn version is created!"
	echo "!This build tool is for 64b version (creates 32b and 64b thought)!"
	exit 1
fi
###############################################################################
# ARGUMENT PASSING
###############################################################################
while getopts v:s arg
do
	case $arg in
		v) VERSION=${OPTARG};;
		*) VERSION="9999";;
	esac
done
###############################################################################
# VARIABLES
###############################################################################
APPNAME="tuxanci"
SVN="https://opensvn.csie.org/tuxanci_ng/"
BUNDLE_PREFIX="${HOME}/tuxanci-bundle"
LOG="${BUNDLE_PREFIX}/linux.log"
SOURCE="${BUNDLE_PREFIX}/source"
D="${BUNDLE_PREFIX}/${APPNAME}"
ARCHS="64b 32b"
OPTIONS="client server"
ERROR_MESSAGE="Check ${LOG}, cause i was unable to finish my stuff correctly!"
CMAKE_PARAMS="-DBundle=1 -DTUXANCI_VERSION=${VERSION}"
###############################################################################
# PREPARING ENVIROMENT
###############################################################################
mkdir -p "${BUNDLE_PREFIX}" || ( echo "I was unable to create working directory"; exit 1 )
#rm -rf "${BUNDLE_PREFIX}"/*
touch "${LOG}" || ( echo "I was unable to create log file"; exit 1 )
echo "" > "${LOG}"	# LOG CLEANUP
echo "<Downloading files from SVN repository>"
echo "<******************************>"
svn export $SVN ${SOURCE} >> "${LOG}" || ( echo "${ERROR_MESSAGE}"; exit 1 )
# fix prefix placement

for X in ${ARCHS}; do
if [[ ${X} == "32b" ]]; then
	CMAKE_PARAMS="${CMAKE_PARAMS} -DCMAKE_C_FLAGS=-m32"
fi
###############################################################################
# BUILDING CLIENT
###############################################################################
echo "<******************************>"
echo "<Building Client ${X}>"
echo "<******************************>"
cd "${SOURCE}"
cmake . ${CMAKE_PARAMS} >> "${LOG}" || ( echo "${ERROR_MESSAGE}"; exit 1 )
make >> "${LOG}" || ( echo "${ERROR_MESSAGE}"; exit 1 )
make install DESTDIR="${D}_client-${X}/" >> "${LOG}" || ( echo "${ERROR_MESSAGE}"; exit 1 )
###############################################################################
# SRC CLEANUP
###############################################################################
rm CMakeCache.txt || ( echo "I was unable to cleanup CMakeCache.txt"; exit 1; )
make clean
###############################################################################
# BUILDING SERVER
###############################################################################
echo "<******************************>"
echo "<Building Server ${X}>"
echo "<******************************>"
cd "${SOURCE}"
cmake . ${CMAKE_PARAMS} -DServer=ON >> "${LOG}" || ( echo "${ERROR_MESSAGE}"; exit 1 )
make >> "${LOG}" || ( echo "${ERROR_MESSAGE}"; exit 1 )
make install DESTDIR="${D}_server-${X}/" >> "${LOG}" || ( echo "${ERROR_MESSAGE}"; exit 1 )
###############################################################################
# SRC CLEANUP
###############################################################################
rm CMakeCache.txt || ( echo "I was unable to cleanup CMakeCache.txt"; exit 1; )
make clean
###############################################################################
done
# DESTRUCTION OF SRC
###############################################################################
cd "${BUNDLE_PREFIX}"
#rm -rf "${SOURCE}"
###############################################################################
# BUILDING TARS
###############################################################################
echo "<Creating tar.bz2 archives>"
echo "<******************************>"
for X in ${ARCHS}; do
	for Y in ${OPTIONS}; do
		tar cjf "${APPNAME}"_"${Y}"-"${X}".tar.bz2 "${APPNAME}"_"${Y}"-"${X}"/*
	done
done
###############################################################################
# DESTRUCTION OF UNPACKED BINARIES
###############################################################################
for X in ${ARCHS}; do
	for Y in ${OPTIONS}; do
		rm -rf "${APPNAME}"_"${Y}"-"${X}"/
	done
done
###############################################################################
# SHOW WHAT HAVE WE DONE
###############################################################################
echo "<What have i created :>"
echo "<******************************>"
find ./ -maxdepth 1 -type f -print
echo
echo
echo "<GNU/Linux binary packages are ready>"
echo
echo
