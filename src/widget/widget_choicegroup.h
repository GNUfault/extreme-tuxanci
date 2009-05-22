#ifndef WIDGET_CHOICEGROUP_H
#define WIDGET_CHOICEGROUP_H

#include "main.h"
#include "list.h"
#include "widget.h"

#define WIDGET_CHOICEGROUP_TIME_SWITCH_STATUS	5

#define WIDGET_CHOICEGROUP_WIDTH	25
#define WIDGET_CHOICEGROUP_HEIGHT	25

typedef struct widget_choicegroup {
	bool_t status;
	int time;
	void (*fce_event) (void *);
	list_t *list;
} widget_choicegroup_t;

extern widget_t *choice_group_new(int x, int y, bool_t status, list_t *list, void (*fce_event) (void *));

extern void choice_group_draw(widget_t *p);
extern void choice_group_set_active(widget_t *widget);
extern bool_t choice_group_get_status(widget_t *widget);
extern void choiceGroup_set_status(widget_t *widget, bool_t status);
extern void choice_group_event(widget_t *p);
extern void choice_group_destroy(widget_t *p);

#endif /* WIDGET_CHOICEGROUP_H */
