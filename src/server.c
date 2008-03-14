
#include "list.h"
#include "tux.h"
#include "network.h"
#include "buffer.h"
#include "net_multiplayer.h"
#include "screen_world.h"
#include "tcp.h"
#include "udp.h"
#include "proto.h"
#include "server.h"
#include "assert.h"

static int protocolType;
static sock_tcp_t *sock_server_tcp;
static sock_udp_t *sock_server_udp;
static list_t *listClient;

int initTcpServer(int port)
{
	protocolType = NET_PROTOCOL_TYPE_TCP;

	sock_server_tcp = bindTcpSocket(port);
	listClient = newList();

	if( sock_server_tcp == NULL )
	{
		return -1;
	}

	printf("server listen port %d\n", port);

	return 0;
}

static client_t* newAnyClient()
{
	client_t *new;
	
	new = malloc( sizeof(client_t) );
	memset(new, 0, sizeof(client_t));
	
	new->status = NET_STATUS_OK;
	new->buffer = newBuffer(LIMIT_BUFFER);
	new->tux = newTux();
	new->tux->control = TUX_CONTROL_NET;
	addList(getWorldArena()->listTux, new->tux);

	return new;
}

client_t* newTcpClient(sock_tcp_t *sock_tcp)
{
	client_t *new;
	
	new = newAnyClient();
	new->socket_tcp = sock_tcp;

	return new;
}

client_t* newUdpClient(sock_udp_t *sock_udp)
{
	client_t *new;
	
	new = newAnyClient();
	new->socket_udp = sock_udp;

	return new;
}

void destroyClient(client_t *p)
{
	int index;

	if( p->socket_tcp != NULL )
	{
		closeTcpSocket(p->socket_tcp);
	}

	if( p->socket_udp != NULL )
	{
		closeUdpSocket(p->socket_udp);
	}

	destroyBuffer(p->buffer);
	index = searchListItem(getWorldArena()->listTux, p->tux);
	delListItem(getWorldArena()->listTux, index, destroyTux);

	free(p);
}

void sendClient(client_t *p, char *msg)
{
	if( p->status == NET_STATUS_OK )
	{
		int ret;

		if( protocolType == NET_PROTOCOL_TYPE_TCP )
		{
			ret = writeTcpSocket(p->socket_tcp, msg, strlen(msg));
	
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
	
		if( protocolType == NET_PROTOCOL_TYPE_UDP )
		{
		}
	}
}

void sendAllClientBut(char *msg, client_t *p)
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

void sendAllClient(char *msg)
{
	sendAllClientBut(msg, NULL);
}

static void eventCreateNewTcpClient(sock_tcp_t *socket_tcp)
{
	client_t *client;
	client_t *thisClient;
	tux_t *thisTux;
	item_t *thisItem;
	int i;

	client = newTcpClient( getTcpNewClient(socket_tcp) );
	addList(listClient, client);

	proto_send_init_server(client);

	proto_send_newtux_server(client, (tux_t *)(getWorldArena()->listTux->list[0]) );

	for( i = 0 ; i < listClient->count; i++)
	{
		thisClient = (client_t *) listClient->list[i];
		thisTux = thisClient->tux;

		if( thisTux != client->tux )
		{
			proto_send_newtux_server(thisClient, client->tux);
			proto_send_newtux_server(client, thisTux);
		}
	}

	for( i = 0 ; i < getWorldArena()->listItem->count; i++)
	{
		thisItem = (item_t *) getWorldArena()->listItem->list[i];
		proto_send_additem_server(thisItem);
	}
}

static void eventClientTcpSelect(client_t *client)
{
	char buffer[STR_SIZE];
	int ret;

	assert( client != NULL );

	memset(buffer, 0, STR_SIZE);
	ret = readTcpSocket(client->socket_tcp, buffer, STR_SIZE-1);

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

	/* obsluha udalosti od clientov */
	
	while( getBufferLine(client->buffer, line, STR_SIZE) >= 0 )
	{
 		//printf("dostal som %s", line);

		if( strncmp(line, "event", 5) == 0 )proto_recv_event_server(client, line);
		if( strncmp(line, "context", 7) == 0 )proto_recv_context_server(client, line);
	}
}

void eventClientListBuffer()
{
	int i;
	client_t *thisClient;

	for( i = 0 ; i < listClient->count; i++)
	{
		thisClient = (client_t *) listClient->list[i];
		eventClientBuffer(thisClient);
	}
}

void selectServerTcpSocket()
{
	fd_set readfds;
	struct timeval tv;
	client_t *thisClient;
	int max_fd;
	int i;

	tv.tv_sec = 0;
	tv.tv_usec = 0;
	
	FD_ZERO(&readfds);
	FD_SET(sock_server_tcp->sock, &readfds);
	max_fd = sock_server_tcp->sock;

	for( i = 0 ; i < listClient->count; i++)
	{
		thisClient = (client_t *) listClient->list[i];

		FD_SET(thisClient->socket_tcp->sock, &readfds);
	
		if( thisClient->socket_tcp->sock > max_fd )
		{
			max_fd = thisClient->socket_tcp->sock;
		}
	}

	select(max_fd+1, &readfds, (fd_set *)NULL, (fd_set *)NULL, &tv);

	if( FD_ISSET(sock_server_tcp->sock, &readfds) )
	{
		eventCreateNewTcpClient(sock_server_tcp);
	}

	for( i = 0 ; i < listClient->count; i++)
	{
		thisClient = (client_t *) listClient->list[i];

		if( FD_ISSET(thisClient->socket_tcp->sock, &readfds) )
		{
			eventClientTcpSelect(thisClient);
		}

		if( thisClient->status == NET_STATUS_ZOMBIE )
		{
			proto_send_deltux_server(thisClient);
			delListItem(listClient, i, destroyClient);
		}
	}
}

void quitTcpServer()
{
	destroyListItem(listClient, destroyClient);
	closeTcpSocket(sock_server_tcp);
	printf("quit port\n");
}


