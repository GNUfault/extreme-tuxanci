
#ifndef TERM_H

#define TERM_H

#include "main.h"

#define TERM_ACTIVE_TIME_INTERVAL		500
#define TERM_REFRESH_TIME_INTERVAL		1000

extern void initTerm();
extern void drawTerm();
extern void eventTerm();
extern void quitTerm();

#endif
