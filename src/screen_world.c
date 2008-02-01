
#include <stdio.h>
#include <assert.h>

#include "main.h"
#include "interface.h"

#include "screen.h"
#include "screen_world.h"
#include "screen_analyze.h"
#include "screen_setting.h"
#include "screen_gameType.h"

#include "list.h"
#include "layer.h"
#include "image.h"
#include "music.h"
#include "tux.h"
#include "wall.h"
#include "item.h"
#include "shot.h"
#include "gun.h"
#include "panel.h"
#include "myTimer.h"
#include "net_multiplayer.h"
#include "arena.h"
#include "arenaFile.h"

static arena_t *arena;
static int count;
static int max_count;

static bool_t isScreenWorldInit = FALSE;

bool_t isScreenWorldInicialized()
{
	return isScreenWorldInit;
}

static void setGameType()
{
	if( getSettingGameType() == NET_GAME_TYPE_SERVER )
	{
		initServer( getSettingPort() );
	}

	if( getSettingGameType() == NET_GAME_TYPE_CLIENT )
	{
		initClient( getSettingPort() , getSettingIP() );
	}
}

arena_t* getWorldArena()
{
	return arena;
}

void countRoundInc()
{
	count++;
}

void createTux()
{
	tux_t *tux;

	switch( getNetTypeGame() )
	{
		case NET_GAME_TYPE_NONE :
			tux = newTux();
			tux->control = TUX_CONTROL_KEYBOARD_RIGHT;
			getSettingNameRight(tux->name);
			addList(arena->listTux, tux);
		
			tux = newTux();
			tux->control = TUX_CONTROL_KEYBOARD_LEFT;
			getSettingNameLeft(tux->name);
			addList(arena->listTux, tux);
		break;

		case NET_GAME_TYPE_SERVER :
			tux = newTux();
			tux->control = TUX_CONTROL_KEYBOARD_RIGHT;
			getSettingNameRight(tux->name);
			addList(arena->listTux, tux);
		break;

		case NET_GAME_TYPE_CLIENT :
		break;
	}
}

void drawWorld()
{
	drawImage(arena->background, 0, 0, 0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y);

	drawListTux(arena->listTux);
	drawListWall(arena->listWall);
	drawListShot(arena->listShot);
	drawListItem(arena->listItem);

	drawLayer();

	drawPanel(arena->listTux);
}

int conflictSpace(int x1,int y1,int w1,int h1,int x2,int y2,int w2,int h2)
{
	return (x1<x2+w2 && x2<x1+w1 && y1<y2+h2 && y2<y1+h1);
}

int isFreeSpace(int x, int y, int w, int h)
{
	if ( isConflictWithListTux(arena->listTux, x, y, w, h) )return 0;
	if ( isConflictWithListWall(arena->listWall, x, y, w, h) )return 0;
	if ( isConflictWithListShot(arena->listShot, x, y, w, h) )return 0;
	if ( isConflictWithListItem(arena->listItem, x, y, w, h) )return 0;

	return 1;
}

void findFreeSpace(int *x, int *y, int w, int h)
{
	int z_x;
	int z_y;

	assert( x != NULL );
	assert( y != NULL );

	do{
		z_x = random() % WINDOW_SIZE_X;
		z_y = random() % (WINDOW_SIZE_Y-200);
	}while( isFreeSpace(z_x, z_y ,w ,h) == 0 );

	*x = z_x;
	*y = z_y;
}

static void control_keyboard_right(tux_t *p)
{
	Uint8 *mapa;
	mapa = SDL_GetKeyState(NULL);

	assert( p != NULL );
	assert( mapa != NULL );

	if( mapa[(SDLKey)SDLK_UP] == SDL_PRESSED )moveTux(p, TUX_UP);
	if( mapa[(SDLKey)SDLK_RIGHT] == SDL_PRESSED )moveTux(p, TUX_RIGHT);
	if( mapa[(SDLKey)SDLK_LEFT] == SDL_PRESSED )moveTux(p, TUX_LEFT);
	if( mapa[(SDLKey)SDLK_DOWN] == SDL_PRESSED )moveTux(p, TUX_DOWN);

	if( mapa[(SDLKey)SDLK_KP0] == SDL_PRESSED )
	{
		proto_send_shottux(p);
		shotTux(p);
	}

	if( mapa[(SDLKey)SDLK_KP1] == SDL_PRESSED )
	{
		proto_send_switchgun(p);
		switchTuxGun(p);
	}
}

