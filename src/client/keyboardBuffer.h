/*
 * keyboardBuffer.h
 *
 *  Created on: 1.8.2008
 *      Author: Karel Podvolecky
 *
 *  Jednoducha fronta stisknutych klaves.
 */

#ifndef KEYBOARD_BUFFER_H

#    define KEYBOARD_BUFFER_H

#    include <SDL.h>
#    include "main.h"

typedef struct {
	SDL_keysym *buff;			/* buffer klaves (kod dle SDLKey enumu) */
	int size;					/* aktualni velikost alokovaneho bufferu (v poctu uchovatelnych klaves) */
	int count;					/* pocet klaves aktualne v bufferu */
	int begin;					/* ukazatel na zacatek bufferu (tj. kam se pridavaji klavesy) */
	int end;					/* ukazatel na konec bufferu (tj. odkud se odebiraji klavesy) */
} keyboardBuffer_t;

extern void keyboard_buffer_init(int size);
extern void keyboard_buffer_clear();
extern bool_t keyboard_buffer_push(SDL_keysym key);
extern SDL_keysym keyboard_buffer_pop();
extern int keyboard_buffer_get_size();
extern int keyboard_buffer_get_count();
extern bool_t keyboard_buffer_is_any_key();
extern void keyboard_buffer_quit();

#endif							/* KEYBOARDBUFFER_H_ */
