
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/select.h>

#include "main.h"
#include "list.h"
#include "tux.h"
#include "proto.h"

#include "client.h"
#include "language.h"

#include "screen_analyze.h"
#include "screen_world.h"

#include "udp.h"

static int protocolType;

static sock_udp_t *sock_server_udp;

static list_t *clientBuffer;
static my_time_t lastPing;
static my_time_t lastPingServerAlive;

static void initClient()
{
	char name[STR_NAME_SIZE];

	clientBuffer = newList();
	lastPing = getMyTime();
	lastPingServerAlive = getMyTime();

	getSettingNameRight(name);
	proto_send_hello_client(name);
}

int initUdpClient(char *ip, int port, int proto)
{
	sock_server_udp = connectUdpSocket(ip, port, proto);

	if( sock_server_udp == NULL )
	{
		return -1;
	}

	printf("connect UDP %s %d\n", ip, port);

	protocolType = NET_PROTOCOL_TYPE_UDP;
	initClient();

	return 0;
}

void sendServer(char *msg)
{
	int ret;

	assert( msg != NULL );

#ifndef PUBLIC_SERVER
	if( isParamFlag("--send") )
	{
		printf("send -> %s", msg);
	}
#endif

	ret = writeUdpSocket(sock_server_udp, sock_server_udp, msg, strlen(msg));
}

static int eventServerSelect()
{
	char buffer[STR_PROTO_SIZE];
	int ret;

	memset(buffer,0 ,STR_PROTO_SIZE);

	ret = readUdpSocket(sock_server_udp, sock_server_udp, buffer, STR_PROTO_SIZE-1);

	if( ret < 0 )
	{
		return ret;
	}

	addList(clientBuffer, strdup(buffer) );
	
	return ret;
}

static void eventServerBuffer()
{
	char *line;
	int i;

	/* obsluha udalosti od servera */
	
	assert( clientBuffer != NULL );

	for( i = 0 ; i < clientBuffer->count ; i++ )
	{
		line = (char *)clientBuffer->list[i];

#ifndef PUBLIC_SERVER
		if( isParamFlag("--recv") )
		{
			printf("recv -> %s", line);
		}
#endif
		if( strncmp(line, "error", 5) == 0 )proto_recv_error_client(line);
		if( strncmp(line, "init", 4) == 0 )proto_recv_init_client(line);
		if( strncmp(line, "event", 5) == 0 )proto_recv_event_client(line);
		if( strncmp(line, "newtux", 6) == 0 )proto_recv_newtux_client(line);
		if( strncmp(line, "del", 3) == 0 )proto_recv_del_client(line);
		if( strncmp(line, "additem", 7) == 0 )proto_recv_additem_client(line);
		if( strncmp(line, "shot", 4) == 0 )proto_recv_shot_client(line);
		if( strncmp(line, "kill", 4) == 0 )proto_recv_kill_client(line);
		if( strncmp(line, "module", 6) == 0 )proto_recv_module_client(line);
		if( strncmp(line, "chat", 4) == 0 )proto_recv_chat_client(line);
		if( strncmp(line, "ping", 4) == 0 )proto_recv_ping_client(line);
		if( strncmp(line, "end", 3) == 0 )proto_recv_end_client(line);

		lastPingServerAlive = getMyTime();
	}

	destroyListItem(clientBuffer, free);
	clientBuffer = newList();
}

static void eventPingServer()
{
	my_time_t currentTime;

 	currentTime = getMyTime();

	if( currentTime - lastPing > CLIENT_TIMEOUT )
	{
		proto_send_ping_client();
		lastPing = getMyTime();
	}
}

static bool_t isServerAlive()
{
	my_time_t currentTime;

 	currentTime = getMyTime();

	if( currentTime - lastPingServerAlive > SERVER_TIMEOUT_ALIVE )
	{
		return FALSE;
	}

	return TRUE;
}

static void selectClientUdpSocket()
{
	fd_set readfds;
	struct timeval tv;
	int max_fd;
	bool_t isNext;

	if( isServerAlive() == FALSE )
	{
		fprintf(stderr, "server neodpoveda !\n");
		setMsgToAnalyze(getMyText("ERROR_SERVER_DONT_ALIVE"));
		setWorldEnd();
		return;
	}

	do{
		isNext = FALSE;

		tv.tv_sec = 0;
		tv.tv_usec = 0;
		
		FD_ZERO(&readfds);
		FD_SET(sock_server_udp->sock, &readfds);
		max_fd = sock_server_udp->sock;
	
		select(max_fd+1, &readfds, (fd_set *)NULL, (fd_set *)NULL, &tv);
	
		if( FD_ISSET(sock_server_udp->sock, &readfds) )
		{
			eventServerSelect();
			isNext = TRUE;
		}

	}while( isNext == TRUE );
}

void eventClient()
{
	eventPingServer();
	selectClientUdpSocket();
	eventServerBuffer();
}

static void quitClient()
{
	proto_send_end_client();
	assert( clientBuffer != NULL );
	destroyListItem(clientBuffer, free);
}

void quitUdpClient()
{
	quitClient();

	assert( sock_server_udp != NULL );
	closeUdpSocket(sock_server_udp);

	printf("quit UDP conenct\n");
}
