
#ifndef LOG_H

#define LOG_H

#define LOG_INF		1
#define LOG_DBG		2
#define LOG_WRN		3
#define LOG_ERR		4

extern int initLog(char *name);
extern void addToLog(int type, char *msg);
extern void quitLog();

#endif
