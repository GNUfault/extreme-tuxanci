
#include <stdlib.h>
#include <assert.h>

#include "main.h"
#include "interface.h"
#include "image.h"
#include "widget_choicegroup.h"

widget_choicegroup_t* newWidgetChoicegroup(int x, int y, bool_t status, list_t *list, void (*fce_event)(void *))
{
	widget_choicegroup_t *new;

	new = malloc( sizeof(widget_choicegroup_t) );
	new->x = x;
	new->y = y;
	new->time = 0;
	new->status = status;
	new->fce_event = fce_event;
	new->list = list;
	addList(new->list, new);

	return new;
}

void drawWidgetChoicegroup(widget_choicegroup_t *p)
{
	static image_t *g_choicegroup = NULL;
	int x, y;
	int offset;

	getMousePosition(&x, &y);

	if( g_choicegroup == NULL )
	{
		g_choicegroup = addImageData("check.png", IMAGE_ALPHA, "choicegroup", IMAGE_GROUP_BASE);
	}

	if( p->status == TRUE )
	{
		offset = 0;
	}
	else
	{
		offset = WIDGET_CHOICEGROUP_WIDTH;
	}

	drawImage(g_choicegroup, p->x, p->y, offset, 0, WIDGET_CHOICEGROUP_WIDTH, WIDGET_CHOICEGROUP_HEIGHT);
}

static void setActive(widget_choicegroup_t *p)
{
	int i;

	for( i = 0 ; i < p->list->count ; i++ )
	{
		widget_choicegroup_t *this;

		this = (widget_choicegroup_t *) p->list->list[i];

		if( this == p )
		{
			this->status = TRUE;
			this->time = WIDGET_CHOICEGROUP_TIME_SWITCH_STATUS;
			this->fce_event(p);
		}
		else
		{
			this->status = FALSE;
		}
	}
}

void eventWidgetChoicegroup(widget_choicegroup_t *p)
{
	int x, y;

	if( p->time > 0 )
	{
		p->time--;
		return;
	}

	getMousePosition(&x, &y);

	if( x >= p->x && x <= p->x+WIDGET_CHOICEGROUP_WIDTH && 
	    y >= p->y && y <= p->y+WIDGET_CHOICEGROUP_HEIGHT &&
	    isMouseClicked() )
	{
		setActive(p);
	}
}

void destroyWidgetChoicegroup(widget_choicegroup_t *p)
{
	int index;

	index = searchListItem(p->list, p);
	assert( index != -1 );
	delList(p->list, index);

	free(p);
}
