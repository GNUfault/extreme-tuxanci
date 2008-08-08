
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>

#include "main.h"
#include "list.h"
#include "homeDirector.h"
#include "textFile.h"

#include "configFile.h"
#include "language.h"
#include "director.h"

static textFile_t *languageFile;
static char fontFile[STR_FILE_NAME_SIZE];
static int fontSize;

static char* getHead(textFile_t *ts)
{
	int i;

	for( i = 0 ; i < ts->text->count ; i++ )
	{
		char *line;
		line = (char *) (ts->text->list[i]);

		if( strncmp(line, "HEAD", 4) == 0 )
		{
			return line;
		}
	}

	return NULL;
}

static char* getSupportLanguage()
{
	director_t *director;
	char *envLang;
	char lang[STR_SIZE];
	int i;

	printf("I search language..\n");

	envLang = getenv("LANG");

	if( envLang == NULL )
	{
		printf("Environment LANG not defined, I use default value \"en\"\n");
		return strdup("en");
	}

	memset(lang, 0, STR_SIZE);
	strncpy(lang, envLang, 2);

	director = loadDirector(PATH_LANG);

	if( director == NULL )
	{
		fprintf(stderr, "Do not open director %s\n", PATH_LANG);
		return strdup("en");
	}

	for( i = 0 ; i < director->list->count ; i++ )
	{
		char *line;

		line = (char *)director->list->list[i];

		if( strncmp(lang, line, 2) == 0 )
		{
			printf("I choice lang %s in file %s\n", lang, line);
			return strdup(lang);
		}
	}

	destroyDirector(director);

	printf("Lang %s not support, I use default language english\n", lang);

	return strdup("en");
}
 
int initLanguage()
{
	textFile_t *languageTypeFile;
	char path[STR_PATH_SIZE];
	char *lang;
	char *head;

	sprintf(path, "%s/lang.conf", getHomeDirector());
	printf("open %s\n", path);
	languageTypeFile = loadTextFile(path);

	if( languageTypeFile == NULL )
	{
		char *defaultLang;

		fprintf(stderr, "Don't load %s\n", path);
		fprintf(stderr, "I create %s\n", path);

		languageTypeFile = newTextFile(path);
		defaultLang = getSupportLanguage();
		addList(languageTypeFile->text, strdup(defaultLang) );
		saveTextFile(languageTypeFile);
		free(defaultLang);
	}

	if( languageTypeFile == NULL )
	{
		fprintf(stderr, "Don't create %s\n", path);
		return -1;
	}

	if( languageTypeFile->text->count == 0 )
	{
		fprintf(stderr, "File %s empty\n", path);
		return -1;
	}

	lang = ((char *)languageTypeFile->text->list[0]);
	printf("select lang %s\n", lang);

	sprintf(path, PATH_LANG "%s.lang", lang);

	destroyTextFile(languageTypeFile);
	
	accessExistFile(path);
	languageFile = loadTextFile(path);

	if( languageFile == NULL )
	{
		fprintf(stderr, "Don't load %s\n", path);
		return -1;
	}

	head = getHead(languageFile);

	if( head != NULL )
	{
		char val[STR_SIZE];
	
		if( getValue(head, "fontfile", val, STR_SIZE) == 0 )
		{
			strcpy(fontFile, val);
		}
		else
		{
			fprintf(stderr, "fontfile in lang file %s not found\n", path);
			return -1;
		}

		if( getValue(head, "fontsize", val, STR_SIZE) == 0 )
		{
			fontSize = atoi(val);
		}
		else
		{
			fprintf(stderr, "fontsize in lang file %s not found\n", path);
			return -1;
		}
	}
	else
	{
		fprintf(stderr, "head in lang file %s not found\n", path);
		return -1;
	}

	return 0;
}

char* getMyText(char *key)
{
	int i;
	int len;

	len = strlen(key);

	for( i = 0 ; i < languageFile->text->count ; i++ )
	{
		char *line;
		line = (char *) (languageFile->text->list[i]);

		if( strncmp(key, line, len) == 0 )
		{
			return line+strlen(key)+1;
		}
	}

	return NULL;
}

char* getLanguageFont()
{
	return fontFile;
}

int getLanguageSize()
{
	return fontSize;
}

void quitLanguage()
{
	destroyTextFile(languageFile);
}

