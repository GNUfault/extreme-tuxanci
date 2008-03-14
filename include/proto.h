
#ifndef MY_PROTO

#define MY_PROTO

#include "tux.h"
#include "server.h"

extern void proto_send_init_server(client_t *client);
extern void proto_recv_init_client(char *msg);
extern void proto_send_event_server(tux_t *tux, int action, client_t *client);
extern void proto_recv_event_client(char *msg);
extern void proto_send_event_client(int action);
extern void proto_recv_event_server(client_t *client, char *msg);
extern void proto_send_newtux_server(client_t *client, tux_t *tux);
extern void proto_recv_newtux_client(char *msg);
extern void proto_send_deltux_server(client_t *client);
extern void proto_recv_deltux_client(char *msg);
extern void proto_send_additem_server(item_t *p);
extern void proto_recv_additem_client(char *msg);
extern void proto_send_context_client(tux_t *tux);
extern void proto_recv_context_server(client_t *client, char *msg);

#endif

