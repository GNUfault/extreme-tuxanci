
#ifndef PIPE_H

#define PIPE_H

#include "interface.h"
#include "list.h"

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

	SDL_Surface *img; //obrazok
} pipe_t;

extern pipe_t* newPipe(int x, int y, int w, int h, int layer,
		int id, int id_out, int position, SDL_Surface *img);

extern void drawPipe(pipe_t *p);
extern void drawListPipe(list_t *listPipe);
extern pipe_t* isConflictWithListPipe(list_t *listPipe, int x, int y, int w, int h);
extern void eventConflictShotWithPipe(list_t *listPipe, list_t *listShot);
extern void destroyPipe(pipe_t *p);

#endif

