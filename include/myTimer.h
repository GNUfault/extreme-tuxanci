
#ifndef MY_TIME_H

#define MY_TIME_H

#include "main.h"

#define my_time_t unsigned int

typedef struct my_timer_struct
{
	unsigned int id;
	void (*fce)(void *p);
	void *arg;
	my_time_t time;
} my_timer_t;

extern bool_t isMyTimerInit();
extern void initTimer();
extern my_time_t getMyTime();
extern int addTimer(void (*fce)(void *p), void *arg, my_time_t my_time);
extern void eventTimer();
extern void delTimer(int id);
extern void quitTimer();

#endif
