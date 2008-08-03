
#include <stdlib.h>
#include <assert.h>
#include <SDL.h>

#include "main.h"
#include "list.h"
#include "myTimer.h"
#include "image.h"
#include "serverSendMsg.h"

#include "net_multiplayer.h"
#include "screen_world.h"
#include "proto.h"
#include "interface.h"
#include "chat.h"
#include "font.h"
#include "keyboardBuffer.h"

static image_t *g_chat;

static list_t *listText;

static char line[STR_SIZE];

static int line_time, line_atime;
static int timeBlick;

static bool_t chat_active;
static bool_t receivedNewMsg;

static my_time_t lastActive;

void initChat()
{
	g_chat = addImageData("chat.png", IMAGE_NO_ALPHA, "chat", IMAGE_GROUP_USER);

	listText = newList();
	strcpy(line, "");
	chat_active = FALSE;
	receivedNewMsg = FALSE;
	line_time = 0;
	line_atime = 0;
	timeBlick = 0;
	lastActive = 0;
}

void drawChat()
{
	char str[STR_SIZE];
	int i;

	if( chat_active == FALSE )
	{
		return;
	}

	drawImage(g_chat,
		CHAT_LOCATION_X, CHAT_LOCATION_Y,
		0, 0, CHAT_SIZE_X, CHAT_SIZE_Y);

	for( i = 0 ; i < listText->count ; i++ )
	{
		char *s;

		s = (char *)listText->list[i];

		drawFontMaxSize(s,
			CHAT_LOCATION_X+5, CHAT_LOCATION_Y+5 + i*20,
			CHAT_SIZE_X-10, 20, COLOR_WHITE);
	}

	strcpy(str, line);

	if( timeBlick > CHAT_TIME_BLICK_CURSOR/2 )
	{
		strcat(str, "\f");
	}

	timeBlick++;

	if( timeBlick == CHAT_TIME_BLICK_CURSOR )
	{
		timeBlick = 0;
	}

	drawFont(str, CHAT_LOCATION_X+5, CHAT_LOCATION_Y+5 + CHAT_MAX_LINES*20, COLOR_WHITE);
}

static void processMessageKey(SDL_keysym keysym)
{
	int w, h;
	int len;
	//int i;

	if (line_atime < 100)
		line_atime ++;

	len = strlen(line);
	getTextSize(line, &w, &h);

	if(  w > CHAT_SIZE_X-40 )
	{
		return;
	}

	/* zpracovani backspace */
	if (keysym.sym == SDLK_BACKSPACE){
		line[len-1]='\0';
		return;
	}

	/* zpracovani tisknutelnych paznaku */
	if ( (keysym.sym >= SDLK_SPACE && keysym.sym <= SDLK_AT)
			|| (keysym.sym >= SDLK_LEFTBRACKET && keysym.sym <= SDLK_BACKQUOTE) ){
		line[len] = keysym.sym;	/*  tady by bylo daleko lepsi dat strcat,
							predpoklada se, ze line je vyplnen 0 az dokonce */
	}

	/* zpracovani pismenek */
	if ( keysym.sym >= SDLK_a && keysym.sym <= SDLK_z){
		char c = keysym.sym;
		if (keysym.mod == KMOD_SHIFT)
			c = toupper(c);
		line[len] = c;	/*  tady by bylo daleko lepsi dat strcat,
							predpoklada se, ze line je vyplnen 0 az dokonce */
	}

	return;
}

static void switchChatActive()
{
	if( chat_active == TRUE )
	{
		chat_active = FALSE;
	}
	else
	{
		chat_active = TRUE;
	}
}

static void sendNewMessage()
{
	if( getNetTypeGame() == NET_GAME_TYPE_CLIENT )
	{
		proto_send_chat_client(line);
	}

	if( getNetTypeGame() == NET_GAME_TYPE_SERVER )
	{
		char out[STR_PROTO_SIZE];

		snprintf(out, STR_PROTO_SIZE, "chat %s:%s\n",
			getControlTux(TUX_CONTROL_KEYBOARD_RIGHT)->name, line);

		proto_send_chat_server(PROTO_SEND_ALL, NULL, out);
	}
}

static void eventChatDisable()
{ 
	Uint8 *mapa;
	mapa = SDL_GetKeyState(NULL);

	if( mapa[SDLK_RETURN] == SDL_PRESSED )
	{ /* chat neni aktivni, tak jej aktivujeme */
		receivedNewMsg = FALSE;
		switchChatActive();
		memset(line, '\0', STR_SIZE);
		/* zapneme zachytavani klaves do bufferu a vycistime buffer */
		enableKeyboardBuffer();
		clearKeyboardBuffer();
	}
}

static void eventChatEnable()
{
	/* Mame zobrazene okno chatu. Je potreba zpracovat vsechny klavesy v bufferu
	 * s tim, ze pri klavese RETURN provedeme odeslani radku chatu
	 * na server
	 */

	while( isAnyKeyInKeyboardBuffer() == TRUE )
	{
		SDL_keysym key;
		key = popKeyFromKeyboardBuffer();

		if ( key.sym == SDLK_RETURN )
		{
			if ( strcmp(line, "") != 0 )
			{ /* mame napsany radek: je potreba jej odeslat */
				sendNewMessage();
				memset(line, '\0', STR_SIZE);
				continue;	/* pokracovat s dalsimi klavesami */
			}
			else
			{ /* radek je prazdny, je potreba vypnout okno chatu */
				switchChatActive();
				memset(line, '\0', STR_SIZE);
				/* vypneme zachytavani klaves do bufferu a vycistime buffer */
				disableKeyboardBuffer();
				clearKeyboardBuffer();
			}
		}

		processMessageKey(key);
	}
}

/**
 * Zpracovani "udalosti" chatu?
 */
void eventChat()
{
	if (isChatActive() == FALSE)
	{ /* okno chatu neni aktivni */
		eventChatDisable();
	}
	else
	{
		eventChatEnable();
	}
}

bool_t isChatActive()
{
	return chat_active;
}

bool_t isRecivedNewMsg()
{
	return receivedNewMsg;
}

void addToChat(char *s)
{
	addList(listText, strdup(s) );

	if( listText->count > CHAT_MAX_LINES )
	{
		delListItem(listText, 0, free);
	}

	if( chat_active == FALSE )
	{
		receivedNewMsg = TRUE;
	}
}

void quitChat()
{
	assert( listText != NULL );
	destroyListItem(listText, free);
}
