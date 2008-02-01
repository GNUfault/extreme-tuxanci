
#ifndef MY_TIME_H

#define MY_TIME_H

#include "interface.h"
#include "main.h"

typedef struct my_timer_struct
{
	unsigned int id;
	void (*fce)(void *p);
	void *arg;
	Uint32 time;
} my_timer_t;

extern bool_t isMyTimerInit();
extern void initTimer();
extern int addTimer(void (*fce)(void *p), void *arg, int my_time);
extern void eventTimer();
extern void delTimer(int id);
extern void quitTimer();

#endif
