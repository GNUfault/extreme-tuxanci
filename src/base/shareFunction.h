
#ifndef SHARE_FUNCTION_H
#    define SHARE_FUNCTION_H

extern void share_function_init();
extern void share_function_add(char *name, void *function);
extern void *share_function_get(char *name);
extern void share_function_quit();
#endif
