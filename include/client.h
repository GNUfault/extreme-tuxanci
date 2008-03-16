
#ifndef MY_CLIENT

#define MY_CLIENT

#if defined SUPPORT_NET_UNIX_UDP || defined SUPPORT_NET_SDL_UDP

#define CLIENT_TIMEOUT	2500

#endif

#ifdef SUPPORT_NET_UNIX_TCP
extern int initTcpClient(char *ip, int port);
extern void selectClientTcpSocket();
extern void quitTcpClient();
#endif

#ifdef SUPPORT_NET_UNIX_UDP
extern int initUdpClient(char *ip, int port);
extern void eventPingServer();
extern void selectClientUdpSocket();
extern void quitUdpClient();
#endif

#ifdef SUPPORT_NET_SDL_UDP
extern int initSdlUdpClient(char *ip, int port);
extern void eventPingServer();
extern void selectClientSdlUdpSocket();
extern void quitSdlUdpClient();
#endif

extern void sendServer(char *msg);
extern void eventServerBuffer();

#endif 
