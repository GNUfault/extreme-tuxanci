
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "main.h"
#include "interface.h"
#include "list.h"
#include "path.h"
#include "string_length.h"
#include "audio.h"
#include "music.h"

static list_t *listMusic;

static bool_t isMusicInit = FALSE;
static music_t *currentMusic;

bool_t isMusicInicialized()
{
	return isMusicInit;
}

void initMusic()
{
	assert( isAudioInicialized() );

	listMusic = newList();
	currentMusic = NULL;

	isMusicInit = TRUE;
	printf("init music..\n");
}

static Mix_Music* loadMixMusic(char *file)
{
	Mix_Music *new;
	char str[STR_PATH_SIZE];

	printf("load music %s..\n", file);
	sprintf(str, PATH_MUSIC "%s", file);

	new = Mix_LoadMUS(str);

	if( new == NULL )
	{
		fprintf(stderr, "Nepodarilo sa nacitat hudbu %s\n", file);
		return NULL;
	}

	return new;
}

static music_t *newMusic(char *file, char *name, int group)
{
	music_t *new;

	new = malloc( sizeof(music_t) );
	new->name = strdup(name);
	new->group = group;
	new->music = loadMixMusic(file);

	return new;
}

static void playMixMusic()
{
	printf("play music %s..\n", currentMusic->name);
	Mix_PlayMusic(currentMusic->music, 1000);
}

static void destroyMusic(music_t *p)
{
	free(p->name);
	Mix_FreeMusic(p->music);
	free(p);
}

void addMusic(char *file, char *name, int group)
{
	addList( listMusic, newMusic(file, name, group) );
}

void stopMusic()
{
	if( currentMusic != NULL )
	{
		printf("stop music %s..\n", currentMusic->name);
		Mix_HaltMusic();
		currentMusic = NULL;
	}
}

void playMusic(char *name, int group)
{
	int i;
	music_t *this;

	if( currentMusic != NULL &&
	    currentMusic->group == group &&
	    strcmp(currentMusic->name, name) == 0 )
	{
		return;
	}

	if( currentMusic != NULL )
	{
		stopMusic();
	}

	for( i = 0 ; i < listMusic->count ; i++ )
	{
		this = (music_t *)listMusic->list[i];

		if( this->group == group && strcmp(this->name, name) == 0 )
		{
			currentMusic = this;
			break;
		}
	}

	if( currentMusic != NULL )
	{
		playMixMusic();
	}
}

char* getCurrentMusic()
{
	return currentMusic->name;
}

void delAllMusicInGroup(int group)
{
	int i;
	music_t *this;

	for( i = 0 ; i < listMusic->count ; i++ )
	{
		this = (music_t *)listMusic->list[i];

		if( this->group == group )
		{
			delListItem(listMusic, i, destroyMusic);
			break;
		}
	}
}

void quitMusic()
{
	stopMusic();
	destroyListItem(listMusic, destroyMusic);
	isMusicInit = FALSE;

	printf("quit music..\n");
}

