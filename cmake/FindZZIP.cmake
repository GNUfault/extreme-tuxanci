# - Find zzip
# Find the native ZZIP includes and library
#
#  ZZIP_INCLUDE_DIR - where to find zzip.h, etc.
#  ZZIP_LIBRARIES   - List of libraries when using zzip.
#  ZZIP_FOUND       - True if zzip found.


IF (ZZIP_INCLUDE_DIR)
  # Already in cache, be silent
  SET(ZZIP_FIND_QUIETLY TRUE)
ENDIF (ZZIP_INCLUDE_DIR)

FIND_PATH(ZZIP_INCLUDE_DIR zzip.h)

SET(ZZIP_NAMES zzip)
FIND_LIBRARY(ZZIP_LIBRARY NAMES ${ZZIP_NAMES} )

# handle the QUIETLY and REQUIRED arguments and set ZZIP_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ZZIP DEFAULT_MSG ZZIP_LIBRARY ZZIP_INCLUDE_DIR)

IF(ZZIP_FOUND)
  SET( ZZIP_LIBRARIES ${ZZIP_LIBRARY} )
ELSE(ZZIP_FOUND)
  SET( ZZIP_LIBRARIES )
ENDIF(ZZIP_FOUND)

MARK_AS_ADVANCED( ZZIP_LIBRARY ZZIP_INCLUDE_DIR )
