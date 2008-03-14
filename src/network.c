
#include "network.h"
#include "tcp.h"
#include "udp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <assert.h>

int newSocket(int port,const char *ip)
{
	struct sockaddr_in address;
	int sockfd;
	int len;

	assert( port > 0 && port < 65535 );

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if( sockfd < 0 )
	{
		fprintf(stderr, "Nemozem vytvorit sietovy socket\n");
		return -1;
	}

	address.sin_family = AF_INET;

	if( ip != NULL )
		address.sin_addr.s_addr = inet_addr(ip);
	else
		address.sin_addr.s_addr = htonl(INADDR_ANY);

	address.sin_port = htons(port);
	
	len = sizeof(address);

	if( ip == NULL )
	{
		if ( bind(sockfd, (struct sockaddr *)&address, len) < 0 )
		{
			fprintf(stderr, "Nemozem obsadit sietovy port %d\n", port);
			return -1;
		}

		listen(sockfd, 5);
	}
	else
	{
		if ( connect(sockfd, (struct sockaddr *)&address, len) < 0 )
		{
			fprintf(stderr, "Nemozem sa pripojit na %s %d\n", ip, port);
			return -1;
		}
	}

	return sockfd;
}

int getNewClient(int sockfd)
{
	struct sockaddr_in client_address;
	int client_sockfd;
	int client_len;

	assert( sockfd >= 0 );

	client_len = sizeof(client_address);
	
	client_sockfd = accept(sockfd, (struct sockaddr *)&client_address,
			       (socklen_t *)&client_len);

	//printf("ip = %s\n",inet_ntoa(client_address.sin_addr));

	return client_sockfd;
}

int getNewClientIP(int sockfd,char **str_ip)
{
	struct sockaddr_in client_address;
	int client_sockfd;
	int client_len;

	assert( sockfd >= 0 );

	client_len = sizeof(client_address);
	
	client_sockfd = accept(sockfd, (struct sockaddr *)&client_address,
			       (socklen_t *)&client_len);

	*str_ip = strdup( inet_ntoa(client_address.sin_addr) );

	return client_sockfd;
}
