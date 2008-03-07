
#ifndef TELEPORT_H

#define TELEPORT_H

#include <interface.h>
#include "list.h"

typedef struct teleport_struct
{
	int x; // poloha steny	
	int y;

	int w; // rozmery steny
	int h;

	int layer; // vrstva

	SDL_Surface *img; //obrazok
} teleport_t;

extern teleport_t* newTeleport(int x, int y, int w, int h, int layer, SDL_Surface *img);
extern void drawTeleport(teleport_t *p);
extern void drawListTeleport(list_t *listTeleport);
extern teleport_t* isConflictWithListTeleport(list_t *listTeleport, int x, int y, int w, int h);
extern void eventConflictShotWithTeleport(list_t *listTeleport, list_t *listShot);
extern void eventTeleportTux(list_t *listTeleport, teleport_t *teleport, tux_t *tux);
extern void destroyTeleport(teleport_t *p);

#endif

