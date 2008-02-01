
#ifndef SHOT_H

#include "main.h"
#include "interface.h"
#include "list.h"
#include "tux.h"

typedef struct shot_struct
{
	int id;

	int x;
	int y;

	int w;
	int h;

	int px;
	int py;

	int course;
	int speed;
	int gun;

	tux_t *author;
	bool_t isCanKillAuthor;

	SDL_Surface *img;
} shot_t; 

extern bool_t isShotInicialized();
extern void initShot();
extern shot_t* newShot(int x,int y, int px, int py, int gun, tux_t *author);
extern void drawShot(shot_t *p);
extern void drawListShot(list_t *listShot);
extern int isConflictWithListShot(list_t *listShot, int x, int y, int w, int h);
extern void eventMoveListShot(list_t *listShot);
extern void destroyShot(shot_t *p);
extern void quitShot();

#endif

