#include <stdio.h>
#include <errno.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "keypad.h"
#include "display.h"

SDL_Window *window = NULL;
SDL_Texture *pixel_grid = NULL;
SDL_Renderer *renderer = NULL;
bool gfx[RESOLUTION] = {0};

int init_display() {
    int rc = 0;

    if(SDL_Init(SDL_INIT_VIDEO) == false) {
        SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        rc = -EINVAL;
    } else {
        window = SDL_CreateWindow("CHIP-8", WIDTH, HEIGHT, 0);
        if(window == NULL) {
            SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
            rc = -EINVAL;
        } else {
            renderer = SDL_CreateRenderer(window, NULL);
            if (renderer == NULL) {
                SDL_Log("Failed to create renderer: %s", SDL_GetError());
                rc = -EINVAL;
            }

            pixel_grid = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
            if (pixel_grid == NULL) {
                SDL_Log("Failed to create pixel_grid: %s", SDL_GetError());
                rc = -EINVAL;
            }
        }
    }

    return rc;
}

bool update_gfx() {
    int rc = 0;
    SDL_Event e;

    void* pixels;
    int pitch;
    if (SDL_LockTexture(pixel_grid, NULL, &pixels, &pitch)) {
        Uint32* pixel_data = (Uint32*)pixels;

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                Uint8 r,g,b,a;
                a = 255;
                if (gfx[(y/SCALE)*64 + (x/SCALE)]) {
                    r = 90;
                    g = 195;
                    b = 130;
                } else {
                    r = 4;
                    g = 8;
                    b = 6;
                }
                // RGBA8888 format: pack into single 32-bit value
                Uint32 color = (a << 0) | (b << 8) | (g << 16) | (r << 24);

                // Set pixel (pitch is in bytes, so divide by 4 for Uint32 index)
                pixel_data[y * (pitch / 4) + x] = color;
            }
        }

        SDL_UnlockTexture(pixel_grid);
    }

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_RenderTexture(renderer, pixel_grid, NULL, NULL);

    // Present to screen
    SDL_RenderPresent(renderer);

    return rc;
}

void destroy_display() {
    SDL_DestroyTexture(pixel_grid);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    window = NULL;
    pixel_grid = NULL;
    renderer = NULL;

    SDL_Quit();
    printf("debug: Window Closing\n");
}
