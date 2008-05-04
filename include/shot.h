
#ifndef SHOT_H

#define SHOT_H

#include "main.h"
#ifndef PUBLIC_SERVER	
#include "interface.h"
#endif	
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

	int position;
	int gun;

	int author_id;
	bool_t isCanKillAuthor;

#ifndef PUBLIC_SERVER	
	SDL_Surface *img;
#endif	
} shot_t; 

extern bool_t isShotInicialized();
extern void initShot();
extern shot_t* newShot(int x,int y, int px, int py, int gun, int author_id);
extern shot_t* getShotID(list_t *listShot, int id);
#ifndef PUBLIC_SERVER	
extern void drawShot(shot_t *p);
extern void drawListShot(list_t *listShot);
#endif
extern int isConflictWithListShot(list_t *listShot, int x, int y, int w, int h);
extern void eventMoveListShot(list_t *listShot);

extern void transformOnlyLasser(shot_t *shot);
extern void boundBombBall(shot_t *shot);

extern void moveShot(shot_t *shot, int position, int src_x, int src_y,
	int dist_x, int dist_y, int dist_w, int dist_h);

extern void destroyShot(shot_t *p);
extern void quitShot();

#endif

