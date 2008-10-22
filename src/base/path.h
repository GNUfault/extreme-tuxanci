
#ifndef PATH_H
/* #undef USE_MAGICK */
	#define TUXANCI_VERSION				"dev"
	#define PATH_H
	#define PREFIX					"/usr/local"
	#ifdef PUBLIC_SERVER
		#define APPNAME					"tuxanci-server"
		#define SERVER_CONFIG	"/etc" "/"	APPNAME	"/server.conf"
	#else
		#define APPNAME					"tuxanci"
	#endif
	#define PATH_DIR	"/usr/local/share"	"/"	APPNAME	"-" "dev" "/"
	// windows bug
	#ifndef __WIN32__
		#define PATH_MODULES 	"/usr/local/lib" "/"	APPNAME	"-" "dev" "/"
	#else
		#define PATH_MODULES 	PREFIX
	#endif
	#define PATH_IMAGE	PATH_DIR	"images/"
	#define FONT_FILE	"/usr/local/share/tuxanci-dev/font/DejaVuSans.ttf"
	#define PATH_ARENA	PATH_DIR	"arena/"
	#define PATH_SOUND	PATH_DIR	"sound/"
	#define PATH_MUSIC	PATH_DIR	"music/"
	#define PATH_LOCALE	"/usr/local/share/locale"	"/"
	#define PATH_DOC	"/usr/local/share/doc"	"/"		APPNAME	"-" "dev" "/"
	#define PACKAGE		"tuxanci"
	#include "main.h"
#endif
