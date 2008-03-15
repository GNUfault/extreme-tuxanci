
#ifndef MY_SERVER

#define MY_SERVER

#include "list.h"
#include "tux.h"
#include "buffer.h"
#include "myTimer.h"
#include "tcp.h"
#include "udp.h"

#ifndef BUBLIC_SERVER
#include "interface.h"
#endif

#define SERVER_TIMEOUT		5000
#define SERVER_TIME_SYNC	5000

#define SERVER_INDEX_ROOT_TUX	0

typedef struct client_struct
{
	sock_tcp_t *socket_tcp;
	sock_udp_t *socket_udp;
	int status;
	my_time_t lastPing;
	tux_t *tux;
	buffer_t *buffer;
} client_t;

extern int initTcpServer(int port);
extern int initUdpServer(int port);
extern client_t* newTcpClient(sock_tcp_t *sock_tcp);
extern client_t* newUdpClient(sock_udp_t *sock_udp);
extern void destroyClient(client_t *p);
extern void sendClient(client_t *p, char *msg);
extern void sendAllClientBut(char *msg, client_t *p);
extern void sendAllClient(char *msg);
extern void eventClientListBuffer();
extern void selectServerTcpSocket();
extern void selectServerUdpSocket();
extern void eventPeriodicSyncClient();
extern void quitTcpServer();
extern void quitUdpServer();

#endif 
 
