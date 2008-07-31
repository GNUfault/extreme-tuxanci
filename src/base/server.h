
#ifndef MY_SERVER

#define MY_SERVER

#include <time.h>

#include "list.h"
#include "tux.h"
#include "myTimer.h"
#include "protect.h"
#include "udp.h"

#ifndef PUBLIC_SERVER
#include "interface.h"
#endif

#define SERVER_TIMEOUT		1000
#define SERVER_TIME_SYNC	1000
#define SERVER_TIME_PING	1000
#define SERVER_MAX_CLIENTS	100


#define SERVER_INDEX_ROOT_TUX	0

typedef struct client_struct
{
	sock_udp_t *socket_udp_server;
	sock_udp_t *socket_udp;

	int status;

	tux_t *tux;

	protect_t *protect;

	list_t *listSendMsg;
	list_t *listRecvMsg;

	list_t *listSeesShot;
} client_t;


extern time_t getUpdateServer();
extern int initUdpServer(char *ip, int port, int proto);
extern int initUdpPublicServer(char *ip4, int port4, char *ip6, int port6);
extern list_t* getListServerClient();
extern int getServerMaxClients();
extern void setServerMaxClients(int n);
extern void sendClient(client_t *p, char *msg);
extern void protoSendClient(int type, client_t *client, char *msg, int type2, int id);
extern void sendInfoCreateClient(client_t *client);
extern void eventServer();
extern void quitUdpServer();

#endif
