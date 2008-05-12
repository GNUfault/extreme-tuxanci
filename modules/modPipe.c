
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "main.h"
#include "interface.h"
#include "modules.h"
#include "tux.h"
#include "shot.h"
#include "list.h"
#include "gun.h"
#include "image.h"

#ifdef PUBLIC_SERVER
#include "publicServer.h"
#endif

typedef struct pipe_struct
{
	int x; // poloha steny	
	int y;

	int w; // rozmery steny
	int h;

	int id;
	int id_out;
	int position;

	int layer; // vrstva

#ifndef PUBLIC_SERVER	
	SDL_Surface *img; //obrazok
#endif
} pipe_t;

static export_fce_t *export_fce;

static list_t *listPipe;

#ifndef PUBLIC_SERVER	
static pipe_t* newPipe(int x, int y, int w, int h, int layer, int id, int id_out, int position, SDL_Surface *img)
#endif

#ifdef PUBLIC_SERVER	
static pipe_t* newPipe(int x, int y, int w, int h, int layer, int id, int id_out, int position)
#endif

{
	pipe_t *new;
	
#ifndef PUBLIC_SERVER	
	assert( img != NULL );
#endif

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
#ifndef PUBLIC_SERVER	
	new->img = img;
#endif
	return new;
}

#ifndef PUBLIC_SERVER	

static void drawPipe(pipe_t *p)
{
	assert( p != NULL );

	export_fce->fce_addLayer(p->img, p->x, p->y, 0, 0, p->img->w, p->img->h, p->layer);
}

#endif

static pipe_t* isConflictWithListPipe(list_t *listPipe, int x, int y, int w, int h)
{
	pipe_t *thisPipe;
	int i;

	assert( listPipe != NULL );

	for( i = 0 ; i < listPipe->count ; i++ )
	{
		thisPipe  = (pipe_t *)listPipe->list[i];
		assert( thisPipe != NULL );

		if( export_fce->fce_conflictSpace(x, y, w, h,
		    thisPipe->x, thisPipe->y, thisPipe->w, thisPipe->h) )
		{
			return thisPipe;
		}
	}

	return NULL;
}

static void destroyPipe(pipe_t *p)
{
	assert( p != NULL );
	free(p);
}


