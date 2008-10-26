/*
 *  (C) Copyright 2006 ZeXx86 (zexx86@gmail.com)
 *  (C) Copyright 2007 ZeXx86 (zexx86@gmail.com)
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


#ifndef _RESOLVE_H_
#define _RESOLVE_H_

#define		SERVER_STATE_OFFLINE		0
#define		SERVER_STATE_ONLINE		1
#define		SERVER_STATE_LAG		2
#define		SERVER_STATE_TIMEOUT		3

/* User structure */
typedef struct server_context {
  	struct server_context *next, *prev;

	unsigned socket;
	short state;

	unsigned ip;
	unsigned port;

	char version[4];
} server_t;

typedef struct client_context {
	struct client_context *next, *prev;

	unsigned socket;
	unsigned ip;
} client_t;

typedef struct response_header {
	unsigned port;

	/* for another use */
	unsigned ip;
} response_head;


extern unsigned client_connected (unsigned int client);
extern unsigned client_disconnected (client_t *client);
extern unsigned init_resolve ();
extern unsigned handler (client_t *client, char *buf, int size);
extern int server_resolve (unsigned ip, unsigned int port);

#endif 
