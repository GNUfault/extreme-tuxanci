#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "main.h"
#include "list.h"
#include "textFile.h"
#include "serverConfigFile.h"

static textFile_t *serverTextFile;

static void prepareConfigFile(textFile_t *ts)
{
	int i, j;

	for (i = 0; i < serverTextFile->text->count; i++) {
		char *line;
		int len;

		line = (char *) (serverTextFile->text->list[i]);
		len = strlen(line);

		for (j = 0; j < len; j++) {
			if (line[j] == '	') {	/* [TAB] */
				line[j] = ' ';
			}
		}
	}
}

void server_configFile_init()
{
	char *configFile;

	configFile = getParamElse("--config-file", SERVER_CONFIG);
	debug("Loading configuration file [%s]", getParamElse("--config-file", SERVER_CONFIG));

	serverTextFile = text_file_load(configFile);

	if (serverTextFile == NULL) {
		warning("Unable to load config file - using defaults");
		return;
	}

	prepareConfigFile(serverTextFile);
}

static char *findValue(char *s)
{
	int i;
	int len;

	len = strlen(s);

	for (i = 0; i < len - 1; i++) {
		if (s[i] == ' ' && s[i + 1] != ' ') {
			return s + i + 1;
		}
	}

	return NULL;
}

char *server_configFile_get_value(char *env, char *s)
{
	int i;
	int len;

	if (serverTextFile == NULL) {
		return s;
	}

	len = strlen(env);

	for (i = 0; i < serverTextFile->text->count; i++) {
		char *line;

		line = (char *) (serverTextFile->text->list[i]);

		if (strncmp(line, env, len) == 0) {
			return findValue(line);
		}
	}

	return s;
}

void server_configFile_quit()
{
	if (serverTextFile != NULL) {
		text_file_destroy(serverTextFile);
	}
}
