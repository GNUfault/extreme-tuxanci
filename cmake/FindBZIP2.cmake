# This scripts search up bzip2 binary and put it to the
# BZIP2_TOOL varable.
FIND_PROGRAM(BZIP2_TOOL
	NAMES bzip2
	PATHS
	/bin
	/usr/bin
	/usr/local/bin
)
IF(NOT BZIP2_TOOL)
	MESSAGE(FATAL_ERROR "Unable to find 'bzip2' executable") 
ENDIF(NOT BZIP2_TOOL)
