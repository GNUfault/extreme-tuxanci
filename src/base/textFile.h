
#ifndef TEXTFILE_H

#    define TEXTFILE_H

#    include "list.h"

#    ifdef __WIN32__
#        define lstat stat
#    endif

typedef struct str_textFile_t {
	char *file;
	list_t *text;
} textFile_t;

extern textFile_t *text_file_new(char *s);
extern textFile_t *text_file_load(char *s);
extern void text_file_print(textFile_t * p);
extern void text_file_save(textFile_t * p);
extern void text_file_destroy(textFile_t * p);


#endif
