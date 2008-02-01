
#ifndef ARENA_H

#define ARENA_H

#include "main.h"
#include "interface.h"
#include "list.h"

typedef struct arena_struct
{
	char music[STR_SIZE];
	SDL_Surface *background;
	list_t *listTux;
	list_t *listShot;
	list_t *listWall;
	list_t *listItem;
} arena_t;

extern arena_t* newArena();
extern void destroyArena(arena_t *p);

#endif
