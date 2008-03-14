
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "list.h"
#include "tux.h"
#include "item.h"
#include "network.h"
#include "buffer.h"
#include "string_length.h"
#include "screen.h"
#include "screen_world.h"
#include "screen_setting.h"
#include "screen_choiceArena.h"
#include "arenaFile.h"
#include "net_multiplayer.h"
#include "server.h"
#include "client.h"
#include "tcp.h"
#include "udp.h"

static int netGameType;

int getNetTypeGame()
{
	return netGameType;
}

int initNetMuliplayer(int type, char *ip, int port)
{
	netGameType = type;

	switch( netGameType )
	{
		case NET_GAME_TYPE_NONE :
		break;

		case NET_GAME_TYPE_SERVER :
			if( initTcpServer(port) != 0 )
			{
				fprintf(stderr, "Neomzem inicalizovat sietovu hru pre server !\n");
				netGameType = NET_GAME_TYPE_NONE;
				return -1;
			}
		break;

		case NET_GAME_TYPE_CLIENT :
			if( initTcpClient(ip, port) != 0 )
			{
				fprintf(stderr, "Neomzem inicalizovat sietovu hru pre clienta !\n");
				netGameType = NET_GAME_TYPE_NONE;
				return -1;
			}
		break;

		default :
			assert( ! "Premenna netGameType ma zlu hodnotu !" );
		break;
	}

	return 0;
}

void eventNetMultiplayer()
{
	switch( netGameType )
	{
		case NET_GAME_TYPE_NONE :
		break;

		case NET_GAME_TYPE_SERVER :
			selectServerTcpSocket();
			eventClientListBuffer();
		break;

		case NET_GAME_TYPE_CLIENT :
			selectClientTcpSocket();
			eventServerBuffer();
		break;

		default :
			assert( ! "Premenna netGameType ma zlu hodnotu !" );
		break;
	}
}

void quitNetMultiplayer()
{
	switch( netGameType )
	{
		case NET_GAME_TYPE_NONE :
		break;

		case NET_GAME_TYPE_SERVER :
			quitTcpServer();
		break;

		case NET_GAME_TYPE_CLIENT :
			quitTcpClient();
		break;

		default :
			assert( ! "Premenna netGameType ma zlu hodnotu !" );
		break;
	}

	netGameType = NET_GAME_TYPE_NONE;
}
