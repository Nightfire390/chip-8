#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#include <SDL3/SDL.h>

#define KEYS 16

extern bool keys[KEYS];

unsigned char key_press(SDL_Event);
unsigned char key_reset(SDL_Event);

#endif
