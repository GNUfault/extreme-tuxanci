#ifndef SERVER_UDP_H
#define SERVER_UDP_H

#include "server.h"
#include "udp.h"

extern client_t *server_udp_new_client(sock_udp_t *sock_udp);
extern void server_udp_destroy_client(client_t *p);
extern int server_udp_init(char *ip4, int port4, char *ip6, int port6);
extern void server_udp_set_select();
extern int server_udp_select_sock();
extern void server_udp_quit();

#endif /* SERVER_UDP_H */
