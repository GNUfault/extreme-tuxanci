
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
#    include "hotKey.h"
#    include "world.h"
#    include "setting.h"
#    include "layer.h"
#endif

static arena_t *currentArena;
static int splitType;

#ifndef PUBLIC_SERVER
static int isBigArena(arena_t * arena)
{
	if (arena->w > WINDOW_SIZE_X || arena->h > WINDOW_SIZE_Y) {
		return 1;
	} else {
		return 0;
	}
}

static int isTuxNear(tux_t * tux1, tux_t * tux2)
{
	if (abs(tux1->x - tux2->x) < WINDOW_SIZE_X &&
	    abs(tux1->y - tux2->y) < WINDOW_SIZE_Y) {
		return 1;
	}

	return 0;
}
#endif

void setCurrentArena(arena_t * p)
{
	currentArena = p;
}

arena_t *getCurrentArena()
{
	return currentArena;
}

void hotkey_splitArena()
{
	if (splitType == SCREEN_SPLIT_VERTICAL) {
		splitType = SCREEN_SPLIT_HORIZONTAL;
	} else {
		splitType = SCREEN_SPLIT_VERTICAL;
	}
}

#ifndef PUBLIC_SERVER
void initArena()
{
	splitType = SCREEN_SPLIT_VERTICAL;

	if (getParam("--split-vertical")) {
		splitType = SCREEN_SPLIT_VERTICAL;
		printf("SCREEN_SPLIT_VERTICAL\n");
	}

	if (getParam("--split-horizontal")) {
		splitType = SCREEN_SPLIT_HORIZONTAL;
		printf("SCREEN_SPLIT_HORIZONTAL\n");
	}

	registerHotKey(SDLK_F3, hotkey_splitArena);
}

void quitArena()
{
	unregisterHotKey(SDLK_F3);
}
#endif

arena_t *newArena(int w, int h)
{
	arena_t *new;
	int zone_w, zone_h;

	new = malloc(sizeof(arena_t));

#ifndef PUBLIC_SERVER
	new->background = NULL;
	strcpy(new->music, "");
#endif

	new->w = w;
	new->h = h;

	new->listTimer = newList();

	if (w > 800 || h > 600) {
		zone_w = 320;
		zone_h = 240;
	} else {
		zone_w = 80;
		zone_h = 60;
	}

	new->spaceTux = newSpace(w, h, zone_w, zone_h, getStatusTux, setStatusTux);
	new->spaceItem = newSpace(w, h, zone_w, zone_h, getStatusItem, setStatusItem);
	new->spaceShot = newSpace(w, h, zone_w, zone_h, getStatusShot, setStatusShot);

	new->countRound = 0;
	new->max_countRound = 0;

	return new;
}

int
conflictSpace(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
	return (x1 < x2 + w2 && x2 < x1 + w1 && y1 < y2 + h2 && y2 < y1 + h1);
}

int isFreeSpace(arena_t * arena, int x, int y, int w, int h)
{
	if (x < 0 || y < 0 || x + w > arena->w || y + h > arena->h) {
		return 0;
	}

	if (isConflictWithObjectFromSpace(arena->spaceTux, x, y, w, h))
		return 0;

	if (isConflictWithObjectFromSpace(arena->spaceShot, x, y, w, h))
		return 0;

	if (isConflictWithObjectFromSpace(arena->spaceItem, x, y, w, h))
		return 0;

	if (isConflictWithObjectFromSpace(arena->spaceShot, x, y, w, h))
		return 0;

	if (isConflictModule(x, y, w, h))
		return 0;

	return 1;
}

void findFreeSpace(arena_t * arena, int *x, int *y, int w, int h)
{
	int z_x;
	int z_y;

	assert(x != NULL);
	assert(y != NULL);

	do {
		z_x = random() % (arena->w - w);
		z_y = random() % (arena->h - h);
	} while (isFreeSpace(arena, z_x, z_y, w, h) == 0);

	*x = z_x;
	*y = z_y;
}

void getCenterScreen(int *x, int *y, int x, int y, int size_x, int size_y)
{
	arena_t *arena;

	arena = getCurrentArena();

	*x = x - size_x / 2;
	*y = y - size_y / 2;

	if (*x < 0) {
		*x = 0;
	}

	if (*y < 0) {
		*y = 0;
	}

	if (*x + size_x >= arena->w) {
		*x = arena->w - size_x;
	}

	if (*y + size_y >= arena->h) {
		*y = arena->h - size_y;
	}
}

#ifndef PUBLIC_SERVER

static void drawBackground(arena_t * arena, int x, int y)
{

	if (isBigArena(arena)) {
		int i, j;

		for (i = y / arena->background->h;
		     i <= y / arena->background->h + WINDOW_SIZE_Y / arena->background->h + 1; i++) {

			for (j = x / arena->background->w;
			     j <= x / arena->background->w + WINDOW_SIZE_X / arena->background->w + 1; j++) {

				addLayer(arena->background, j * arena->background->w,
					 i * arena->background->h, 0, 0, arena->background->w,
					 arena->background->h, -100);

				//count++;
			}
		}
	} else {
		addLayer(arena->background, 0, 0, 0, 0, arena->background->w, arena->background->h, -100);
	}
}

static void action_drawTux(space_t * space, tux_t * tux, void *p)
{
	drawTux(tux);
}

static void action_drawItem(space_t * space, item_t * item, void *p)
{
	drawItem(item);
}

