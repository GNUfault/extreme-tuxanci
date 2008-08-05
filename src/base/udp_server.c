#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

#ifndef __WIN32__
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/select.h>
#else
#include <io.h>
#include <winsock2.h>
#endif

#include "main.h"
#include "list.h"
#include "tux.h"
#include "proto.h"
#include "server.h"
#include "myTimer.h"
#include "arena.h"
#include "net_multiplayer.h"
#include "checkFront.h"
#include "protect.h"
#include "index.h"

#ifndef PUBLIC_SERVER
#include "screen_world.h"
#endif

#ifdef PUBLIC_SERVER
#include "publicServer.h"
#include "heightScore.h"
#include "log.h"
#endif

#include "udp.h"
#include "udp_server.h"
#include "serverSelect.h"

static sock_udp_t *sock_server_udp;
static sock_udp_t *sock_server_udp_second;

client_t* newUdpClient(sock_udp_t *sock_udp)
{
	client_t *new;

	assert( sock_udp != NULL );

		new = newAnyClient();
	new->type = CLIENT_TYPE_UDP;
	new->socket_udp = sock_udp;

#ifdef PUBLIC_SERVER
	char str_log[STR_LOG_SIZE];
	char str_ip[STR_IP_SIZE];

	getSockUdpIp(sock_udp, str_ip, STR_IP_SIZE);
	sprintf(str_log, "new client UDP from %s %d", str_ip, getSockUdpPort(sock_udp));
	addToLog(LOG_INF, str_log);
#endif

	return new;
}

void destroyUdpClient(client_t *p)
{
	eventMsgInCheckFront(p);

#ifdef PUBLIC_SERVER
	char str_log[STR_LOG_SIZE];
	char str_ip[STR_IP_SIZE];

	getSockUdpIp(p->socket_udp, str_ip, STR_IP_SIZE);
	sprintf(str_log, "close UDP connect %s %d", str_ip, getSockUdpPort(p->socket_udp));
	addToLog(LOG_INF, str_log);
#endif

	destroySockUdp(p->socket_udp);

	destroyAnyClient(p);
}

int initUdpServer(char *ip4, int port4, char *ip6, int port6)
{
	int ret;

	ret = 0;

	if( ip4 != NULL )
	{
		sock_server_udp = bindUdpSocket(ip4, port4, PROTO_UDPv4);

		if( sock_server_udp != NULL )
		{
			ret++;
			printf("server listen UDP port %s %d\n", ip4, port4);
		}
		else
		{
			printf("server listen UDP port %s %d -- failed !!!\n", ip4, port4);
		}
	}

	if( ip6 != NULL )
	{
		sock_server_udp_second = bindUdpSocket(ip6, port6, PROTO_UDPv6);

		if( sock_server_udp_second != NULL )
		{
			ret++;
			printf("server listen UDP port %s %d\n", ip6, port6);
		}
		else
		{
			printf("server listen UDP port %s %d -- failed !!!\n", ip6, port6);
		}
	}

	return ret;
}

static void eventCreateNewUdpClient(sock_udp_t *socket_udp)
{
	list_t *listClient;
	client_t *client;

	assert( socket_udp != NULL );

	listClient = getListServerClient();

	client = newUdpClient(socket_udp);
	addList(listClient, client);
}

static client_t* findUdpClient(sock_udp_t *sock_udp)
{
	list_t *listClient;
	int port;
	int i;

	port  = getSockUdpPort(sock_udp);
	listClient = getListServerClient();

	for( i  = 0 ; i < listClient->count ; i++ )
	{
		client_t *client;

		client = (client_t *)listClient->list[i];

		if( client->type == CLIENT_TYPE_UDP &&
		    getSockUdpPort(client->socket_udp) == port )
		{
			return client;
		}
	}

	return NULL;
}

static void eventClientUdpSelect(sock_udp_t *sock_server)
{
	sock_udp_t *sock_client;
	client_t *client;
	char listRecvMsg[STR_SIZE];
	bool_t isCreateNewClient;
	int ret;

	assert( sock_server != NULL );

	sock_client = newSockUdp(sock_server->proto);
	isCreateNewClient = FALSE;

	memset(listRecvMsg, 0, STR_SIZE);

	ret = readUdpSocket(sock_server, sock_client, listRecvMsg, STR_SIZE-1);

	client = findUdpClient(sock_client);

	if( client == NULL )
	{
		if( getCurrentArena()->spaceTux->list->count+1 > getServerMaxClients() )
		{
			destroySockUdp(sock_client);
			return;
		}

		eventCreateNewUdpClient(sock_client);
		client = findUdpClient(sock_client);
		isCreateNewClient = TRUE;
	}

	if( client == NULL )
	{
		fprintf(stderr, "Client total not found !\n");
		return;
	}
	
	if( isCreateNewClient == FALSE )
	{
		destroySockUdp(sock_client);
	}

	if( ret <= 0 )
	{
		client->status = NET_STATUS_ZOMBIE;
		return;
	}

	//printf("add packet >>%s<<\n", listRecvMsg);
	addList(client->listRecvMsg, strdup(listRecvMsg) );
}

void setServerUdpSelect()
{
	if( sock_server_udp != NULL )
	{
		addSockToSelectRead(sock_server_udp->sock);
	}

	if( sock_server_udp_second != NULL )
	{
		addSockToSelectRead(sock_server_udp_second->sock);
	}
}

int selectServerUdpSocket()
{
	int count;

	count = 0;

	if( sock_server_udp != NULL )
	{
		if( isChangeSockInSelectRead(sock_server_udp->sock) )
		{
			eventClientUdpSelect(sock_server_udp);
			count++;
		}
	}

	if( sock_server_udp_second != NULL )
	{
		if( isChangeSockInSelectRead(sock_server_udp_second->sock) )
		{
			eventClientUdpSelect(sock_server_udp_second);
			count++;
		}
	}

	return count;
}

void quitUdpServer()
{
	if( sock_server_udp != NULL )
	{
		printf("close port %d\n", getSockUdpPort(sock_server_udp));
		closeUdpSocket(sock_server_udp);
	}

	if( sock_server_udp_second != NULL )
	{
		printf("close port %d\n", getSockUdpPort(sock_server_udp_second));
		closeUdpSocket(sock_server_udp_second);
	}

	printf("quit UDP port\n");
}
