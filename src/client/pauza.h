#ifndef PAUZA_H
#define PAUZA_H

#include "main.h"

#define PAUZE_ACTIVE_TIME_INTERVAL	1000

extern void pauza_init();
extern void pauza_draw();
extern void pauza_event();
extern bool_t pauza_is_active();
extern void pauza_quit();

#endif /* PAUZA_H */
