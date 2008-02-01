
#ifndef ARENA_FILE_H

#define ARENA_FILE_H

#include "main.h"
#include "arena.h"

extern bool_t isArenaFileInicialized();
extern void initArenaFile();
extern arena_t* getArena(int id);
extern void quitArenaFile();

#endif
