#include "keypad.h"

bool keys[KEYS] = {0};

unsigned char key_press(SDL_Event e) {
    switch (e.key.scancode) {
        case SDL_SCANCODE_1:
            keys[0x1] = 1;
            return 0x1;
        case SDL_SCANCODE_2:
            keys[0x2] = 1;
            return 0x2;
        case SDL_SCANCODE_3:
            keys[0x3] = 1;
            return 0x3;
        case SDL_SCANCODE_4:
            keys[0xc] = 1;
            return 0xc;
        case SDL_SCANCODE_Q:
            keys[0x4] = 1;
            return 0x4;
        case SDL_SCANCODE_W:
            keys[0x5] = 1;
            return 0x5;
        case SDL_SCANCODE_E:
            keys[0x6] = 1;
            return 0x6;
        case SDL_SCANCODE_R:
            keys[0xd] = 1;
            return 0xd;
        case SDL_SCANCODE_A:
            keys[0x7] = 1;
            return 0x7;
        case SDL_SCANCODE_S:
            keys[0x8] = 1;
            return 0x8;
        case SDL_SCANCODE_D:
            keys[0x9] = 1;
            return 0x9;
        case SDL_SCANCODE_F:
            keys[0xe] = 1;
            return 0xe;
        case SDL_SCANCODE_Z:
            keys[0xa] = 1;
            return 0xa;
        case SDL_SCANCODE_X:
            keys[0x0] = 1;
            return 0x0;
        case SDL_SCANCODE_C:
            keys[0xb] = 1;
            return 0xb;
        case SDL_SCANCODE_V:
            keys[0xf] = 1;
            return 0xf;
        default:
            return 0xff;
    }
}

unsigned char key_reset(SDL_Event e) {
    switch (e.key.scancode) {
        case SDL_SCANCODE_1:
            keys[0x1] = 0;
            return 0x1;
        case SDL_SCANCODE_2:
            keys[0x2] = 0;
            return 0x2;
        case SDL_SCANCODE_3:
            keys[0x3] = 0;
            return 0x3;
        case SDL_SCANCODE_4:
            keys[0xc] = 0;
            return 0xc;
        case SDL_SCANCODE_Q:
            keys[0x4] = 0;
            return 0x4;
        case SDL_SCANCODE_W:
            keys[0x5] = 0;
            return 0x5;
        case SDL_SCANCODE_E:
            keys[0x6] = 0;
            return 0x6;
        case SDL_SCANCODE_R:
            keys[0xd] = 0;
            return 0xd;
        case SDL_SCANCODE_A:
            keys[0x7] = 0;
            return 0x7;
        case SDL_SCANCODE_S:
            keys[0x8] = 0;
            return 0x8;
        case SDL_SCANCODE_D:
            keys[0x9] = 0;
            return 0x9;
        case SDL_SCANCODE_F:
            keys[0xe] = 0;
            return 0xe;
        case SDL_SCANCODE_Z:
            keys[0xa] = 0;
            return 0xa;
        case SDL_SCANCODE_X:
            keys[0x0] = 0;
            return 0x0;
        case SDL_SCANCODE_C:
            keys[0xb] = 0;
            return 0xb;
        case SDL_SCANCODE_V:
            keys[0xf] = 0;
            return 0xf;
        default:
            return 0xff;
    }
}

