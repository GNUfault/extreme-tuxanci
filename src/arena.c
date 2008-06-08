
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "main.h"
#include "arena.h"
#include "list.h"
#include "tux.h"
#include "shot.h"
#include "item.h"
#include "myTimer.h"
#include "modules.h"

#ifndef PUBLIC_SERVER
#include "screen_world.h"
#include "layer.h"
#endif

static arena_t *currentArena;

void setCurrentArena(arena_t *p)
{
	currentArena = p;
}

arena_t* getCurrentArena()
{
	return currentArena;
}

arena_t* newArena()
{
	arena_t *new;
	new = malloc( sizeof(arena_t) );
	
#ifndef PUBLIC_SERVER
	new->background = NULL;
	strcpy(new->music, "");
#endif

	new->listTimer = newList();
	new->listTux = newList();
	new->listShot = newList();
	new->listItem = newList();

	return new;
}

int conflictSpace(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2)
{
	return (x1<x2+w2 && x2<x1+w1 && y1<y2+h2 && y2<y1+h1);
}

int isFreeSpace(arena_t *arena, int x, int y, int w, int h)
{
	if( isConflictWithListTux(arena->listTux, x, y, w, h) )return 0;
	if( isConflictWithListShot(arena->listShot, x, y, w, h) )return 0;
	if( isConflictWithListItem(arena->listItem, x, y, w, h) )return 0;
	if( isConflictModule(x, y, w, h) )return 0;
	//if( isConflictWithListWall(arena->listWall, x, y, w, h) )return 0;
	//if( isConflictWithListTeleport(arena->listTeleport, x, y, w, h) )return 0;
	//if( isConflictWithListPipe(arena->listPipe, x, y, w, h) )return 0;

	return 1;
}

void findFreeSpace(arena_t *arena, int *x, int *y, int w, int h)
{
	int z_x;
	int z_y;

	assert( x != NULL );
	assert( y != NULL );

	do{
		z_x = random() % arena->w;
		z_y = random() % arena->h;
	}while( isFreeSpace(arena, z_x, z_y ,w ,h) == 0 );

	*x = z_x;
	*y = z_y;
}

void getCenterScreen(int *screen_x, int *screen_y, int x, int y)
{
	arena_t *arena;

	arena = getCurrentArena();

	*screen_x = x - WINDOW_SIZE_X/2;
	*screen_y = y - WINDOW_SIZE_Y/2;

	if( *screen_x < 0 )
	{
		*screen_x = 0;
	}

	if( *screen_y < 0 )
	{
		*screen_y = 0;
	}

	if( *screen_x + WINDOW_SIZE_X >= arena->w )
	{
		*screen_x = arena->w - WINDOW_SIZE_X;
	}

	if( *screen_y + WINDOW_SIZE_Y >= arena->h )
	{
		*screen_y = arena->h - WINDOW_SIZE_Y;
	}
}

#ifndef PUBLIC_SERVER

void drawArena(arena_t *arena)
{
	int screen_x, screen_y;
	tux_t *tux = NULL;
	int i, j;

	tux = getControlTux(TUX_CONTROL_KEYBOARD_RIGHT);

/*
	screen_x = tux->x - WINDOW_SIZE_X/2;
	screen_y = tux->y - WINDOW_SIZE_Y/2;
*/

	getCenterScreen(&screen_x, &screen_y, tux->x, tux->y);

	for( i = screen_y/arena->background->h ;
	     i <= screen_y/arena->background->h + WINDOW_SIZE_Y/arena->background->h ; i++ )
	{
		for( j = screen_x/arena->background->w ;
		     j <= screen_x/arena->background->w + WINDOW_SIZE_X/arena->background->w ; j++ )
		{
			addLayer(arena->background,
				j * arena->background->w,
				i * arena->background->h,
				0, 0, arena->background->w, arena->background->h, -100);
		}

	}

	drawListTux(arena->listTux);
	//drawListWall(arena->listWall);
	//drawListTeleport(arena->listTeleport);
	//drawListPipe(arena->listPipe);
	drawListShot(arena->listShot);
	drawListItem(arena->listItem);


	if( tux == NULL )
	{
		drawLayerCenter(WINDOW_SIZE_X/2, WINDOW_SIZE_Y/2);
	}
	else
	{
		drawLayerCenter(tux->x, tux->y);
	}
}

#endif

void eventArena(arena_t *arena)
{
	int i;

	//eventConflictTuxWithTeleport(arena->listTux, arena->listTeleport);
	eventConflictTuxWithShot(arena->listTux, arena->listShot);

	for( i = 0 ; i < 8 ; i++)
	{
		eventMoveListShot(arena->listShot);
		//eventConflictShotWithWall(arena->listWall, arena->listShot);
		//eventConflictShotWithTeleport(arena->listTeleport, arena->listShot);
		//eventConflictShotWithPipe(arena->listPipe, arena->listShot);
		eventConflictShotWithItem(arena->listItem, arena->listShot);
		eventConflictTuxWithShot(arena->listTux, arena->listShot);
		eventModule();
	}

	eventListItem(arena->listItem);
	
	eventListTux(arena->listTux);

	eventTimer(arena->listTimer);
}

void destroyArena(arena_t *p)
{
	destroyListItem(p->listTux, destroyTux);
	destroyListItem(p->listShot, destroyShot);
	destroyListItem(p->listItem, destroyItem);
	destroyTimer(p->listTimer);
	free(p);
}
