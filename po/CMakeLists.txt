# vyroba jazyku pomoci gettexu doufam ze na nej prejdem
IF ( HAVE_GETTEXT )
	INCLUDE( Po )
	COMPILE_PO_FILES( ${CMAKE_SOURCE_DIR}/po MO_FILES )
	ADD_CUSTOM_TARGET( mofiles ALL DEPENDS ${MO_FILES} )
ENDIF ( HAVE_GETTEXT )
