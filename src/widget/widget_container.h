#ifndef WIDGET_CONTAINER_H
#define WIDGET_CONTAINER_H

#include "list.h"
#include "widget.h"

typedef struct widget_container {
	list_t *listWidget;
} widget_container_t;

extern widget_t* widget_container_new(int x, int y, int w, int h);
extern void widget_container_add(widget_t *widget, widget_t *widget_add);
extern void widget_set_layout_table(widget_t *widget, int countW, int countH, int w, int h);
extern void widget_container_draw(widget_t *widget);
extern void widget_container_event(widget_t *widget);
extern void widget_container_destroy(widget_t *widget);

#endif /* WIDGET_CONTAINER_H */
