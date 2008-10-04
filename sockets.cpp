/*
 *  (C) Copyright 2008 ZeXx86 (zexx86@gmail.com)
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

 
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <fcntl.h>
#include <vector>
#include <errno.h>
#include "sockets.h"
#include "resolve.h"
#include "defaults.h"

int BUFSIZE = 1000;

using namespace std;

extern server_t server_list; // structure of all servers
extern client_t client_list; // structure with all connected clients

sockaddr_in sockName;         // Server's info structure
sockaddr_in clientInfo;       // Client's info structure
int mainSocket;               // Socket
int port = DEFAULT_PORT;      // Number of port
int size;                     // Po?et p?ijatých a odeslaných byt?
socklen_t addrlen;            // Velikost adresy vzdáleného po?íta?e


/* Functions for sending a data on socket */
int send_to_socket (int id, char *data, unsigned len)
{
	if(id < 1)
		return id;

	unsigned int size;
	if ((size = send (id, data, len, 0)) != len) {
		//cerr << "**WARNING** Socket >> " << id << " >> "
		    // << " return(" << size << "/" << txt.size()
		    // << ") >> " << strerror(errno) << endl;
	
		usleep (4096);

		return 0;
	}

	usleep (20);
	
	return 1;
}

int send_to_server (server_t* server, char *data)
{
	unsigned int len = strlen (data);

	unsigned int size;
	if ((size = send (server->socket, data, len, 0)) != len) {
		//cerr << "**WARNING** Socket >> " << server->socket << " >> "
		    // << " return(" << size << "/" << data
		    // << ") >> " << strerror(errno) << endl;
	
		usleep (4096);

		return 0;
	}

	usleep (20);

	return 1;
}

int send_to_server_len (server_t* server, char *data, unsigned int len)
{
	unsigned int size;
	if ((size = send (server->socket, data, len, 0)) != len) {
		//cerr << "**WARNING** Socket >> " << server->socket << " >> "
		     //<< " return(" << size << "/" << data
		    // << ") >> " << strerror(errno) << endl;

		usleep (4096);

		return 0;
	}

	return 1;
}

int server_resolve (unsigned ip, unsigned port)
{
	struct sockaddr_in srv;
	int mySocket;

	if ((mySocket = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
	    return 0;
	}

	// Lets make socket non-blocking
	int oldFlag = fcntl (mySocket, F_GETFL, 0);
	if (fcntl (mySocket, F_SETFL, oldFlag | O_NONBLOCK) == -1) {
		return 0;
	}

	struct timeval tv;

	fd_set myset;
	FD_ZERO(&myset);
	FD_SET(mySocket, &myset);

	tv.tv_sec = 20;
	tv.tv_usec = 0;

	srv.sin_family = AF_INET;
	srv.sin_port = htons (port);
	srv.sin_addr.s_addr = ip;

	if (connect (mySocket, (struct sockaddr *) &srv, sizeof (srv)) == -1) {
		if (errno != EINPROGRESS)
		    return 0;
	}

	int ret = select (mySocket+1, NULL, &myset, NULL, &tv);

	if (ret == -1)
		return -1;

	if (ret == 0)
		return -1;

	char request[8];
	memcpy (request, "status\n", 7);

	int r = send (mySocket, request, 7, 0);

	if (r == -1)
		return -1;

	FD_ZERO(&myset);
	FD_SET(mySocket, &myset);
	tv.tv_sec = 10;
	tv.tv_usec = 0;

	char *str = (char *) malloc (sizeof (char) * 257);

	if (!str)
		return 0;

	while (1) {
		int sel = select (mySocket + 1, &myset, NULL, NULL, &tv);

		if (sel == 0) {
			free (str);
			close (mySocket);
			return -1;
		}

		if (sel == -1) {
			free (str);
			close (mySocket);
			return -1;
		}
  
		if ((ret = recv(mySocket, str, 256, 0)) == -1) {
			free (str);
			close (mySocket);
			return -1;
		}
    
		if (ret > 0)
			break;
	}

	close (mySocket);

   	return 1;
}

bool init_sockets ()
{
	// Create listen socket
	if ((mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		cerr << "Nelze vytvo?it soket" << endl;
		return -1;
	}
	
	int yes = 1;
	setsockopt (mainSocket, SOL_SOCKET, SO_REUSEADDR, (char *) &yes, sizeof (yes));
	
	// Nastavíme soket do neblokovacího re¾imu
	int oldFlag = fcntl (mainSocket, F_GETFL, 0);
	if (fcntl (mainSocket, F_SETFL, oldFlag | O_NONBLOCK) == -1) {
		cerr << "**ERROR** Problem with set socket mode" << endl;
		return -1;
	}

	// Lets fill structure sockaddr_in
	sockName.sin_family = AF_INET;
	sockName.sin_port = htons (port);
	sockName.sin_addr.s_addr = INADDR_ANY;

	// bind port
	if (bind (mainSocket, (sockaddr *) &sockName, sizeof (sockName)) == -1) {
		cerr << "Problém s pojmenováním soketu." << endl;
		return -1;
	}
	
	// Create front for 10 clients
	if (listen (mainSocket, 10) == -1) {
		cerr << "Problém s vytvo?ením fronty" << endl;
		return -1;
	}
	
	return 1;
}

/* MAIN LOOP */
bool loop ()
{
	{
		// Pozna?ím si velikost struktury clientInfo.
		// P?edám to funkci accept. 
		addrlen = sizeof(clientInfo);
		// Vyberu z fronty po?adavek na spojení.
		// "client" je nový soket spojující klienta se serverem.
		int client = accept (mainSocket, (sockaddr*)&clientInfo, &addrlen);
		
		/* Pripojil se novy client ? */
		if (client != -1 || errno != EAGAIN) {
			// Nastavíme soket do neblokovacího re?imu
			int oldFlag = fcntl(client, F_GETFL, 0);
			if (fcntl(client, F_SETFL, oldFlag | O_NONBLOCK) == -1) {
				//cerr << "**ERROR** Problem with set socket mode" << endl;
				return 0;
			}
			
			if (client == -1) {
				cerr << "> ERROR -> I cant accept socket" <<endl;
				return 0;
			}
			// Zjistím IP klienta.
			cout << "-> New client: "
				<< inet_ntoa ((in_addr)clientInfo.sin_addr)
				<< " Socket: " << client << endl;

			client_connected (client);	// create a new client structure
		}
	}

	usleep (128);

	char buf[BUFSIZE];            // Receive buffer

	{
		client_t *client;
		for (client = client_list.next; client != &client_list; client = client->next) {
			unsigned int id = client->socket;
	
			if ((size = recv (id, buf, BUFSIZE - 1, 0)) == 0) {
				//cout << strerror(errno) << size << endl;
				// Received 0 bytes - lets disconnect current client
				client_disconnected (client);
				
				// Go out from client's loop now !
				break;
			}

			buf[size] = '\0';
			
			handler (client, buf, size);
		}
	}

	return 1;
}
