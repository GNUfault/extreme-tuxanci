
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

typedef struct teleport_struct
{
	int x; // poloha steny	
	int y;

	int w; // rozmery steny
	int h;

	int layer; // vrstva

#ifndef PUBLIC_SERVER	
	SDL_Surface *img; //obrazok
#endif
} teleport_t;

static export_fce_t *export_fce;

static list_t *listTeleport;

#ifndef PUBLIC_SERVER	
teleport_t* newTeleport(int x, int y, int w, int h, int layer, SDL_Surface *img)
#endif

#ifdef PUBLIC_SERVER	
teleport_t* newTeleport(int x, int y, int w, int h, int layer)
#endif
{
	teleport_t *new;
	
#ifndef PUBLIC_SERVER	
	assert( img != NULL );
#endif

	new  = malloc( sizeof(teleport_t) );
	assert( new != NULL );

	new->x = x;
	new->y = y;
	new->w = w;
	new->h = h;
	new->layer = layer;
#ifndef PUBLIC_SERVER	
	new->img = img;
#endif

	return new;
}

#ifndef PUBLIC_SERVER

static void drawTeleport(teleport_t *p)
{
	assert( p != NULL );

	export_fce->fce_addLayer(p->img, p->x, p->y, 0, 0, p->img->w, p->img->h, p->layer);
}

#endif

static teleport_t* isConflictWithListTeleport(list_t *list, int x, int y, int w, int h)
{
	teleport_t *thisTeleport;
	int i;

	assert( list != NULL );

	for( i = 0 ; i < list->count ; i++ )
	{
		thisTeleport  = (teleport_t *)list->list[i];
		assert( thisTeleport != NULL );

		if( export_fce->fce_conflictSpace(x, y, w, h,
		    thisTeleport->x, thisTeleport->y, thisTeleport->w, thisTeleport->h) )
		{
			return thisTeleport;
		}
	}

	return NULL;
}

static void destroyTeleport(teleport_t *p)
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
	char str_layer[STR_NUM_SIZE];
	char str_image[STR_SIZE];
	teleport_t *new;

	if( export_fce->fce_getValue(line, "x", str_x, STR_NUM_SIZE) != 0 )return;
	if( export_fce->fce_getValue(line, "y", str_y, STR_NUM_SIZE) != 0 )return;
	if( export_fce->fce_getValue(line, "w", str_w, STR_NUM_SIZE) != 0 )return;
	if( export_fce->fce_getValue(line, "h", str_h, STR_NUM_SIZE) != 0 )return;
	if( export_fce->fce_getValue(line, "layer", str_layer, STR_NUM_SIZE) != 0 )return;
	if( export_fce->fce_getValue(line, "image", str_image, STR_SIZE) != 0 )return;

#ifndef PUBLIC_SERVER
	new = newTeleport(atoi(str_x), atoi(str_y),
			atoi(str_w), atoi(str_h),
			atoi(str_layer), export_fce->fce_getImage(IMAGE_GROUP_USER, str_image) );
#endif

#ifdef PUBLIC_SERVER
	new = newTeleport(atoi(str_x), atoi(str_y),
			atoi(str_w), atoi(str_h),
			atoi(str_layer) );
#endif

	addList(listTeleport, new);
}

static teleport_t* getRandomTeleportBut(list_t *list, teleport_t *teleport)
{
	int x;

	do{
		x = random() % list->count;
	}while( list->list[x] == teleport );

	return (teleport_t *) list->list[x];
}

static int getRandomPosition()
{
	switch( random() % 4 )
	{
		case 0 :
		return TUX_UP;

		case 1 :
		return TUX_LEFT;

		case 2 :
		return TUX_RIGHT;

		case 3 :
		return TUX_DOWN;
	}

	assert( ! "Stupid error ! " );
}

static void teleportTux(tux_t *tux, teleport_t *teleport)
{
	teleport_t *distTeleport;
	int current_x, current_y;
	int dist_x, dist_y;

	if( tux->bonus == BONUS_GHOST ||
	    export_fce->fce_getNetTypeGame() == NET_GAME_TYPE_CLIENT )
	{
		return;
	}

	distTeleport = getRandomTeleportBut(listTeleport, teleport);
	
	export_fce->fce_getTuxProportion(tux, &current_x, &current_y, NULL, NULL);
	
	switch( tux->position )
	{
		case TUX_UP :
			dist_x = distTeleport->x + distTeleport->w/2;
			dist_y = distTeleport->y - ( TUX_HEIGHT + 20 );
		break;
		case TUX_LEFT :
			dist_x = distTeleport->x - ( TUX_WIDTH + 20 );
			dist_y = distTeleport->y + distTeleport->h/2;
		break;
	
		case TUX_RIGHT :
			dist_x = distTeleport->x + distTeleport->w + 20;
			dist_y = distTeleport->y + distTeleport->h/2;
		break;
	
		case TUX_DOWN :
			dist_x = distTeleport->x + distTeleport->w/2;
			dist_y = distTeleport->y + distTeleport->h + 20;
		break;
	
		default :
			assert( ! "premenna p->control ma zlu hodnotu !" );
		break;
	}
	
	if( export_fce->fce_isFreeSpace(export_fce->fce_getCurrentArena(), dist_x, dist_y, TUX_WIDTH, TUX_HEIGHT) )
	{
		export_fce->fce_setTuxProportion(tux, dist_x, dist_y);
#ifndef PUBLIC_SERVER
		//playSound("teleport", SOUND_GROUP_BASE);
#endif
		if( export_fce->fce_getNetTypeGame() == NET_GAME_TYPE_SERVER )
		{
			export_fce->fce_proto_send_newtux_server(PROTO_SEND_ALL, NULL, tux);
		}
	}
}

