
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

#ifndef __WIN32__
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/select.h>
#else
#include <io.h>
#include <winsock2.h>
#endif

#include "server.h"

static struct timeval tv;
static fd_set readfds;
static int max_fd;

void restartSelect()
{
#ifndef PUBLIC_SERVER
	tv.tv_sec = 0;
	tv.tv_usec = 0;
#endif	

#ifdef PUBLIC_SERVER
	tv.tv_sec = 0;
	tv.tv_usec = 1000;
#endif	

	FD_ZERO(&readfds);
	max_fd = 0;
}

void addSockToSelect(int sock)
{
	//printf("addSockToSelect %d\n", sock);

	FD_SET(sock, &readfds);

	if(sock > max_fd )
	{
		max_fd = sock;
	}
}

int actionSelect()
{
	int ret;

#ifdef PUBLIC_SERVER
	list_t *listClient;
	listClient = getListServerClient();

	if( listClient->count == 0 )
	{
		ret = select(max_fd+1, &readfds, (fd_set *)NULL, (fd_set *)NULL, NULL);
		setServerTimer();
	}
	else
	{
		ret = select(max_fd+1, &readfds, (fd_set *)NULL, (fd_set *)NULL, &tv);
	}
#endif

#ifndef PUBLIC_SERVER
	ret = select(max_fd+1, &readfds, (fd_set *)NULL, (fd_set *)NULL, &tv);
#endif

	return ret;
}

int isChangeSockInSelect(int sock)
{
	int ret;

	ret = FD_ISSET(sock, &readfds);

	//printf("isChangeSockInSelect %d -> %d\n", sock, ret);

	return ret;
}
