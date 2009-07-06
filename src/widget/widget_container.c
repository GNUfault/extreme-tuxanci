#include <stdlib.h>
#include <assert.h>

#include "main.h"
#include "widget.h"
#include "widget_container.h"

widget_t* widget_container_new(int x, int y, int w, int h)
{
	widget_container_t *new;

	new = malloc(sizeof(widget_container_t));
	new->listWidget = list_new();

	return widget_new(WIDGET_TYPE_CONTAINER, x, y, w, h, new);
}

void widget_container_add(widget_t *widget, widget_t *widget_add)
{
	widget_container_t *p;

	assert(widget != NULL);
	assert(widget->type == WIDGET_TYPE_CONTAINER);

	p = (widget_container_t *) widget->private_data;
	list_add(p->listWidget, widget_add);
}

void widget_set_layout_table(widget_t *widget, int countW, int countH, int w, int h)
{
	widget_container_t *p;
	int i, j;
	int count;

	assert(widget != NULL);
	assert(widget->type == WIDGET_TYPE_CONTAINER);

	p = (widget_container_t *) widget->private_data;

	count = 0;

	for (i = 0; i < countH; i++) {
		for (j = 0; j < countW; j++) {
			widget_t *widget_tmp;

			if (count >= p->listWidget->count) {
				break;
			}

			widget_tmp = (widget_t *) p->listWidget->list[count];
	
			widget_set_location(widget_tmp, widget->x + j * w, widget->y + i * h);
			count++;
		}
	}
}

void widget_container_draw(widget_t *widget)
{
	widget_container_t *p;
	int i;

	assert(widget != NULL);
	assert(widget->type == WIDGET_TYPE_CONTAINER);

	p = (widget_container_t *) widget->private_data;

	for (i = 0; i < p->listWidget->count; i++) {
		widget_t *widget;

		widget = (widget_t *) p->listWidget->list[i];

		widget_draw(widget);
	}
}

void widget_container_event(widget_t *widget)
{
	widget_container_t *p;
	int i;

	assert(widget != NULL);
	assert(widget->type == WIDGET_TYPE_CONTAINER);

	p = (widget_container_t *) widget->private_data;

	for (i = 0; i < p->listWidget->count; i++) {
		widget_t *widget;

		widget = (widget_t *) p->listWidget->list[i];

		widget_event(widget);
	}
}

void widget_container_destroy(widget_t *widget)
{
	widget_container_t *p;

	assert(widget != NULL);
	assert(widget->type == WIDGET_TYPE_CONTAINER);

	p = (widget_container_t *) widget->private_data;
	list_destroy(p->listWidget);
	free(p);

	widget_destroy(widget);
}
