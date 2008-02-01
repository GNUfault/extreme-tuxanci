
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "configFile.h"

char* getValueInConfigFile(textFile_t *p, char *s)
{
	int i;

	assert( s != NULL );
	assert( p != NULL );

	for( i = 0 ; i < p->text->count ; i++ )
	{
		char *line;
		char *offset;

		line = (char *) (p->text->list[i]);

		if( ( offset = strstr(line, s) ) != NULL )
		{
			return offset + ( strlen(s) + 1 );
		}
	}

	return NULL;
}

void delRemInConfigFile(textFile_t *p)
{
	int i;

	assert( p != NULL );

	for( i = 0 ; i < p->text->count ; i++ )
	{
		char *line;
		char *offset;

		line = (char *) (p->text->list[i]);

		if( ( offset = strchr(line, '#') ) != NULL )
		{
			*offset = '\0';
		}
	}
}
