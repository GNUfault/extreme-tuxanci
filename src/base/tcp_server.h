
#ifndef SERVER_TCP_H

#    define SERVER_TCP_H

#    include "server.h"
#    include "tcp.h"

#    define SERVER_TCP_BUFFER_LIMIT	4096

extern client_t *server_tcp_new_client(sock_tcp_t * sock_tcp);
extern void server_tcp_destroy_client(client_t * p);
extern int server_tcp_init(char *ip4, int port4, char *ip6, int port6);
extern void server_tcp_set_select();
extern int server_tcp_select_sock();
extern void server_tcp_send_client_buffer(client_t * p);
extern void server_tcp_quit();

#endif
