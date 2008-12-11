
#ifndef SCREEN_H

#    define SCREEN_H

#    include "main.h"

typedef struct struct {
	char *name;
	void (*fce_start) ();
	void (*fce_event) ();
	void (*fce_draw) ();
	void (*fce_stop) ();
} t;

extern bool_t isScreenInicialized();

extern t *newScreen(char *name,
						   void (*fce_start) (), void (*fce_event) (),
						   void (*fce_draw) (), void (*fce_stop) ());

extern void destroyScreen(t * p);
extern void registerScreen(t * p);
extern void initScreen();
extern void setScreen(char *name);
extern void switchScreen();
extern void startScreen(char *name);
extern char *getScreen();
extern void drawScreen();
extern void eventScreen();
extern void quitScreen();

#endif
