
#ifndef MY_PATH_H

#define MY_PATH_H

#ifdef PUBLIC_SERVER
#define PATH_PREFIX "share/tuxanci-server"
#define PATH_PREFIX_MODULE "lib/tuxanci-server"
#endif

#ifndef PUBLIC_SERVER
#define PATH_PREFIX "share/tuxanci"
#define PATH_PREFIX_MODULE "lib/tuxanci"
#endif

#define PATH_PREFIX_IMAGE PATH_PREFIX "/images"
#define PATH_PREFIX_ARENA PATH_PREFIX "/arena"
#define PATH_PREFIX_MUSIC PATH_PREFIX "/music"
#define PATH_PREFIX_SOUND PATH_PREFIX "/sound"

extern int path_init();
extern char* path_get_prefix();
extern int path_set_prefix(char *path, int size, const char *subpath, const char *name);
extern int path_quit();

#endif

