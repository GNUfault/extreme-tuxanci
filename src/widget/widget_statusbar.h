#ifndef WIDGET_STATUSBAR_H
#define WIDGET_STATUSBAR_H

#include "main.h"
#include "widget.h"
#include "list.h"

typedef struct widget_statusbar {
	list_t *listElement;
	int selectElement;
} widget_statusbar_t;

#define WIDGET_STATUSBAR_NONE_SELECT_ELEMENT	-1

#define WIDGET_STATUSBAR_WIDTH	250
#define WIDGET_STATUSBAR_HEIGHT	65

extern widget_t *statusbar_new(int x, int y);
extern void statusbar_add(widget_t *widget, widget_t *widget_catch, char *text);
extern void statusbar_draw(widget_t *widget);
extern void statusbar_event(widget_t *widget);
extern void statusbar_destroy(widget_t *widget);

#endif /* WIDGET_STATUSBAR_H */