static void control_keyboard_left(tux_t *p)
{
	Uint8 *mapa;
	mapa = SDL_GetKeyState(NULL);

	assert( p != NULL );
	assert( mapa != NULL );

	if( mapa[(SDLKey)SDLK_w] == SDL_PRESSED )moveTux(p, TUX_UP);
	if( mapa[(SDLKey)SDLK_d] == SDL_PRESSED )moveTux(p, TUX_RIGHT);
	if( mapa[(SDLKey)SDLK_a] == SDL_PRESSED )moveTux(p, TUX_LEFT);
	if( mapa[(SDLKey)SDLK_s] == SDL_PRESSED )moveTux(p, TUX_DOWN);
}

static void eventEnd()
{
	Uint8 *mapa;
	mapa = SDL_GetKeyState(NULL);

	if( mapa[(SDLKey)SDLK_ESCAPE] == SDL_PRESSED )
	{
		setScreen("analyze");
		return;
	}

	if( count >= max_count && getNetTypeGame() != NET_GAME_TYPE_CLIENT )
	{
		setScreen("analyze");
		return;
	}	
}

void tuxControl(tux_t *p)
{
	assert( p != NULL );

	if( p->status != TUX_STATUS_ALIVE )
	{
		return;
	}

	switch( p->control )
	{
		case TUX_CONTROL_NONE :
			assert( ! "Tux nema definovane ovladanie !" );
		break;

		case TUX_CONTROL_KEYBOARD_LEFT :
			control_keyboard_left(p);
		break;

		case TUX_CONTROL_KEYBOARD_RIGHT :
			control_keyboard_right(p);
		break;
	}
}

void eventWorld()
{
	eventListTux(arena->listTux);

	eventConflictShotWithTux(arena->listTux, arena->listShot);
	eventConflictShotWithWall(arena->listWall, arena->listShot);
	eventMoveListShot(arena->listShot);
	eventListItem(arena->listItem);
	eventConflictShotWithItem(arena->listItem, arena->listShot);
	eventTimer();

	eventNetMultiplayer();

	eventEnd();
}

void startWorld()
{
	arena = getArena(0);
	count = 0;
	getSettingCountRound(&max_count);

	netSetArena(arena);
	setGameType();
	createTux();
	addNewItem(arena->listItem);
	playMusic(arena->music, MUSIC_GROUP_USER);
	initTimer();
}

static void setAnalyze()
{
	int i;
	tux_t *tux;

	restartAnalyze();

	for( i = 0 ; i < arena->listTux->count ; i++ )
	{
		tux = (tux_t *)(arena->listTux->list[i]);
		addAnalyze(tux->name, tux->score);
	}

	endAnalyze();
}

void stoptWorld()
{
	setAnalyze();
	delAllImageInGroup(IMAGE_GROUP_USER);
	quitNetMultiplayer();
	destroyArena(arena);
	stopMusic();
	delAllMusicInGroup(MUSIC_GROUP_USER);
	quitTimer();
}

void initWorld()
{
	assert( isImageInicialized() == TRUE );
	assert( isTuxInicialized() == TRUE );
	assert( isShotInicialized() == TRUE );
	assert( isPanelInicialized() == TRUE );
	assert( isScreenInicialized() == TRUE );

	registerScreen( newScreen("world", startWorld, eventWorld, drawWorld, stoptWorld) );

	isScreenWorldInit = TRUE;
}

void quitWorld()
{
	printf("quit world\n");
	isScreenWorldInit = FALSE;
}

