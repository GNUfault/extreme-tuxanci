/*
 *  (C) Copyright 2006 ZeXx86 (zexx86@gmail.com)
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


#ifndef _SOCKETS_H
#define _SOCKETS_H

#include "resolve.h"

extern int send_to_socket (int id, char *data, unsigned len);
extern int send_to_server (server_t* server, char *data);
extern int send_to_server_len (server_t* server, char *data, unsigned len);
extern bool init_sockets ();
extern bool loop ();

#endif
