
#ifndef PAUZA_H

#    define PAUZA_H

#    define PAUZE_ACTIVE_TIME_INTERVAL	1000

#    include "main.h"

extern void initPauza();
extern void drawPauza();
extern void eventPauza();
extern bool_t isPauzeActive();
extern void quitPauza();

#endif
