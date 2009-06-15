#include <stdlib.h>
#include <assert.h>

#include "main.h"

#include "widget.h"
#include "widget_label.h"
#include "widget_button.h"
#include "widget_buttonimage.h"
#include "widget_textfield.h"
#include "widget_catchkey.h"
#include "widget_check.h"
#include "widget_choicegroup.h"
#include "widget_image.h"
#include "widget_select.h"
#include "widget_statusbar.h"

widget_t *widget_new(int type, int x, int y, int w, int h, void *private_data)
{
	widget_t *new;

	new = malloc(sizeof(widget_t));
	new->type = type;
	new->x = x;
	new->y = y;
	new->w = w;
	new->h = h;
	new->private_data = private_data;

	return new;
}

void widget_set_location(widget_t *p, int x, int y)
{
	p->x = x;
	p->y = y;
}

void widget_get_location(widget_t *p, int *x, int *y)
{
	if (x != NULL) {
		*x = p->x;
	}

	if (y != NULL) {
		*y = p->y;
	}
}

void widget_get_size(widget_t *p, int w, int h)
{
	p->w = w;
	p->h = h;
}

void widget_set_size(widget_t *p, int *w, int *h)
{
	if (w != NULL) {
		*w = p->w;
	}

	if (h != NULL) {
		*h = p->h;
	}
}

void widget_draw(widget_t *p)
{
	switch (p->type)
	{
		case WIDGET_TYPE_LABEL: label_draw(p); break;
		case WIDGET_TYPE_BUTTON: button_draw(p); break;
		case WIDGET_TYPE_BUTTONIMAGE: button_image_draw(p); break;
		case WIDGET_TYPE_TEXTFILED: text_field_draw(p); break;
		case WIDGET_TYPE_CATCHKEY: catch_key_draw(p); break;
		case WIDGET_TYPE_CHECK: check_draw(p); break;
		case WIDGET_TYPE_CHOICE: choice_group_draw(p); break;
		case WIDGET_TYPE_IMAGE: wid_image_draw(p); break;
		case WIDGET_TYPE_SELECT: select_draw(p); break;
		case WIDGET_TYPE_STATUSBAR: statusbar_draw(p); break;
	}
}

void widget_event(widget_t *p)
{
	switch (p->type)
	{
		case WIDGET_TYPE_LABEL: label_event(p); break;
		case WIDGET_TYPE_BUTTON: button_event(p); break;
		case WIDGET_TYPE_BUTTONIMAGE: button_image_event(p); break;
		case WIDGET_TYPE_TEXTFILED: text_field_event(p); break;
		case WIDGET_TYPE_CATCHKEY: catch_key_event(p); break;
		case WIDGET_TYPE_CHECK: check_event(p); break;
		case WIDGET_TYPE_CHOICE: choice_group_event(p); break;
		case WIDGET_TYPE_IMAGE: wid_image_event(p); break;
		case WIDGET_TYPE_SELECT: select_event(p); break;
		case WIDGET_TYPE_STATUSBAR: statusbar_event(p); break;
	}
}

void widget_destroy(widget_t *p)
{
	free(p);
}
