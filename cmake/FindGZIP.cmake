# This scripts search up bzip2 binary and put it to the
# GZIP_TOOL varable.
FIND_PROGRAM(GZIP_TOOL
	NAMES gzip
	PATHS
	/bin
	/usr/bin
	/usr/local/bin
)
IF(NOT GZIP_TOOL)
	IF(GZIP_REQUIRED)
		MESSAGE(FATAL_ERROR "Unable to find 'gzip' executable")
	ENDIF(GZIP_REQUIRED)
ENDIF(NOT GZIP_TOOL)
