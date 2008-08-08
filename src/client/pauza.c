
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "main.h"
#include "list.h"
#include "myTimer.h"

#include "interface.h"
#include "image.h"
#include "pauza.h"

static image_t *g_pauza;
static bool_t activePauza;
static my_time_t lastActive;

void initPauza()
{
	g_pauza = addImageData("pauza.png", IMAGE_ALPHA, "pauza", IMAGE_GROUP_USER);
	activePauza = FALSE;
	lastActive = getMyTime();
}

static void switchPauzed()
{
	if( activePauza == TRUE )
	{
		activePauza = FALSE;
	}
	else
	{
		activePauza = TRUE;
	}
}

void drawPauza()
{
	if( activePauza )
	{
		drawImage(g_pauza,
			WINDOW_SIZE_X/2 - g_pauza->w/2,
			WINDOW_SIZE_Y/2 - g_pauza->h/2,
			0, 0,
			g_pauza->w, g_pauza->h);
	}
}

void eventPauza()
{
	Uint8 *mapa;
	mapa = SDL_GetKeyState(NULL);

	if( mapa[SDLK_p] == SDL_PRESSED )
	{
		my_time_t currentTime;

		currentTime = getMyTime();

		if( currentTime - lastActive > PAUZE_ACTIVE_TIME_INTERVAL )
		{
			lastActive = currentTime;
			switchPauzed();
		}
	}
}

bool_t isPauzeActive()
{
	return activePauza;
}

void quitPauza()
{
}

