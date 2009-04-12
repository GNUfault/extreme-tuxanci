
#ifndef WIDGET_BUTTONIMAGE_H

#    include "main.h"

#    define WIDGET_BUTTONIMAGE_H

#    define WIDGET_BUTTONIMAGE_TIME	10

#    include "widget.h"

typedef struct widget_buttonimageimage {
	int w, h;
	int active;
	image_t *image;
	void (*fce_event) (void *);
} widget_buttonimage_t;

extern widget_t *button_image_new(image_t * image, int x, int y,
									  void (*fce_event) (void *));

extern void button_image_set_active(widget_t * widget, bool_t active);
extern void button_image_draw(widget_t * widget);
extern void button_image_event(widget_t * widget);
extern void button_image_destroy(widget_t * widget);

#endif
