
#ifndef NET_MULTIPLAYER_H

#define NET_MULTIPLAYER_H

#include "tux.h"
#include "item.h"
#include "buffer.h"
#include "arena.h"

#define NET_GAME_TYPE_NONE	0
#define NET_GAME_TYPE_SERVER	1
#define NET_GAME_TYPE_CLIENT	2

#define NET_STATUS_OK		0
#define NET_STATUS_ZOMBIE	1

#define  LIMIT_BUFFER 10240

typedef struct client_struct
{
	int socket;
	int status;

	tux_t *tux;
	buffer_t *buffer;
} client_t;

extern int initServer(int port);
extern int initClient(int port, char *ip);
extern void netSetArena(arena_t *p);
extern int getNetTypeGame();
extern void proto_send_settux(tux_t *p);
extern void proto_send_shottux(tux_t *p);
extern void proto_send_switchgun(tux_t *p);
extern void proto_send_additem(item_t *item);
extern void eventNetMultiplayer();
extern void quitNetMultiplayer();

#endif
