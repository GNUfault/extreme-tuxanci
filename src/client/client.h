#ifndef MY_CLIENT_H
#define MY_CLIENT_H

#include "main.h"

#define CLIENT_TIMEOUT		1000
#define SERVER_TIMEOUT_ALIVE	5000

#define CLIENT_BUFFER_LIMIT	4096

extern int client_init(char *ip, int port);
extern void client_send(char *msg);
extern void client_event();
extern void client_quit();

#endif /* MY_CLIENT_H */
