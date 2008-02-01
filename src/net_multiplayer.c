
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "list.h"
#include "tux.h"
#include "item.h"
#include "network.h"
#include "buffer.h"
#include "string_length.h"
#include "screen.h"
#include "screen_world.h"
#include "screen_setting.h"
#include "net_multiplayer.h"

static int netGameType;
static int sock_server;

static list_t *listClient;
static buffer_t *clientBuffer;
static arena_t *arena;

int initServer(int port)
{
	sock_server = newSocket(port , NULL);
	
	if( sock_server < 0 )
	{
		return -1;
	}

	listClient = newList();
	netGameType = NET_GAME_TYPE_SERVER;

	printf("server listen port %d\n", port);

	return 0;
}

int initClient(int port, char *ip)
{
	sock_server = newSocket(port , ip);

	if( sock_server < 0 )
	{
		return -1;
	}

	clientBuffer = newBuffer( LIMIT_BUFFER );
	netGameType = NET_GAME_TYPE_CLIENT;

	printf("connect %s %d\n", ip, port);

	return 0;
}

void netSetArena(arena_t *p)
{
	arena = p;
}

int getNetTypeGame()
{
	return netGameType;
}

static void sendServer(char *msg)
{
	int ret;

	ret =  write(sock_server, msg, strlen(msg));

	if ( ret == 0 )
	{
		fprintf(stderr, "server uzatvoril sietovy socket\n");
		setScreen("analyze");
	}

	if ( ret < 0 )
	{
		fprintf(stderr, "nastala chyba pri poslani spravy serveru\n");
		setScreen("analyze");
	}
}

static void sendClient(client_t *p, char *msg)
{
	if( p->status == NET_STATUS_OK )
	{
		int ret;

		ret = write(p->socket, msg, strlen(msg));

		if( ret == 0 )
		{
			fprintf(stderr, "client sa odpojil\n");
			p->status = NET_STATUS_ZOMBIE;
		}

		if( ret < 0 )
		{
			fprintf(stderr, "nastala chyba pri posielanie spravy clientovy\n");
			p->status = NET_STATUS_ZOMBIE;
		}
	}
}

static void sendAllClientBut(char *msg, client_t *p)
{
	client_t *thisClient;
	int i;

	for( i = 0 ; i < listClient->count; i++)
	{
		thisClient = (client_t *) listClient->list[i];

		if( thisClient != p )
		{
			sendClient(thisClient, msg);
		}
	}
}

static void sendAllClient(char *msg)
{
	sendAllClientBut(msg, NULL);
}

static void sendNetwork(char *msg)
{
	switch( netGameType )
	{
		case NET_GAME_TYPE_SERVER :
			sendAllClient(msg);
		break;
		case NET_GAME_TYPE_CLIENT :
			sendServer(msg);
		break;
	}
}

static client_t* newClient(int sock)
{
	client_t *new;
	
	new = malloc( sizeof(client_t) );
	memset(new, 0, sizeof(client_t));
	
	new->socket = sock;
	new->status = NET_STATUS_OK;
	new->buffer = newBuffer(LIMIT_BUFFER);
	new->tux = newTux();
	new->tux->control = TUX_CONTROL_NET;
	addList(arena->listTux, new->tux);

	return new;
}

static void destroyClient(client_t *p)
{
	int index;

	close(p->socket);
	destroyBuffer(p->buffer);
	index = searchListItem(arena->listTux, p->tux);
	delListItem(arena->listTux, index, destroyTux);
	free(p);
}

static void proto_send_init(client_t *p)
{
	char msg[STR_SIZE];

	sprintf(msg, "init %d\n", p->tux->id);
	sendClient(p, msg);
}

void proto_send_settux(tux_t *p)
{
	char msg[STR_SIZE];
	
	sprintf(msg, "settux %d %d %d %d %d\n",
		p->id ,p->x, p->y, p->position ,p->frame);
	
	sendNetwork(msg);
}

void proto_send_deltux(tux_t *p)
{
	char msg[STR_SIZE];
	
	sprintf(msg, "deltux %d\n", p->id);

	sendNetwork(msg);
}

void proto_send_shottux(tux_t *p)
{
	char msg[STR_SIZE];
	
	sprintf(msg, "shottux %d\n", p->id);

	sendNetwork(msg);
}

void proto_send_switchgun(tux_t *p)
{
	char msg[STR_SIZE];
	
	sprintf(msg, "switchgun %d %d\n", p->id, p->gun);

	sendNetwork(msg);
}

void proto_send_additem(item_t *p)
{
	char msg[STR_SIZE];
	
	sprintf(msg, "additem %d %d %d\n", p->type, p->x, p->y);

	sendNetwork(msg);
}

void proto_send_context(tux_t *p)
{
	char msg[STR_SIZE];
	
	sprintf(msg, "context %d %s\n", p->id, p->name);

	sendNetwork(msg);
}

