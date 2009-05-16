###############################################################################
# tuxanci/src/*/CMakeLists.txt
###############################################################################
# Generate source_code files from .c files we find in directory
###############################################################################
FILE ( GLOB src ${fn}/*.c )
FOREACH ( fle ${src} )
	GET_FILENAME_COMPONENT ( fnp ${fle} NAME_WE )
	GET_FILENAME_COMPONENT ( pth ${fle} PATH )
	SET ( SRC_${fn} ${SRC_${fn}} ${pth}/${fnp} )
ENDFOREACH ( fle )
###############################################################################
