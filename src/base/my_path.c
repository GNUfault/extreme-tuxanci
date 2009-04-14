
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "main.h"
#include "homeDirector.h"
#include "textFile.h"
#include "path.h"
#include "my_path.h"

static char *path_prefix;

int path_init()
{
#if 0
	char str[STR_PATH_SIZE];
	textFile_t *text_file;

	sprintf(str, "%s/path", home_director_get());

	text_file = text_file_load(str);

	if( text_file == NULL )
	{
		fprintf(stderr, "I dont load path\n");
		exit(-1);
	}

	if( text_file->text->count == 0 )
	{
		fprintf(stderr, "%s is empty\n", str);
		exit(-1);
	}

	path_prefix = strdup(text_file->text->list[0]);

	text_file_destroy(text_file);

#endif

	path_prefix = malloc((strlen(PREFIX)+1)* sizeof(char));
	strcpy(path_prefix, PREFIX);
	strcat(path_prefix, "/");

	printf("use prefix:%s\n", path_prefix);

	return 0;
}

char* path_get_prefix()
{
	return path_prefix;
}

int path_set_prefix(char *path, int size, const char *subpath, const char *name)
{
	char *prefix;
	char *slash;
	int len;

	prefix = path_prefix;

/*
	printf("subpath = %s\n", subpath);
	printf("name = %s\n", name);
*/
	if( isFillPath(name) )
	{
		snprintf(path, size, "%s", name);
		//printf("%s\n", path);
		return 1;
	}

	if( isFillPath(subpath) )
	{
		snprintf(path, size, "%s", subpath);
		//printf("%s\n", path);
		return 1;
	}

	len = strlen(subpath);

	slash = "/";

	if( subpath[len-1] == '/' )
	{
		slash = "";
	}

/*
	printf("slash = %s\n", slash);
	printf("prefix = %s\n", prefix);
*/
	snprintf(path, size, "%s%s%s%s", prefix, subpath, slash, name);
	//printf("file %s\n", path);

	return 1;
}

int path_quit()
{
	if( path_prefix != NULL )
	{
		free(path_prefix);
	}

	return 0;
}