static void proto_recv_init(char *msg)
{
	char cmd[STR_SIZE];
	tux_t *tux;

	tux = newTux();

	sscanf(msg, "%s %d\n", cmd, &tux->id);

	tux->control = TUX_CONTROL_KEYBOARD_RIGHT;
	getSettingNameRight(tux->name);
	proto_send_context(tux);
	addList(arena->listTux, tux);
}

static void proto_recv_settux(char *msg)
{
	char cmd[STR_SIZE];
	int a, b, c, d, e;
	tux_t *tux;

	sscanf(msg, "%s %d %d %d %d %d", cmd, &a, &b, &c, &d, &e);

	tux = getTuxID(arena->listTux, a);

	if( tux == NULL )
	{
		tux = newTux();
		tux->control = TUX_CONTROL_NET;
		addList(arena->listTux, tux);
	}

	tux->id = a;
	tux->x = b;
	tux->y = c;
	tux->position = d;
	tux->frame = e;
	tux->status = TUX_STATUS_ALIVE;

	// ???
	//eventGiveTuxItem(tux, arena->listItem);
}

static void proto_recv_deltux(char *msg)
{
	char cmd[STR_SIZE];
	int id;
	tux_t *tux;

	sscanf(msg, "%s %d\n", cmd, &id);

	tux = getTuxID(arena->listTux, id);

	if( tux != NULL )
	{
		int index;
		
		index = searchListItem(arena->listTux, tux);
		delListItem(arena->listTux, index, destroyTux);
	}
}

static void proto_recv_shottux(char *msg)
{
	char cmd[STR_SIZE];
	int id;
	tux_t *tux;

	sscanf(msg, "%s %d\n", cmd, &id);

	tux = getTuxID(arena->listTux, id);

	if( tux != NULL )
	{
		shotTux(tux);
	}
}

void proto_recv_switchgun(char *msg)
{
	char cmd[STR_SIZE];
	int id, gun;
	tux_t *tux;

	sscanf(msg, "%s %d %d\n", cmd, &id, &gun);

	tux = getTuxID(arena->listTux, id);

	if( tux != NULL )
	{
		tux->gun = gun;
	}
}

void proto_recv_additem(char *msg)
{
	char cmd[STR_SIZE];
	int type, x, y;
	item_t *item;

	sscanf(msg, "%s %d %d %d\n", cmd, &type, &x, &y);

	item = newItem(x, y, type);

	if( isConflictWithListItem(arena->listItem, item->x, item->y, item->w, item->h) )
	{
		destroyItem(item);
		return;
	}

	addList(arena->listItem, item);
}

void proto_recv_context(char *msg)
{
	char cmd[STR_SIZE];
	char name[STR_NAME_SIZE];
	int id;
	tux_t *tux;

	sscanf(msg, "%s %d %s\n", cmd, &id, name);

	tux = getTuxID(arena->listTux, id);

	if( tux != NULL )
	{
		strcpy(tux->name, name);
	}
}

static void eventCreateNewClient(int sock)
{
	client_t *new;
	tux_t *thisTux;
	item_t *thisItem;
	int i;

	new = newClient( getNewClient(sock) );
	addList(listClient, new);

	proto_send_init(new);

	for( i = 0 ; i < arena->listTux->count; i++)
	{
		thisTux = (tux_t *) arena->listTux->list[i];
		
		proto_send_settux(thisTux);
		
		if( thisTux != new->tux )
		{
			proto_send_context(thisTux);
		}
	}

	for( i = 0 ; i < arena->listItem->count; i++)
	{
		thisItem = (item_t *) arena->listItem->list[i];
		proto_send_additem(thisItem);
	}
}

static void eventClientSelect(client_t *client)
{
	char buffer[STR_SIZE];
	int ret;

	assert( client != NULL );

	memset(buffer, 0, STR_SIZE);
	ret = read(client->socket, buffer, STR_SIZE-1);

	if( ret <= 0 )
	{
		client->status = NET_STATUS_ZOMBIE;
		return;
	}

	if( addBuffer(client->buffer, buffer, ret) != 0 )
	{
		client->status = NET_STATUS_ZOMBIE;
		return;
	}
}

static void eventClientBuffer(client_t *client)
{
	char line[STR_SIZE];

	if( netGameType == NET_GAME_TYPE_NONE )
	{
		return;
	}

	/* obsluha udalosti od clientov */
	
	while( getBufferLine(client->buffer, line, STR_SIZE) >= 0 )
	{
		sendAllClientBut(line, client);

		if( strncmp(line, "settux", 6) == 0 )proto_recv_settux(line);
		if( strncmp(line, "shottux", 7) == 0 )proto_recv_shottux(line);
		if( strncmp(line, "switchgun", 9) == 0 )proto_recv_switchgun(line);
		if( strncmp(line, "additem", 7) == 0 )proto_recv_additem(line);
		if( strncmp(line, "context", 7) == 0 )proto_recv_context(line);
	}
}

