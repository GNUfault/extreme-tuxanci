
#ifndef SAVE_DIALOG_H

#    define SAVE_DIALOG_H

#    define SAVEDIALOG_ACTIVE_TIME_INTERVAL	500

#    define SAVE_DIALOG_SIZE_X	320
#    define SAVE_DIALOG_SIZE_Y	120

#    define SAVE_DIALOG_LOCATIN_X	(WINDOW_SIZE_X/2 - SAVE_DIALOG_SIZE_X/2)
#    define SAVE_DIALOG_LOCATIN_Y	(WINDOW_SIZE_Y/2 - SAVE_DIALOG_SIZE_Y/2)


extern void initSaveDialog();
extern bool_t isSaveDialogActive();
extern void drawSaveDialog();
extern void eventSaveDialog();
extern void quitSaveDialog();

#endif
