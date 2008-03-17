
#ifndef PATH_H

#define PATH_H

#include "main.h"

#ifdef DISTDIR

#define PATH_DIR	"share/tuxanci-ng/"

#endif

#ifndef DISTDIR

#define PATH_DIR "./"

#undef DISTDIR
#define DISTDIR ""

#endif

#define PATH_IMAGE	DISTDIR PATH_DIR "image/"
#define PATH_FONT	DISTDIR PATH_DIR "font/"
#define PATH_ARENA	DISTDIR PATH_DIR "arena/"
#define PATH_SOUND	DISTDIR PATH_DIR "sound/"
#define PATH_MUSIC	DISTDIR PATH_DIR "music/"
#define PATH_CONFIG	DISTDIR PATH_DIR "conf/"
#define PATH_LANG	DISTDIR PATH_DIR "lang/"
#define PATH_DATA	DISTDIR PATH_DIR "data/"

#endif 
