
#ifndef CONFIGFILE_H

#define CONFIGFILE_H

#include "textFile.h"

extern char* getValueInConfigFile(textFile_t *p, char *s);
extern void delRemInConfigFile(textFile_t *p);

#endif
