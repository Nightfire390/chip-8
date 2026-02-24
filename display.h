#include <SDL3/SDL.h>

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#define RESOLUTION 2048
#define SCALE 10
#define HEIGHT (32*SCALE)
#define WIDTH (64*SCALE)

extern SDL_Window *window;
extern SDL_Texture *pixel_grid;
extern SDL_Renderer *renderer;
extern bool gfx[RESOLUTION];

int init_display(void);
bool update_gfx(void);
void destroy_display(void);

#endif
