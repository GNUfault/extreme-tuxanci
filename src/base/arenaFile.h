#ifndef ARENA_FILE_H
#define ARENA_FILE_H
#define PUBLIC_SERVER_PATH_ARENA "./arena/"

#include "main.h"
#include "arena.h"
#include "textFile.h"

typedef struct {
	char *path;
	textFile_t *map;
} arenaFile_t;

extern bool_t arena_file_is_inicialized();
extern int arena_file_get_value(char *line, char *env, char *val, int len);
extern int arena_file_get_count();
extern char *arena_file_get_name(arenaFile_t * arenaFile);
extern char *arena_file_get_net_name(arenaFile_t * arenaFile);
extern arenaFile_t *arena_file_get_file_format_net_name(char *s);
extern char *arena_file_get_image(arenaFile_t * arenaFile);
#ifndef PUBLIC_SERVER
extern image_t *arena_file_load_image(arenaFile_t *arenaFile, char *filename,
				      char *group, char *name, int alpha);
#endif /* PUBLIC_SERVER */
extern arenaFile_t *arena_file_get(int n);
extern int arena_file_get_id(arenaFile_t * arenaFile);
extern arena_t *arena_file_get_arena(arenaFile_t * arenaFile);
extern arenaFile_t *arena_file_new(char *path);
extern void arena_file_load(char *path);
extern void arena_file_init();
extern void arena_file_quit();

#endif /* ARENA_FILE_H */