static void cmd_teleport(char *line)
{
	char str_x[STR_NUM_SIZE];
	char str_y[STR_NUM_SIZE];
	char str_w[STR_NUM_SIZE];
	char str_h[STR_NUM_SIZE];
	char str_id[STR_NUM_SIZE];
	char str_id_out[STR_NUM_SIZE];
	char str_position[STR_NUM_SIZE];
	char str_layer[STR_NUM_SIZE];
	char str_image[STR_SIZE];
	pipe_t *new;

	if( export_fce->fce_getValue(line, "x", str_x, STR_NUM_SIZE) != 0 )return;
	if( export_fce->fce_getValue(line, "y", str_y, STR_NUM_SIZE) != 0 )return;
	if( export_fce->fce_getValue(line, "w", str_w, STR_NUM_SIZE) != 0 )return;
	if( export_fce->fce_getValue(line, "h", str_h, STR_NUM_SIZE) != 0 )return;
	if( export_fce->fce_getValue(line, "id", str_id, STR_NUM_SIZE) != 0 )return;
	if( export_fce->fce_getValue(line, "id_out", str_id_out, STR_NUM_SIZE) != 0 )return;
	if( export_fce->fce_getValue(line, "position", str_position, STR_NUM_SIZE) != 0 )return;
	if( export_fce->fce_getValue(line, "layer", str_layer, STR_NUM_SIZE) != 0 )return;
	if( export_fce->fce_getValue(line, "image", str_image, STR_SIZE) != 0 )return;

#ifndef PUBLIC_SERVER
	new = newPipe(atoi(str_x), atoi(str_y),
			atoi(str_w), atoi(str_h),
			atoi(str_layer), atoi(str_id), atoi(str_id_out), atoi(str_position),
			export_fce->fce_getImage(IMAGE_GROUP_USER, str_image) );
#endif

#ifdef PUBLIC_SERVER
	new = newPipe(atoi(str_x), atoi(str_y),
			atoi(str_w), atoi(str_h),
			atoi(str_layer), atoi(str_id), atoi(str_id_out), atoi(str_position) );
#endif

	addList(listPipe, new);
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

static void moveShot(shot_t *shot, int position, int src_x, int src_y,
	int dist_x, int dist_y, int dist_w, int dist_h)
{
	int offset = 0;

	switch( shot->position )
	{
		case TUX_UP :
		case TUX_DOWN :
			offset = shot->x - src_x;
		break;

		case TUX_RIGHT :
		case TUX_LEFT :
			offset = shot->y - src_y;
		break;
	}

	transformShot(shot, position);

	switch( shot->position )
	{
		case TUX_UP :
			shot->x = dist_x + offset;
			shot->y = dist_y;
		break;

		case TUX_LEFT :
			shot->x = dist_x;
			shot->y = dist_y + offset;
		break;

		case TUX_RIGHT :
			shot->x = dist_x + dist_w;
			shot->y = dist_y + offset;
		break;

		case TUX_DOWN :
			shot->x = dist_x + offset;
			shot->y = dist_y + dist_h;
		break;
	}

	shot->x += shot->px;
	shot->y += shot->py;

	if( export_fce->fce_getNetTypeGame() == NET_GAME_TYPE_SERVER )
	{
		export_fce->fce_proto_send_shot_server(PROTO_SEND_ALL, NULL, shot);
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

static void moveShotFromPipe(shot_t *shot, pipe_t *pipe, list_t *listPipe)
{
	pipe_t *distPipe;

	distPipe = getPipeId(listPipe, pipe->id_out);

	if( distPipe == NULL )
	{
		fprintf(stderr, "Pipe %d ID not found\n", pipe->id);
		return;
	}

	moveShot(shot, distPipe->position, pipe->x, pipe->y,
		distPipe->x, distPipe->y, distPipe->w, distPipe->h);
}

int init(export_fce_t *p)
{
	export_fce = p;

	listPipe = newList();

	return 0;
}

#ifndef PUBLIC_SERVER

int draw()
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

	return 0;
}
#endif

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

int event()
{
	pipe_t *thisPipe;
	arena_t *arena;
	int i;

	arena = export_fce->fce_getCurrentArena();

	for( i = 0 ; i < arena->listShot->count ; i++ )
	{
		shot_t *thisShot;

		thisShot  = (shot_t *)arena->listShot->list[i];
		assert( thisShot != NULL );

		if( ( thisPipe = isConflictWithListPipe(listPipe, thisShot->x, thisShot->y,
		    thisShot->w, thisShot->h) ) != NULL )
		{
			tux_t *author;

			author = export_fce->fce_getTuxID(arena->listTux, thisShot->author_id);
			
			if( author != NULL &&
			    author->bonus == BONUS_GHOST &&
			    author->bonus_time > 0 )
			{
				continue;
			}

			if( negPosition( thisShot->position ) == thisPipe->position )
			{
				if( export_fce->fce_getNetTypeGame() == NET_GAME_TYPE_CLIENT )
				{
					if( thisShot->gun != GUN_BOMBBALL )
					{
						delListItem(arena->listShot, i, export_fce->fce_destroyShot);
						i--;
					}
				}
				else
				{
					moveShotFromPipe(thisShot, thisPipe, listPipe);
				}
			}
			else
			{
				if( thisShot->gun == GUN_BOMBBALL && 
				    export_fce->fce_getNetTypeGame() != NET_GAME_TYPE_CLIENT )
				{
					export_fce->fce_boundBombBall(thisShot);
					continue;
				}
				else
				{
					delListItem(arena->listShot, i, export_fce->fce_destroyShot);
					i--;
				}
			}
		}
	}

	return 0;
}

int isConflict(int x, int y, int w, int h)
{
	if( isConflictWithListPipe(listPipe, x, y, w, h) != NULL )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void cmd(char *line)
{
	if( strncmp(line, "pipe", 4) == 0 )cmd_teleport(line);
}

int destroy()
{
	destroyListItem(listPipe, destroyPipe);

	return 0;
}
