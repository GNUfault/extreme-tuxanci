
#include <stdlib.h>
#include <assert.h>

#include "main.h"
#include "wall.h"
#include "shot.h"

#ifndef PUBLIC_SERVER
#include "layer.h"
#include "screen_world.h"
#endif

#ifdef PUBLIC_SERVER
#include "publicServer.h"
#endif

#ifndef PUBLIC_SERVER	
wall_t* newWall(int x, int y, int w, int h,
	int img_x, int img_y, int layer, SDL_Surface *img)
#endif

#ifdef PUBLIC_SERVER	
wall_t* newWall(int x, int y, int w, int h,
	int img_x, int img_y, int layer)
#endif
{
	wall_t *new;
	
#ifndef PUBLIC_SERVER	
	assert( img != NULL );
#endif
	new  = malloc( sizeof(wall_t) );
	assert( new != NULL );

	new->x = x;
	new->y = y;
	new->w = w;
	new->h = h;
	new->img_x = img_x;
	new->img_y = img_y;
	new->layer = layer;
#ifndef PUBLIC_SERVER
	new->img = img;
#endif

	return new;
}

#ifndef PUBLIC_SERVER	

void drawWall(wall_t *p)
{
	assert( p != NULL );

	addLayer(p->img, p->img_x, p->img_y, 0, 0, p->img->w, p->img->h, p->layer);
}

void drawListWall(list_t *listWall)
{
	wall_t *thisWall;
	int i;

	assert( listWall != NULL );

	for( i = 0 ; i < listWall->count ; i++ )
	{
		thisWall  = (wall_t *)listWall->list[i];
		assert( thisWall != NULL );
		drawWall(thisWall);
	}
}

#endif

int isConflictWithListWall(list_t *listWall, int x, int y, int w, int h)
{
	wall_t *thisWall;
	int i;

	assert( listWall != NULL );

	for( i = 0 ; i < listWall->count ; i++ )
	{
		thisWall  = (wall_t *)listWall->list[i];
		assert( thisWall != NULL );

		if( conflictSpace(x, y, w, h,
		    thisWall->x, thisWall->y, thisWall->w, thisWall->h) )
		{
			return 1;
		}
	}

	return 0;
}

void eventConflictShotWithWall(list_t *listWall, list_t *listShot)
{
	shot_t *thisShot;
	int i;

	assert( listWall != NULL );
	assert( listShot != NULL );
	
	for( i = 0 ; i < listShot->count ; i++ )
	{
		thisShot  = (shot_t *)listShot->list[i];
		assert( thisShot != NULL );

		if( isConflictWithListWall(listWall, thisShot->x, thisShot->y, thisShot->w, thisShot->h) )
		{
			if( thisShot->author->bonus == BONUS_GHOST &&
			    thisShot->author->bonus_time > 0 )
			{
				continue;
			}

			delListItem(listShot, i, destroyShot);
			i--;
		}
	}
}

void destroyWall(wall_t *p)
{
	assert( p != NULL );
	free(p);
}

