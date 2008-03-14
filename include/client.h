
#ifndef MY_CLIENT

#define MY_CLIENT

extern int initTcpClient(char *ip, int port);
extern int initUdpClient(char *ip, int port);
extern void sendServer(char *msg);
extern void eventServerBuffer();
extern void selectClientTcpSocket();
extern void selectClientUdpSocket();
extern void quitTcpClient();
extern void quitUdpClient();

#endif 