static int myAbs(int n)
{
	return ( n > 0 ? n : -n );
}


static int getSppedShot(shot_t *shot)
{
	return ( myAbs(shot->px) > myAbs(shot->py) ? myAbs(shot->px) : myAbs(shot->py) );
}

/*
static void transformOnlyLasser(shot_t *shot)
{
	switch( shot->position )
	{
		case TUX_RIGHT :
		case TUX_LEFT :
			shot->w = GUN_LASSER_HORIZONTAL; 
			shot->h = GUN_SHOT_VERTICAL; 
#ifndef PUBLIC_SERVER	
			shot->img = g_shot_lasserX;
#endif
		break;
		case TUX_UP :
		case TUX_DOWN :
			shot->w = GUN_SHOT_VERTICAL; 
			shot->h = GUN_LASSER_HORIZONTAL; 
#ifndef PUBLIC_SERVER	
			shot->img = g_shot_lasserY;
#endif
		break;
	}
}
*/

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

/*
	if( shot->gun == GUN_LASSER )
	{
		transformOnlyLasser(shot);
	}
*/
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

static void moveShotFromTeleport(shot_t *shot, teleport_t *teleport, list_t *list)
{
	teleport_t *distTeleport;

	distTeleport = getRandomTeleportBut(list, teleport);

	moveShot(shot, getRandomPosition(), teleport->x, teleport->y,
		distTeleport->x, distTeleport->y, distTeleport->w, distTeleport->h);
}

int init(export_fce_t *p)
{
	export_fce = p;

	listTeleport = newList();

	return 0;
}

#ifndef PUBLIC_SERVER
int draw()
{
	teleport_t *thisTeleport;
	int i;

	assert( listTeleport != NULL );

	for( i = 0 ; i < listTeleport->count ; i++ )
	{
		thisTeleport  = (teleport_t *)listTeleport->list[i];
		assert( thisTeleport != NULL );
		drawTeleport(thisTeleport);
	}

	return 0;
}
#endif

int event()
{
	teleport_t *thisTeleport;
	arena_t *arena;
	int i;

	if( export_fce->fce_getNetTypeGame() == NET_GAME_TYPE_CLIENT )
	{
		return 0;
	}

	arena = export_fce->fce_getCurrentArena();

	for( i = 0 ; i < arena->listTux->count ; i++ )
	{
		tux_t *thisTux;
		int x, y, w, h;

		thisTux = (tux_t *)arena->listTux->list[i];
		export_fce->fce_getTuxProportion(thisTux, &x, &y, &w, &h);

		if( ( thisTeleport = isConflictWithListTeleport(listTeleport, x, y, w, h) ) != NULL )
		{
			teleportTux(thisTux, thisTeleport);
		}
	}
	
	for( i = 0 ; i < arena->listShot->count ; i++ )
	{
		shot_t *thisShot;
	
		thisShot  = (shot_t *)arena->listShot->list[i];
		assert( thisShot != NULL );

		if( thisShot->gun == GUN_LASSER )
		{
			continue;
		}

		if( ( thisTeleport = isConflictWithListTeleport(listTeleport,
			thisShot->x, thisShot->y, thisShot->w, thisShot->h) ) != NULL )
		{
			tux_t *author;

			author = export_fce->fce_getTuxID(arena->listTux, thisShot->author_id);
			
			if( author != NULL && 
			    author->bonus == BONUS_GHOST &&
			    author->bonus_time > 0 )
			{
				continue;
			}

			moveShotFromTeleport(thisShot, thisTeleport, listTeleport);
/*			
			if( export_fce->fce_getNetTypeGame() == NET_GAME_TYPE_CLIENT )
			{
				delListItem(arena->listShot, i, export_fce->fce_destroyShot);
				i--;
			}
			else
			{
				moveShotFromTeleport(thisShot, thisTeleport, listTeleport);
			}
*/
		}
	}

	return 0;
}

int isConflict(int x, int y, int w, int h)
{
	return 0;
}


void eventConflictWithTux(tux_t *tux)
{
}

void eventConflictWithShot(shot_t *tux)
{
}

void cmd(char *line)
{
	if( strncmp(line, "teleport", 8) == 0 )cmd_teleport(line);
}

int destroy()
{
	destroyListItem(listTeleport, destroyTeleport);
	return 0;
}
