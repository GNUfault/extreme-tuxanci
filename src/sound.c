
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "main.h"
#include "interface.h"
#include "list.h"
#include "path.h"
#include "string_length.h"
#include "audio.h"
#include "sound.h"

static list_t *listSound;

static bool_t isSoundInit = FALSE;

bool_t isSoundInicialized()
{
	return isSoundInit;
}

void initSound()
{
	assert( isAudioInicialized() );

	listSound = newList();
	isSoundInit = TRUE;

	printf("init sound..\n");
}

static Mix_Chunk* loadMixSound(char *file)
{
	Mix_Chunk *new;
	char str[STR_PATH_SIZE];

	sprintf(str, PATH_SOUND "%s", file);
	new = Mix_LoadWAV(str);

	if( new == NULL )
	{
		fprintf(stderr, "Nelze nahrat zvuk %s : %s\n", str, Mix_GetError());
		return NULL;
	}

	return new;
}

static sound_t *newSound(char *file, char *name, int group)
{
	sound_t *new;

	new = malloc( sizeof(sound_t) );
	new->name = strdup(name);
	new->group = group;
	new->sound = loadMixSound(file);

	return new;
}

static void playMixSound(sound_t *p)
{
	if( Mix_PlayChannel(-1, p->sound, 0) == -1 )
	{
		fprintf(stderr, "Nelze prehrat zvuk : %s\n", Mix_GetError());
		return;
	}
}

static void destroySound(sound_t *p)
{
	free(p->name);
	Mix_FreeChunk(p->sound);
	free(p);
}

void addSound(char *file, char *name, int group)
{
	addList( listSound, newSound(file, name, group) );
}

void playSound(char *name, int group)
{
	int i;
	sound_t *this;

	for( i = 0 ; i < listSound->count ; i++ )
	{
		this = (sound_t *)listSound->list[i];

		if( this->group == group && strcmp(this->name, name) == 0 )
		{
			playMixSound(this);
			return;
		}
	}
}

void quitSound()
{
	destroyListItem(listSound, destroySound);
	isSoundInit = FALSE;
	printf("quit sound..\n");
}

