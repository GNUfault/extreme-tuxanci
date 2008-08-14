
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "main.h"
#include "list.h"
#include "director.h"
#include "homeDirector.h"
#include "net_multiplayer.h"
#include "tux.h"

#include "language.h"
#include "interface.h"
#include "screen.h"
#include "image.h"

#ifndef NO_SOUND
#include "music.h"
#endif

#include "screen_gameType.h"
#include "screen_setting.h"

#include "widget_label.h"
#include "widget_button.h"
#include "widget_image.h"
#include "widget_textfield.h"
#include "widget_choicegroup.h"
#include "widget_select.h"

static widget_image_t *image_backgorund;

static widget_button_t *button_back;
static widget_button_t *button_play;
static widget_button_t *button_browser;

static widget_label_t *label_none;
static widget_label_t *label_server;
static widget_label_t *label_client;
static widget_label_t *label_load_session;

static widget_label_t *label_ip;
static widget_label_t *label_port;
static widget_label_t *label_session;

static list_t *listChoiceGroup;
static widget_choicegroup_t *check_none;
static widget_choicegroup_t *check_server;
static widget_choicegroup_t *check_client;
static widget_choicegroup_t *check_load_session;

static widget_textfield_t *textfield_ip;
static widget_textfield_t *textfield_port;

static widget_select_t *selectSession;

void startScreenGameType()
{
#ifndef NO_SOUND
	playMusic("menu", MUSIC_GROUP_BASE);
#endif

	check_none->status = TRUE;
	check_server->status = FALSE;
	check_client->status = FALSE;
}

void drawScreenGameType()
{
	int i;

	drawWidgetImage(image_backgorund);
	
	for( i = 0 ; i < listChoiceGroup->count ; i++ )
	{
		widget_choicegroup_t *this;

		this = (widget_choicegroup_t *)listChoiceGroup->list[i];
		drawWidgetChoicegroup(this);
	}

	drawWidgetLabel(label_none);
	drawWidgetLabel(label_server);
	drawWidgetLabel(label_client);
	drawWidgetLabel(label_load_session);

	if( check_server->status == TRUE || check_client->status == TRUE )
	{
		drawWidgetLabel(label_port);
		drawWidgetTextfield(textfield_port);
		drawWidgetLabel(label_ip);
		drawWidgetTextfield(textfield_ip);
	}

	if( check_load_session->status == TRUE )
	{
		drawWidgetLabel(label_session);
		drawWidgetSelect(selectSession);
	}

#if 0
	if( check_server->status == TRUE )
	{
		setSettingIP (getParamElse ("--ip", "127.0.0.1"));
		setSettingPort (atoi (getParamElse("--port", "2200")));
	}
#endif

	if( check_client->status == TRUE )
		drawWidgetButton(button_browser);
	
	drawWidgetButton(button_back);
	drawWidgetButton(button_play);

}

void eventScreenGameType()
{
	int i;

	for( i = 0 ; i < listChoiceGroup->count ; i++ )
	{
		widget_choicegroup_t *this;

		this = (widget_choicegroup_t *)listChoiceGroup->list[i];
		eventWidgetChoicegroup(this);
	}

	if( check_server->status == TRUE || check_client->status == TRUE )
	{
		eventWidgetTextfield(textfield_ip);
		eventWidgetTextfield(textfield_port);
	}

	if( check_load_session->status == TRUE )
	{
		eventWidgetSelect(selectSession);
	}

	eventWidgetButton(button_back);
	eventWidgetButton(button_play);

	if( check_client->status == TRUE )
		eventWidgetButton(button_browser);
}

void stopScreenGameType()
{
}

static void eventWidget(void *p)
{
	widget_button_t *button;
	
	button = (widget_button_t *)(p);

	if( button == button_back )
	{
		setScreen("mainMenu");
	}

	if( button == button_play )
	{
		char *str;

		str = getGemeTypeLoadSession();

		if( str != NULL )
		{
			setScreen("world");
			return;
		}

		if( getSettingGameType() == NET_GAME_TYPE_CLIENT )
		{
			setScreen("world");
		}
		else
		{
			setScreen("chiceArena");
		}
	}

	if( button == button_browser )
	{
		setScreen("browser");
	}
}

static void loadSession(widget_select_t *p)
{
	director_t *director;
	int i;

	director = loadDirector(getHomeDirector());

	if( director == NULL )
	{
		return;
	}

	for( i = 0 ; i < director->list->count ; i++ )
	{
		char *line;

		line = director->list->list[i];

		if( strstr(line, ".sav") != NULL )
		{
			addToWidgetSelect(p, strdup(line));
		}
	}

	destroyDirector(director);
}

