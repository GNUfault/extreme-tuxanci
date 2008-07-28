
#ifndef EXPORT_FUNCTION_H

#define EXPORT_FUNCTION_H

extern void initExortFunction();
extern void addToExportFce(char *name, void *function);
extern void* getExportFce(char *name);
extern void quitExportFunction();

#endif