static void eventClientListBuffer()
{
	client_t *thisClient;
	int i;

	for( i = 0 ; i < listClient->count; i++)
	{
		thisClient = (client_t *) listClient->list[i];
		eventClientBuffer(thisClient);
	}
}

static void selectServerSocket()
{
	fd_set readfds;
	struct timeval tv;
	client_t *thisClient;
	int max_fd;
	int i;

	tv.tv_sec = 0;
	tv.tv_usec = 0;
	
	FD_ZERO(&readfds);
	FD_SET(sock_server, &readfds);
	max_fd = sock_server;

	for( i = 0 ; i < listClient->count; i++)
	{
		thisClient = (client_t *) listClient->list[i];

		FD_SET(thisClient->socket, &readfds);
	
		if( thisClient->socket > max_fd )
		{
			max_fd = thisClient->socket;
		}
	}

	select(max_fd+1, &readfds, (fd_set *)NULL, (fd_set *)NULL, &tv);

	if( FD_ISSET(sock_server, &readfds) )
	{
		eventCreateNewClient(sock_server);
	}

	for( i = 0 ; i < listClient->count; i++)
	{
		thisClient = (client_t *) listClient->list[i];

		if( FD_ISSET(thisClient->socket, &readfds) )
		{
			eventClientSelect(thisClient);
		}

		if( thisClient->status == NET_STATUS_ZOMBIE )
		{
			proto_send_deltux(thisClient->tux);
			delListItem(listClient, i, destroyClient);
		}
	}
}

static void eventServerSelect(int sock)
{
	char buffer[STR_SIZE];
	int ret;

	memset(buffer,0 ,STR_SIZE);
	ret = read(sock, buffer, STR_SIZE-1);

	if( ret == 0 )
	{
		fprintf(stderr, "server uzatovril sietovy socket\n");
		setScreen("analyze");
		return;
	}

	if( ret < 0 )
	{
		fprintf(stderr, "chyba, spojenie prerusene \n");
		setScreen("analyze");
		return;
	}

	if( addBuffer(clientBuffer, buffer, ret) != 0 )
	{
		fprintf(stderr, "chyba, nemozem zapisovat do mojho buffera !\n");
		setScreen("analyze");
		return;
	}
}

static void eventServerBuffer()
{
	char line[STR_SIZE];

	if( netGameType == NET_GAME_TYPE_NONE )
	{
		return;
	}

	/* obsluha udalosti od servera */
	
	while ( getBufferLine(clientBuffer, line, STR_SIZE) >= 0 )
	{
		//printf("dostal som %s\n", line);
	
		if( strncmp(line, "init", 4) == 0 )proto_recv_init(line);
		if( strncmp(line, "settux", 6) == 0 )proto_recv_settux(line);
		if( strncmp(line, "deltux", 6) == 0 )proto_recv_deltux(line);
		if( strncmp(line, "shottux", 7) == 0 )proto_recv_shottux(line);
		if( strncmp(line, "switchgun", 9) == 0 )proto_recv_switchgun(line);
		if( strncmp(line, "additem", 7) == 0 )proto_recv_additem(line);
		if( strncmp(line, "context", 7) == 0 )proto_recv_context(line);
	}
}

static void selectClientSocket()
{
	fd_set readfds;
	struct timeval tv;
	int max_fd;

	tv.tv_sec = 0;
	tv.tv_usec = 0;
	
	FD_ZERO(&readfds);
	FD_SET(sock_server, &readfds);
	max_fd = sock_server;

	select(max_fd+1, &readfds, (fd_set *)NULL, (fd_set *)NULL, &tv);

	if( FD_ISSET(sock_server, &readfds) )
	{
		eventServerSelect(sock_server);
	}
}

void eventNetMultiplayer()
{
	switch( netGameType )
	{
		case NET_GAME_TYPE_NONE :
		break;

		case NET_GAME_TYPE_SERVER :
			selectServerSocket();
			eventClientListBuffer();
		break;

		case NET_GAME_TYPE_CLIENT :
			selectClientSocket();
			eventServerBuffer();
		break;

		default :
			assert( ! "Premenna netGameType ma zlu hodnotu !" );
		break;
	}
}

static void quitServer()
{
	destroyListItem(listClient, destroyClient);
	close(sock_server);
	printf("quit port\n");
}

static void quitClient()
{
	destroyBuffer(clientBuffer);
	close(sock_server);
	printf("quit conenct\n");
}

void quitNetMultiplayer()
{
	switch( netGameType )
	{
		case NET_GAME_TYPE_NONE :
		break;

		case NET_GAME_TYPE_SERVER :
			quitServer();
		break;

		case NET_GAME_TYPE_CLIENT :
			quitClient();
		break;

		default :
			assert( ! "Premenna netGameType ma zlu hodnotu !" );
		break;
	}

	netGameType = NET_GAME_TYPE_NONE;
}
