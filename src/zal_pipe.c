
#include <stdlib.h>
#include <assert.h>

#include "pipe.h"
#include "layer.h"
#include "screen_world.h"
#include "shot.h"

pipe_t* newPipe(int x, int y, int w, int h, int layer, int id, int id_out, int position, SDL_Surface *img)
{
	pipe_t *new;
	
	assert( img != NULL );

	new  = malloc( sizeof(pipe_t) );
	assert( new != NULL );

	new->x = x;
	new->y = y;
	new->w = w;
	new->h = h;
	new->layer = layer;
	new->id = id;
	new->id_out = id_out;
	new->position = position;
	new->img = img;

	return new;
}

void drawPipe(pipe_t *p)
{
	assert( p != NULL );

	addLayer(p->img, p->x, p->y, 0, 0, p->img->w, p->img->h, p->layer);
}

void drawListPipe(list_t *listPipe)
{
	pipe_t *thisPipe;
	int i;

	assert( listPipe != NULL );

	for( i = 0 ; i < listPipe->count ; i++ )
	{
		thisPipe  = (pipe_t *)listPipe->list[i];
		assert( thisPipe != NULL );
		drawPipe(thisPipe);
	}
}

pipe_t* isConflictWithListPipe(list_t *listPipe, int x, int y, int w, int h)
{
	pipe_t *thisPipe;
	int i;

	assert( listPipe != NULL );

	for( i = 0 ; i < listPipe->count ; i++ )
	{
		thisPipe  = (pipe_t *)listPipe->list[i];
		assert( thisPipe != NULL );

		if( conflictSpace(x, y, w, h,
		    thisPipe->x, thisPipe->y, thisPipe->w, thisPipe->h) )
		{
			return thisPipe;
		}
	}

	return NULL;
}

static int negPosition(int n)
{
	switch( n )
	{
		case TUX_UP :
		return TUX_DOWN;

		case TUX_LEFT :
		return TUX_RIGHT;

		case TUX_RIGHT :
		return TUX_LEFT;

		case TUX_DOWN :
		return TUX_UP;

		default :
			assert( ! "premenna n ma zlu hodnotu !" );
		break;
	}
}

static pipe_t* getPipeId(list_t *listPipe, int id)
{
	int i;

	for( i = 0 ; i < listPipe->count ; i++ )
	{
		pipe_t *thisPipe;

		thisPipe = (pipe_t *) listPipe->list[i];

		if( thisPipe->id == id )
		{
			return thisPipe;
		}
	}

	return NULL;
}

static int myAbs(int n)
{
	return ( n > 0 ? n : -n );
}

static int getSppedShot(shot_t *shot)
{
	return ( myAbs(shot->px) > myAbs(shot->py) ? myAbs(shot->px) : myAbs(shot->py) );
}
 
static void transformShot(shot_t *shot, int position)
{
	int speed;

	speed = getSppedShot(shot);

	switch( position )
	{
		case TUX_UP :
			shot->px = 0;
			shot->py = -speed;
		break;

		case TUX_LEFT :
			shot->px = -speed;
			shot->py = 0;
		break;

		case TUX_RIGHT :
			shot->px = speed;
			shot->py = 0;
		break;

		case TUX_DOWN :
			shot->px = 0;
			shot->py = +speed;
		break;

	}

	shot->position = position;	
	shot->isCanKillAuthor = TRUE;
}

static void moveShot(shot_t *shot, pipe_t *pipe, list_t *listPipe)
{
	pipe_t *distPipe;
	int offset;

	distPipe = getPipeId(listPipe, pipe->id_out);

	if( distPipe == NULL )
	{
		fprintf(stderr, "Pipe %d ID not found\n", pipe->id);
		return;
	}

	printf("XXX\n");

	switch( shot->position )
	{
		case TUX_UP :
		case TUX_DOWN :
			offset = shot->x - pipe->x;
		break;

		case TUX_RIGHT :
		case TUX_LEFT :
			offset = shot->y - pipe->y;
		break;
	}

	transformShot(shot, distPipe->position);

	switch( shot->position )
	{
		case TUX_UP :
			shot->x = distPipe->x + offset;
			shot->y = distPipe->y;
		break;

		case TUX_LEFT :
			shot->x = distPipe->x;
			shot->y = distPipe->y + offset;
		break;

		case TUX_RIGHT :
			shot->x = distPipe->x + distPipe->w;
			shot->y = distPipe->y + offset;
		break;

		case TUX_DOWN :
			shot->x = distPipe->x + offset;
			shot->y = distPipe->y + distPipe->h;
		break;
	}
}

void eventConflictShotWithPipe(list_t *listPipe, list_t *listShot)
{
	shot_t *thisShot;
	pipe_t *thisPipe;
	int i;

	assert( listPipe != NULL );
	assert( listShot != NULL );
	
	for( i = 0 ; i < listShot->count ; i++ )
	{
		thisShot  = (shot_t *)listShot->list[i];
		assert( thisShot != NULL );

		if( ( thisPipe = isConflictWithListPipe(listPipe, thisShot->x, thisShot->y,
			thisShot->w, thisShot->h) ) != NULL )
		{
			if( thisShot->author->bonus == BONUS_GHOST &&
			    thisShot->author->bonus_time > 0 )
			{
				continue;
			}

			if( negPosition( thisShot->position ) == thisPipe->position )
			{
				moveShot(thisShot, thisPipe, listPipe);
			}
			else
			{
				delListItem(listShot, i, destroyShot);
				i--;
			}
		}
	}
}

void destroyPipe(pipe_t *p)
{
	assert( p != NULL );
	free(p);
}

