
#ifndef MY_CLIENT

#define MY_CLIENT

extern int initTcpClient(char *ip, int port);
extern void sendServer(char *msg);
extern void eventServerBuffer();
extern void selectClientTcpSocket();
extern void quitTcpClient();

#endif 
