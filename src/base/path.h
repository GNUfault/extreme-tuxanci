
#ifndef PATH_H

#define PATH_H

#include "main.h"

#ifndef PREFIX
  #define PREFIX "/usr/local"
#endif

#ifndef PUBLIC_SERVER
  #define PATH_DIR	PREFIX	"/share/tuxanci-ng/"
  #define PATH_MODULES 	PREFIX	"/lib/tuxanci-ng/"
#endif

#ifdef PUBLIC_SERVER
  #define PATH_DIR	PREFIX	"/share/tuxanci-ng-server/"
  #define PATH_MODULES	PREFIX	"/lib/tuxanci-ng-server/"
#endif

#define PATH_IMAGE	PATH_DIR "image/"
#define PATH_FONT	PATH_DIR "font/"
#define PATH_ARENA	PATH_DIR "arena/"
#define PATH_SOUND	PATH_DIR "sound/"
#define PATH_MUSIC	PATH_DIR "music/"
#define PATH_CONFIG	PATH_DIR "conf/"
#define PATH_LANG	PATH_DIR "lang/"
#define PATH_DATA	PATH_DIR "doc/"

#endif
