
#ifndef PATH_H

#define PATH_H

#include "main.h"

#ifndef PUBLIC_SERVER
#define PATH_DIR		"share/tuxanci-ng/"
#define PATH_MODULES DESTDIR	"lib/tuxanci-ng/"
#endif

#ifdef PUBLIC_SERVER
#define PATH_DIR		"share/tuxanci-ng-server/"
#define PATH_MODULES DESTDIR 	"lib/tuxanci-ng-server/"
#endif

#define PATH_IMAGE	DESTDIR PATH_DIR "image/"
#define PATH_FONT	DESTDIR PATH_DIR "font/"
#define PATH_ARENA	DESTDIR PATH_DIR "arena/"
#define PATH_SOUND	DESTDIR PATH_DIR "sound/"
#define PATH_MUSIC	DESTDIR PATH_DIR "music/"
#define PATH_CONFIG	DESTDIR PATH_DIR "conf/"
#define PATH_LANG	DESTDIR PATH_DIR "lang/"
#define PATH_DATA	DESTDIR PATH_DIR "doc/"

#endif
