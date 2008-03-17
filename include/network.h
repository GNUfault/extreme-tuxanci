
#ifndef NETWORK_H

#include "main.h"

#define NETWORK_H

extern int newSocket(int port,const char *ip);
extern int getNewClient(int sockfd);
extern int getNewClientIP(int sockfd,char **str_ip);

#endif