void initScreenGameType()
{
	image_t *image;

	image = getImage(IMAGE_GROUP_BASE, "screen_main");
	image_backgorund  = newWidgetImage(0, 0, image);

	button_back = newWidgetButton(getMyText("BACK"), 100, WINDOW_SIZE_Y-100, eventWidget);
	button_play = newWidgetButton(getMyText("PLAY"), WINDOW_SIZE_X-200, WINDOW_SIZE_Y-100, eventWidget);
	button_browser = newWidgetButton(getMyText("BROWSER"), 300, 345, eventWidget);

	listChoiceGroup = newList();
	check_none = newWidgetChoicegroup(100, 150, FALSE, listChoiceGroup, eventWidget);
	check_server = newWidgetChoicegroup(100, 200, FALSE, listChoiceGroup, eventWidget);
	check_client = newWidgetChoicegroup(100, 250, FALSE, listChoiceGroup, eventWidget);
	check_load_session = newWidgetChoicegroup(100, 300, FALSE, listChoiceGroup, eventWidget);

	if( isParamFlag("--server") )
	{
		check_server->status = TRUE;
	}
	else if( isParamFlag("--client") )
	{
		check_client->status = TRUE;
	}
	else
	{
		check_none->status = TRUE;
	}

	label_none = newWidgetLabel(getMyText("NONE"), 130, 145, WIDGET_LABEL_LEFT);
	label_server = newWidgetLabel(getMyText("SERVER"), 130, 195, WIDGET_LABEL_LEFT);
	label_client = newWidgetLabel(getMyText("CLIENT"), 130, 245, WIDGET_LABEL_LEFT);
	label_load_session = newWidgetLabel("load session", 130, 295, WIDGET_LABEL_LEFT);

	label_ip = newWidgetLabel(getMyText("IP_ADDR"), 300, 145, WIDGET_LABEL_LEFT);
	label_port = newWidgetLabel(getMyText("NET_PORT"), 300, 245, WIDGET_LABEL_LEFT);
	label_session = newWidgetLabel("load session :", 300, 145, WIDGET_LABEL_LEFT);

	textfield_ip = newWidgetTextfield(
		getParamElse("--ip", "127.0.0.1"),
		WIDGET_TEXTFIELD_FILTER_IP_OR_DOMAIN,
		300, 180);
	
	textfield_port = newWidgetTextfield(
		getParamElse("--port", "2200"),
		WIDGET_TEXTFIELD_FILTER_NUM,
		300, 280);

	selectSession = newWidgetSelect(300, 175, eventWidget);
	loadSession(selectSession);

	registerScreen( newScreen("gameType", startScreenGameType, eventScreenGameType,
		drawScreenGameType, stopScreenGameType) );
}

int setSettingGameType(int status)
{
	check_none->status = status;

	return 0;
}

int getSettingGameType()
{
	if( check_none->status == TRUE  ||
	    check_load_session->status == TRUE )
	{
		return NET_GAME_TYPE_NONE;
	}

	if( check_server->status == TRUE )
	{
		return NET_GAME_TYPE_SERVER;
	}

	if( check_client->status == TRUE )
	{
		return NET_GAME_TYPE_CLIENT;
	}

	assert( ! "Chyba pri zistvani type  hry !" );

	return -1;
}

char* getGemeTypeLoadSession()
{
	if( check_load_session->status == TRUE )
	{
		return getWidgetSelectItem(selectSession);
	}

	return NULL;
}

int setSettingIP(char *address)
{
	strcpy (textfield_ip->text, address);

	return 1;
}

char* getSettingIP()
{
	return textfield_ip->text;
}

int setSettingPort(int port)
{
	sprintf (textfield_port->text, "%d", port);

	return 0;
}

int getSettingPort()
{
	return atoi( textfield_port->text );
}

int getSettingProto()
{
	if( strstr(textfield_ip->text, ".") != NULL )
	{
		return PROTO_UDPv4;
	}

	if( strstr(textfield_ip->text, ":") != NULL )
	{
		return PROTO_UDPv6;
	}

	printf("IP protokol unknown !\n");
	return -1;
}

void quitScreenGameType()
{
	destroyWidgetImage(image_backgorund);

	destroyWidgetLabel(label_none);
	destroyWidgetLabel(label_server);
	destroyWidgetLabel(label_client);
	destroyWidgetLabel(label_load_session);

	destroyWidgetLabel(label_ip);
	destroyWidgetLabel(label_port);
	destroyWidgetLabel(label_session);

	destroyWidgetTextfield(textfield_ip);
	destroyWidgetTextfield(textfield_port);

	destroyWidgetSelect(selectSession);

	destroyWidgetButton(button_back);
	destroyWidgetButton(button_play);
	destroyWidgetButton(button_browser);

	destroyWidgetChoicegroup(check_none);
	destroyWidgetChoicegroup(check_server);
	destroyWidgetChoicegroup(check_client);
	destroyWidgetChoicegroup(check_load_session);
	
	destroyList(listChoiceGroup);
}

