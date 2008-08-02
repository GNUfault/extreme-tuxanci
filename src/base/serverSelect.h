
#ifndef SERVER_SELECT

#define SERVER_SELECT

extern void restartSelect();
extern void addSockToSelect(int sock);
extern int actionSelect();
extern int isChangeSockInSelect(int sock);

#endif
