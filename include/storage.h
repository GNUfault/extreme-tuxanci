
#ifndef STORAGE_H

#define STORAGE_H

#include "main.h"
#include "list.h"

extern list_t* newStorage();
extern void addItemToStorage(list_t *list, char *group, char *name, void *data);
extern void* getItemFromStorage(list_t *list, char *group, char *name);
extern void delItemFromStorage(list_t *list, char *group, char *name, void(*f)(void *));
extern void delAllItemFromStorage(list_t *list, char *group, void(*f)(void *));
extern void destroyStorage(list_t *p, void(*f)(void *) );

#endif
