#ifndef YES_NO_DIALOG_H
#define YES_NO_DIALOG_H

#define YES_NO_ACTIVE_TIME_INTERVAL	500

#define YES_NO_DIALOG_SIZE_X		320
#define YES_NO_DIALOG_SIZE_Y		120

#define YES_NO_DIALOG_LOCATIN_X		(WINDOW_SIZE_X / 2 - YES_NO_DIALOG_SIZE_X / 2)
#define YES_NO_DIALOG_LOCATIN_Y		(WINDOW_SIZE_Y / 2 - YES_NO_DIALOG_SIZE_Y / 2)

extern void yes_no_dialog_init();
extern void yes_no_dialog_set_active(bool_t active);
extern bool_t yes_no_dialog_is_active();
extern void yes_no_dialog_set(char *label, void *function_yes, void *function_no, void *p_param);
extern void yes_no_dialog_draw();
extern void yes_no_dialog_event();
extern void yes_no_dialog_quit();

#endif /* YES_NO_DIALOG_H */
