
#include <time.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>

#include "main.h"
#include "list.h"
#include "myTimer.h"

#ifndef BUBLIC_SERVER
#include "interface.h"
#endif

static list_t *listTime;

static bool_t isTimerInit = FALSE;

bool_t isMyTimerInicialized()
{
	return isTimerInit;
}

void initTimer()
{
	listTime = newList();
	isTimerInit = TRUE;
}

my_time_t getMyTime()
{
	static struct timeval start;
	struct timeval now;
	static char first = '0';
	my_time_t ticks;

	if(  first == '0' )
	{
		gettimeofday(&start, NULL);
		first = 'x';
	}

	gettimeofday(&now, NULL);

	ticks = (now.tv_sec-start.tv_sec)*1000+(now.tv_usec-start.tv_usec)/1000;

	return ticks;
}

int addTimer(void (*fce)(void *p), void *arg, my_time_t my_time)
{
	static int new_id = 0;
	my_timer_t *new;
	my_time_t currentTime;

 	currentTime = getMyTime();

	new = malloc( sizeof(my_timer_t) );
	assert( new != NULL );

	new->id = new_id++;
	new->fce = fce;
	new->arg = arg;
	new->time = currentTime + my_time;

	addList(listTime, new);

	return new->id;
}

void eventTimer()
{
	int i;
	my_timer_t *thisTimer;
	my_time_t currentTime;

 	currentTime = getMyTime();

	for( i = 0 ; i < listTime->count ; i++ )
	{
		thisTimer = (my_timer_t *)listTime->list[i];
		assert( thisTimer != NULL );

		if( currentTime >= thisTimer->time )
		{
			thisTimer->fce(thisTimer->arg);

			if( isTimerInit == FALSE )
			{
				return;
			}

			delListItem(listTime, i, free);
			i--;
		}
	}
}

void delTimer(int id)
{
	int i;
	my_timer_t *thisTimer;

	for( i = 0 ; i < listTime->count ; i++ )
	{
		thisTimer = (my_timer_t *)listTime->list[i];
		assert( thisTimer != NULL );

		if( thisTimer->id == id )
		{
			delListItem(listTime, i, free);
			return;
		}
	}

	assert( ! "Uloha s id nenajdena !" );
}

void delAllItemTimer()
{
	quitTimer();
	initTimer();
}

void quitTimer()
{
	destroyListItem(listTime, free);
	isTimerInit = FALSE;
}