static void action_drawShot(space_t * space, shot_t * shot, void *p)
{
	drawShot(shot);
}

static void drawObjects(arena_t * arena, int x, int y)
{
	actionSpaceFromLocation(arena->spaceTux, action_drawTux, NULL, x, y, WINDOW_SIZE_X, WINDOW_SIZE_Y);
	actionSpaceFromLocation(arena->spaceItem, action_drawItem, NULL, x, y, WINDOW_SIZE_X, WINDOW_SIZE_Y);
	actionSpaceFromLocation(arena->spaceShot, action_drawShot, NULL, x, y, WINDOW_SIZE_X, WINDOW_SIZE_Y);

	drawModule(x, y, WINDOW_SIZE_X, WINDOW_SIZE_Y);
}

static void drawSplitArenaForTux(arena_t * arena, tux_t * tux, int location_x, int location_y)
{
	int x, y;

	switch (splitType) {
		case SCREEN_SPLIT_HORIZONTAL:
			getCenterScreen(&x, &y, tux->x, tux->y, WINDOW_SIZE_X, WINDOW_SIZE_Y / 2);
			break;
		case SCREEN_SPLIT_VERTICAL:
			getCenterScreen(&x, &y, tux->x, tux->y, WINDOW_SIZE_X / 2, WINDOW_SIZE_Y);
			break;
		default:
			x = -1;
			y = -1;
			assert(!"stupd error ");
			break;
	}

	drawBackground(arena, x, y);
	drawObjects(arena, x, y);

	switch (splitType) {
		case SCREEN_SPLIT_HORIZONTAL:
			drawLayerSplit(location_x, location_y, x, y, WINDOW_SIZE_X, WINDOW_SIZE_Y / 2);
			break;
		case SCREEN_SPLIT_VERTICAL:
			drawLayerSplit(location_x, location_y, x, y, WINDOW_SIZE_X / 2, WINDOW_SIZE_Y);
			break;
		default:
			x = -1;
			y = -1;
			assert(!"stupd error ");
			break;
	}
}

void drawSplitArena(arena_t * arena)
{
	tux_t *tux = NULL;

	tux = getControlTux(TUX_CONTROL_KEYBOARD_RIGHT);

	if (tux != NULL) {
		switch (splitType) {
			case SCREEN_SPLIT_HORIZONTAL:
				drawSplitArenaForTux(arena, tux, 0, WINDOW_SIZE_Y / 2);
				break;
			case SCREEN_SPLIT_VERTICAL:
				drawSplitArenaForTux(arena, tux, WINDOW_SIZE_X / 2, 0);
				break;
		}
	}

	tux = getControlTux(TUX_CONTROL_KEYBOARD_LEFT);

	if (tux != NULL) {
		drawSplitArenaForTux(arena, tux, 0, 0);
	}
}

void drawCenterArena(arena_t * arena, int x, int y)
{
	int x, y;

	getCenterScreen(&x, &y, x, y, WINDOW_SIZE_X, WINDOW_SIZE_Y);
	drawBackground(arena, x, y);
	drawObjects(arena, x, y);
	drawLayerCenter(x, y);
}

void drawSimpleArena(arena_t * arena)
{
	int x, y;
	tux_t *tux = NULL;

	tux = getControlTux(TUX_CONTROL_KEYBOARD_RIGHT);

	if (tux == NULL) {
		return;
	}

	x = 0;
	y = 0;

	drawBackground(arena, x, y);
	drawObjects(arena, x, y);
	drawLayer(tux->x, tux->y);
}

void drawArena(arena_t * arena)
{
	if (isBigArena(arena) &&
	    getNetTypeGame() == NET_GAME_TYPE_NONE && !isSettingAI()) {
		tux_t *tux1;
		tux_t *tux2;

		tux1 = getControlTux(TUX_CONTROL_KEYBOARD_RIGHT);
		tux2 = getControlTux(TUX_CONTROL_KEYBOARD_LEFT);

		if (isTuxNear(tux1, tux2)) {
			drawCenterArena(arena, (tux1->x + tux2->x) / 2, (tux1->y + tux2->y) / 2);
		} else {
			drawSplitArena(arena);
		}

		return;
	}

	if (isBigArena(arena)) {
		tux_t *tux = NULL;

		tux = getControlTux(TUX_CONTROL_KEYBOARD_RIGHT);

		if (tux == NULL) {
			return;
		}

		drawCenterArena(arena, tux->x, tux->y);
		return;
	}

	drawSimpleArena(arena);
}

#endif

static void action_tux(space_t * space, tux_t * tux, void *p)
{
	eventTux(tux);
}

void eventArena(arena_t * arena)
{
	int i;

	for (i = 0; i < 8; i++) {
		checkShotIsInTuxScreen(arena);

		eventConflictShotWithItem(arena);
		eventConflictTuxWithShot(arena);
		eventModule();

		eventMoveListShot(arena);
	}

	eventListItem(arena->spaceItem);

	actionSpace(arena->spaceTux, action_tux, NULL);

	eventTimer(arena->listTimer);
}

void destroyArena(arena_t * p)
{
	destroySpaceWithObject(p->spaceTux, destroyTux);
	destroySpaceWithObject(p->spaceItem, destroyItem);
	destroySpaceWithObject(p->spaceShot, destroyShot);
	destroyTimer(p->listTimer);
	free(p);
}
