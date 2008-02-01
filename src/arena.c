
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "arena.h"
#include "list.h"
#include "tux.h"
#include "shot.h"
#include "wall.h"
#include "item.h"

arena_t* newArena()
{
	arena_t *new;
	new = malloc( sizeof(arena_t) );
	
	new->background = NULL;
	strcpy(new->music, "");

	new->listTux = newList();
	new->listShot = newList();
	new->listWall = newList();
	new->listItem = newList();

	return new;
}

void destroyArena(arena_t *p)
{
	destroyListItem(p->listTux, destroyTux);
	destroyListItem(p->listShot, destroyShot);
	destroyListItem(p->listWall, destroyWall);
	destroyListItem(p->listItem, destroyItem);

	free(p);
}
