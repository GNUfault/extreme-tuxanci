# - Find zip
# Find the native ZIP includes and library
#
#  ZIP_INCLUDE_DIR - where to find zip.h, etc.
#  ZIP_LIBRARIES   - List of libraries when using zip.
#  ZIP_FOUND       - True if zip found.


IF (ZIP_INCLUDE_DIR)
  # Already in cache, be silent
  SET(ZIP_FIND_QUIETLY TRUE)
ENDIF (ZIP_INCLUDE_DIR)

FIND_PATH(ZIP_INCLUDE_DIR zip.h)

SET(ZIP_NAMES zip)
FIND_LIBRARY(ZIP_LIBRARY NAMES ${ZIP_NAMES} )

# handle the QUIETLY and REQUIRED arguments and set ZIP_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ZIP DEFAULT_MSG ZIP_LIBRARY ZIP_INCLUDE_DIR)

IF(ZIP_FOUND)
  SET( ZIP_LIBRARIES ${ZIP_LIBRARY} )
ELSE(ZIP_FOUND)
   SET(ZIP_FOUND FALSE)
      IF (ZIP_REQUIRED)
			        MESSAGE(FATAL_ERROR "libzip not found")
	  ENDIF (ZIP_REQUIRED)
  SET( ZIP_LIBRARIES )
ENDIF(ZIP_FOUND)

MARK_AS_ADVANCED( ZIP_LIBRARY ZIP_INCLUDE_DIR )
