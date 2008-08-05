
#ifndef SERVER_SELECT

#define SERVER_SELECT

extern void restartSelect();
extern void addSockToSelectRead(int sock);
extern void addSockToSelectWrite(int sock);
extern int actionSelect();
extern int isChangeSockInSelect(int sock);
extern int isChangeSockInSelectRead(int sock);
extern int isChangeSockInSelectWrite(int sock);

#endif
