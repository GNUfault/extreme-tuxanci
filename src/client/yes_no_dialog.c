#include <stdlib.h>
#include <assert.h>

#include "main.h"
#include "list.h"

#include "interface.h"
#include "hotKey.h"
#include "yes_no_dialog.h"
#include "image.h"
#include "font.h"

#include "widget.h"
#include "widget_label.h"
#include "widget_button.h"

static image_t *g_background;

static widget_t *widgetLabelMsg;

static widget_t *widgetButtonYes;
static widget_t *widgetButtonNo;

static void (*handle_yes)(void *param);
static void (*handle_no)(void *param);
static void *param;
static bool_t active_dialog;

static void eventWidget(void *p)
{
	widget_t *button;

	button = (widget_t *) p;

	yes_no_dialog_set_active(FALSE);

	if (button == widgetButtonYes) {
		handle_yes(param);
	}

	if (button == widgetButtonNo) {
		handle_no(param);
	}
}

void yes_no_dialog_init()
{
	active_dialog = FALSE;

	g_background = image_get(IMAGE_GROUP_BASE, "screen_main");

	widgetLabelMsg = label_new("empty label", WINDOW_SIZE_X / 2,
				   YES_NO_DIALOG_LOCATIN_Y + 20,
				   WIDGET_LABEL_CENTER);

	widgetButtonYes = button_new(_("Yes"), YES_NO_DIALOG_LOCATIN_X + 20,
				      YES_NO_DIALOG_LOCATIN_Y + 60,
				      eventWidget);

	widgetButtonNo = button_new(_("No"), YES_NO_DIALOG_LOCATIN_X + 20 +
						 WIDGET_BUTTON_WIDTH + 20,
				      YES_NO_DIALOG_LOCATIN_Y + 60,
				      eventWidget);
}

static void hotkey_escape()
{
	yes_no_dialog_set_active(FALSE);
	handle_no(param);
}

void yes_no_dialog_set_active(bool_t active)
{
	active_dialog = active;

	if (active_dialog == TRUE)
	{
		hot_key_register(SDLK_ESCAPE, hotkey_escape);
	}
	else
	{
		hot_key_unregister(SDLK_ESCAPE);
	}
}

bool_t yes_no_dialog_is_active()
{
	return active_dialog;
}

void yes_no_dialog_set(char *label, void *function_yes, void *function_no, void *p_param)
{
	label_destroy(widgetLabelMsg);

	widgetLabelMsg = label_new(label, WINDOW_SIZE_X / 2,
				   YES_NO_DIALOG_LOCATIN_Y + 20,
				   WIDGET_LABEL_CENTER);

	handle_yes = function_yes;
	handle_no = function_no;
	param = p_param;
}

void yes_no_dialog_draw()
{
	if (active_dialog == FALSE) {
		return;
	}

	image_draw(g_background, YES_NO_DIALOG_LOCATIN_X,
				 YES_NO_DIALOG_LOCATIN_Y,
				 YES_NO_DIALOG_LOCATIN_X,
				 YES_NO_DIALOG_LOCATIN_Y,
				 YES_NO_DIALOG_SIZE_X,
				 YES_NO_DIALOG_SIZE_Y);

	label_draw(widgetLabelMsg);
	button_draw(widgetButtonYes);
	button_draw(widgetButtonNo);
}

void yes_no_dialog_event()
{
	if (active_dialog == FALSE) {
		return;
	}

	button_event(widgetButtonYes);
	button_event(widgetButtonNo);
}

void yes_no_dialog_quit()
{
	label_destroy(widgetLabelMsg);
	button_destroy(widgetButtonYes);
	button_destroy(widgetButtonNo);
}
