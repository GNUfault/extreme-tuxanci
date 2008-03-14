
#include "list.h"
#include "tux.h"
#include "network.h"
#include "buffer.h"
#include "net_multiplayer.h"
#include "screen_world.h"
#include "tcp.h"
#include "udp.h"
#include "proto.h"
#include "client.h"

static int protocolType;
static sock_tcp_t *sock_server_tcp;
static sock_udp_t *sock_server_udp;
static buffer_t *clientBuffer;

int initTcpClient(char *ip, int port)
{
	sock_server_tcp = connectTcpSocket(ip, port);

	if( sock_server_tcp == NULL )
	{
		return -1;
	}

	printf("connect %s %d\n", ip, port);
	clientBuffer = newBuffer( LIMIT_BUFFER );

	return 0;
}

void sendServer(char *msg)
{
	int ret;

	if( protocolType == NET_PROTOCOL_TYPE_TCP )
	{
		ret = writeTcpSocket(sock_server_tcp, msg, strlen(msg));

		if ( ret == 0 )
		{
			fprintf(stderr, "server uzatvoril sietovy socket\n");
			setWorldEnd();
		}
	
		if ( ret < 0 )
		{
			fprintf(stderr, "nastala chyba pri poslani spravy serveru\n");
			setWorldEnd();
		}
	}

	if( protocolType == NET_PROTOCOL_TYPE_UDP )
	{
	}
}

static void eventServerTcpSelect()
{
	char buffer[STR_SIZE];
	int ret;

	memset(buffer,0 ,STR_SIZE);

	if( protocolType == NET_PROTOCOL_TYPE_TCP )
	{
		ret = readTcpSocket(sock_server_tcp, buffer, STR_SIZE-1);
	
		if( ret == 0 )
		{
			fprintf(stderr, "server uzatovril sietovy socket\n");
			setWorldEnd();
			return;
		}
	
		if( ret < 0 )
		{
			fprintf(stderr, "chyba, spojenie prerusene \n");
			setWorldEnd();
			return;
		}
	}

	if( protocolType == NET_PROTOCOL_TYPE_UDP )
	{
	}

	if( addBuffer(clientBuffer, buffer, ret) != 0 )
	{
		fprintf(stderr, "chyba, nemozem zapisovat do mojho buffera !\n");
		setWorldEnd();
		return;
	}
}

void eventServerBuffer()
{
	char line[STR_SIZE];

	/* obsluha udalosti od servera */
	
	while ( getBufferLine(clientBuffer, line, STR_SIZE) >= 0 )
	{
		//printf("dostal som %s", line);

		if( strncmp(line, "init", 4) == 0 )proto_recv_init_client(line);
		if( strncmp(line, "event", 5) == 0 )proto_recv_event_client(line);
		if( strncmp(line, "newtux", 6) == 0 )proto_recv_newtux_client(line);
		if( strncmp(line, "deltux", 6) == 0 )proto_recv_deltux_client(line);
		if( strncmp(line, "additem", 7) == 0 )proto_recv_additem_client(line);
	}
}

void selectClientTcpSocket()
{
	fd_set readfds;
	struct timeval tv;
	int max_fd;

	tv.tv_sec = 0;
	tv.tv_usec = 0;
	
	FD_ZERO(&readfds);
	FD_SET(sock_server_tcp->sock, &readfds);
	max_fd = sock_server_tcp->sock;

	select(max_fd+1, &readfds, (fd_set *)NULL, (fd_set *)NULL, &tv);

	if( FD_ISSET(sock_server_tcp->sock, &readfds) )
	{
		eventServerTcpSelect(sock_server_tcp);
	}
}

void quitTcpClient()
{
	closeTcpSocket(sock_server_tcp);
	destroyBuffer(clientBuffer);
	printf("quit conenct\n");
}

