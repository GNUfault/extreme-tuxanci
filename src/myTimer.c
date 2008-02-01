
#include <time.h>
#include <assert.h>
#include <stdlib.h>

#include "main.h"
#include "interface.h"
#include "list.h"
#include "myTimer.h"

static list_t *listTime;

static bool_t isTimerInit = FALSE;

bool_t isMyTimerInicialized()
{
	return isTimerInit;
}

void initTimer()
{
	assert( isInterfaceInicialized() == TRUE );

	listTime = newList();
	isTimerInit = TRUE;
}

int addTimer(void (*fce)(void *p), void *arg, int my_time)
{
	static int new_id = 0;
	my_timer_t *new;
	Uint32 currentTime;

 	currentTime = SDL_GetTicks();

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
	Uint32 currentTime;

 	currentTime = SDL_GetTicks();

	for( i = 0 ; i < listTime->count ; i++ )
	{
		thisTimer = (my_timer_t *)listTime->list[i];
		assert( thisTimer != NULL );

		if( currentTime >= thisTimer->time )
		{
			thisTimer->fce(thisTimer->arg);
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
