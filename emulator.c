#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "keypad.h"
#include "display.h"
#include "emulator.h"

unsigned short opcode;
unsigned char mem[MEMORY] = {0x00};
unsigned char reg[REGISTER];
unsigned short I;
unsigned short pc;

unsigned short stack[16] = {0};
unsigned short sp = 0;

unsigned char delay_timer;
unsigned char sound_timer;

bool display_wait_flag = false;
unsigned char current_key_press = 0xff;
unsigned char current_key_reset = 0xff;

void setup_font() {
    unsigned int offset = 0x00;

    // 0
    mem[offset + 0x00] = 0xF0;
    mem[offset + 0x01] = 0x90;
    mem[offset + 0x02] = 0x90;
    mem[offset + 0x03] = 0x90;
    mem[offset + 0x04] = 0xF0;
    // 1
    mem[offset + 0x05] = 0x20;
    mem[offset + 0x06] = 0x60;
    mem[offset + 0x07] = 0x20;
    mem[offset + 0x08] = 0x20;
    mem[offset + 0x09] = 0x70;
    // 2
    mem[offset + 0x0a] = 0xF0;
    mem[offset + 0x0b] = 0x10;
    mem[offset + 0x0c] = 0xF0;
    mem[offset + 0x0d] = 0x80;
    mem[offset + 0x0e] = 0xF0;
    // 3
    mem[offset + 0x0f] = 0xF0;
    mem[offset + 0x10] = 0x10;
    mem[offset + 0x11] = 0xF0;
    mem[offset + 0x12] = 0x10;
    mem[offset + 0x13] = 0xF0;
    // 4
    mem[offset + 0x14] = 0x90;
    mem[offset + 0x15] = 0x90;
    mem[offset + 0x16] = 0xF0;
    mem[offset + 0x17] = 0x10;
    mem[offset + 0x18] = 0x10;
    // 5
    mem[offset + 0x19] = 0xF0;
    mem[offset + 0x1a] = 0x80;
    mem[offset + 0x1b] = 0xF0;
    mem[offset + 0x1c] = 0x10;
    mem[offset + 0x1d] = 0xF0;
    // 6
    mem[offset + 0x1e] = 0xF0;
    mem[offset + 0x1f] = 0x80;
    mem[offset + 0x20] = 0xF0;
    mem[offset + 0x21] = 0x90;
    mem[offset + 0x22] = 0xF0;
    // 7
    mem[offset + 0x23] = 0xF0;
    mem[offset + 0x24] = 0x10;
    mem[offset + 0x25] = 0x20;
    mem[offset + 0x26] = 0x40;
    mem[offset + 0x27] = 0x40;
    // 8
    mem[offset + 0x28] = 0xF0;
    mem[offset + 0x29] = 0x90;
    mem[offset + 0x2a] = 0xF0;
    mem[offset + 0x2b] = 0x90;
    mem[offset + 0x2c] = 0xF0;
    // 9
    mem[offset + 0x2d] = 0xF0;
    mem[offset + 0x2e] = 0x90;
    mem[offset + 0x2f] = 0xF0;
    mem[offset + 0x30] = 0x10;
    mem[offset + 0x31] = 0xF0;
    // A
    mem[offset + 0x32] = 0xF0;
    mem[offset + 0x33] = 0x90;
    mem[offset + 0x34] = 0xF0;
    mem[offset + 0x35] = 0x90;
    mem[offset + 0x36] = 0x90;
    // B
    mem[offset + 0x37] = 0xE0;
    mem[offset + 0x38] = 0x90;
    mem[offset + 0x39] = 0xE0;
    mem[offset + 0x3a] = 0x90;
    mem[offset + 0x3b] = 0xE0;
    // C
    mem[offset + 0x3c] = 0xF0;
    mem[offset + 0x3d] = 0x80;
    mem[offset + 0x3e] = 0x80;
    mem[offset + 0x3f] = 0x80;
    mem[offset + 0x40] = 0xF0;
    // D
    mem[offset + 0x41] = 0xE0;
    mem[offset + 0x42] = 0x90;
    mem[offset + 0x43] = 0x90;
    mem[offset + 0x44] = 0x90;
    mem[offset + 0x45] = 0xE0;
    // E
    mem[offset + 0x46] = 0xF0;
    mem[offset + 0x47] = 0x80;
    mem[offset + 0x48] = 0xF0;
    mem[offset + 0x49] = 0x80;
    mem[offset + 0x4a] = 0xF0;
    // F
    mem[offset + 0x4b] = 0xF0;
    mem[offset + 0x4c] = 0x80;
    mem[offset + 0x4d] = 0xF0;
    mem[offset + 0x4e] = 0x80;
    mem[offset + 0x4f] = 0x80;
}

int load_rom(char* path) {
    int rc = 0;
    FILE *rom = fopen(path, "r");

    if (rom == NULL) {
        if (errno == ENOENT)
            printf("File does not exist.\n");
        else if (errno == EACCES)
            printf("Permission denied.\n");

        return -errno;
    }

    fseek(rom, 0L, SEEK_END);
    size_t sz = ftell(rom);
    fseek(rom, 0L, SEEK_SET);

    if (sz <= USABLE) {
        printf("debug: rom validated\n");

        unsigned char b;
        pc = 0x200;

        while ((b = fgetc(rom)) != EOF && (pc - 0x200 < sz)) {
            mem[pc] = b;
            pc++;
        }

        rc = 0;
    } else {
        printf("debug: rom not validated\n");
        rc = 1;
    }

    fclose(rom);
    return rc;
}


void run_emu() {
    unsigned int now = SDL_GetTicks();
    unsigned int then = SDL_GetTicks();

    setup_font();
    srand(now);

    pc = 0x200;

    unsigned int timer_reduction = 0;
    unsigned int display_timer = 0;
    unsigned int instruction_timer = 0;

    while(true) {
        unsigned int delta = now - then;

        timer_reduction = timer_reduction + delta;
        then = now;

        if (instruction_timer < 1000/SPEED) {
            instruction_timer += delta;
            goto defer;
        }

        instruction_timer = 0;

        int rc = emu_tick();
        if (rc)
            break;

defer:
        SDL_Event e;

        while( SDL_PollEvent( &e ) == true ) {
            if( e.type == SDL_EVENT_QUIT )
                return;

            if (e.type == SDL_EVENT_KEY_DOWN) {
                char key_p = key_press(e);
                if ((opcode & 0xf0ff) == 0xf00a && current_key_press == 0xff)
                    current_key_press = key_p;
            }

            if (e.type == SDL_EVENT_KEY_UP) {
                char key_r = key_reset(e);
                if ((opcode & 0xf0ff) == 0xf00a && (current_key_reset == 0xff || current_key_reset != current_key_press))
                    current_key_reset = key_r;
            }
        }

        if(timer_reduction >= 16) {
            if (delay_timer > 0)
                delay_timer = delay_timer - (unsigned char)(timer_reduction/16);
            if (sound_timer > 0)
                sound_timer = sound_timer - (unsigned char)(timer_reduction/16);

            timer_reduction = 0;
        }

        display_timer = display_timer + delta;;
        if (display_timer > 8)
            display_wait_flag = 0;

        if ((opcode & 0xf000) == 0xd000) {
            if (display_wait_flag) {
                SDL_Delay(8 - display_timer);
                display_wait_flag = false;
            }

            update_gfx();
            display_wait_flag = true;
            display_timer = 0;
        }

        now = SDL_GetTicks();
    }
}

int emu_tick() {
    int rc = 0;
    unsigned char x, y;

    opcode = (mem[pc] << 8) | mem[pc+1];

    switch (opcode >> 12) {
        case 0x0:
            if ((opcode & 0x0f00) >> 8 != 0x0)
                printf("debug: Old machine code subroutine\n");
            if (opcode == 0x00e0)
                memset(gfx, 0, sizeof(gfx));
            else if (opcode == 0x00ee) {
                sp--;
                pc = stack[sp];
                stack[sp] = 0x0;

                break;
            } else {
                printf("debug: end-of-file\n");
                rc = -EOF;
            }

            pc = pc + 2;
            break;

        case 0x1:
            pc = (opcode & 0x0fff);
            break;

        case 0x2:
            stack[sp] = pc + 2;
            sp++;
            pc = (opcode & 0x0fff);

            break;

        case 0x3:
            x = (opcode & 0x0f00) >> 8;

            if (reg[x] == (opcode & 0x00ff))
                pc = pc + 2;
            pc = pc + 2;
            break;

        case 0x4:
            x = (opcode & 0x0f00) >> 8;

            if (reg[x] != (opcode & 0x00ff))
                pc = pc + 2;
            pc = pc + 2;
            break;

        case 0x5:
            x = (opcode & 0x0f00) >> 8;
            y = (opcode & 0x00f0) >> 4;

            if (reg[x] == reg[y])
                pc = pc + 2;
            pc = pc + 2;
            break;

        case 0x6:
            x = (opcode & 0x0f00) >> 8;

            reg[x] = opcode & 0x00ff;
            pc = pc + 2;
            break;

        case 0x7:
            x = (opcode & 0x0f00) >> 8;

            reg[x] = reg[x] + (opcode & 0x00ff);
            pc = pc + 2;
            break;

        case 0x8:
            x = (opcode & 0x0f00) >> 8;
            y = (opcode & 0x00f0) >> 4;

            if ((opcode & 0x000f) == 0x0)
                reg[x] = reg[y];
            else if ((opcode & 0x000f) == 0x1) {
                reg[x] = reg[x] | reg[y];
                reg[0xf] = 0;
            } else if ((opcode & 0x000f) == 0x2) {
                reg[x] = reg[x] & reg[y];
                reg[0xf] = 0;
            } else if ((opcode & 0x000f) == 0x3) {
                reg[x] = reg[x] ^ reg[y];
                reg[0xf] = 0;
            } else if ((opcode & 0x000f) == 0x4) {
                reg[x] = reg[x] + reg[y];
                reg[0xf] = reg[y] > reg[x];
            } else if ((opcode & 0x000f) == 0x5) {
                reg[x] = reg[x] - reg[y];
                reg[0xf] = (reg[x] + reg[y]) % 256 >= reg[y];
            } else if ((opcode & 0x000f) == 0x6) {
                unsigned char tmp = reg[y] & 0b00000001;
                reg[x] = reg[y] >> 1;
                reg[0xf] = tmp;
            } else if ((opcode & 0x000f) == 0x7) {
                unsigned char tmp = reg[y] >= reg[x];
                reg[x] = reg[y] - reg[x];
                reg[0xf] = tmp;
            } else if ((opcode & 0x000f) == 0xe) {
                unsigned char tmp = (reg[y] & 0b10000000) >> 7;
                reg[x] = reg[y] << 1;
                reg[0xf] = tmp;
            }

            pc = pc + 2;
            break;

        case 0x9:
            x = (opcode & 0x0f00) >> 8;
            y = (opcode & 0x00f0) >> 4;

            if (reg[x] != reg[y])
                pc = pc + 4;
            else
                pc = pc + 2;
            break;

        case 0xa:
            I = opcode & 0x0fff;
            pc = pc + 2;
            break;

        case 0xb:
            pc = (opcode & 0x0fff) + reg[0];
            break;

        case 0xc:
            x = (opcode & 0x0f00) >> 8;
            unsigned char r = rand();
            reg[x] = r & (opcode & 0x00ff);

            pc = pc + 2;
            break;

        case 0xd:
            x = (opcode & 0x0f00) >> 8;
            y = (opcode & 0x00f0) >> 4;

            reg[0xf] = 0x0;
            for(unsigned char ior = 0x0; ior < (opcode & 0x000f); ior++) {
                if ((reg[y] % 32) + ior >= 32)
                    break;

                unsigned int row = mem[I + ior];
                unsigned int xy = ((reg[y] % 32) + ior) * 64 + (reg[x] % 64);

                for (unsigned int ioc = 0; ioc < 8; ioc++) {
                    if ((reg[x] % 64) + ioc >= 64)
                        break;

                    bool tmp = gfx[xy + ioc];
                    gfx[xy + ioc] = gfx[xy + ioc] ^ ((row >> (7 - ioc)) & 0b00000001);

                    if (tmp && !gfx[xy + ioc])
                        reg[0xf] = 0x1;
                }
            }

            pc = pc + 2;
            break;

        case 0xe:
            x = (opcode & 0x0f00) >> 8;

            if ((opcode & 0x00ff) == 0x9e) {
                if(keys[reg[x]])
                    pc = pc + 2;
            } else if ((opcode& 0x00ff) == 0xa1) {
                if(!keys[reg[x]])
                    pc = pc + 2;
            } else
                rc = -EINVAL;

            pc = pc + 2;
            break;

        case 0xf:
            x = (opcode & 0x0f00) >> 8;

            if ((opcode & 0x00ff) == 0x07)
                reg[x] = delay_timer;
            else if ((opcode & 0x00ff) == 0x0a) {
                if (current_key_press != 0xff && current_key_press == current_key_reset) {
                    pc = pc + 2;

                    reg[x] = current_key_press;
                    current_key_press = 0xff;
                    current_key_reset = 0xff;
                } else
                    pc = pc - 2;

            }
            else if ((opcode & 0x00ff) == 0x15)
                delay_timer = reg[x];
            else if ((opcode & 0x00ff) == 0x18)
                sound_timer = reg[x];
            else if ((opcode & 0x00ff) == 0x1e)
                I = I + reg[x];
            else if ((opcode & 0x00ff) == 0x29)
                I = ((reg[x] & 0xf))*5;
            else if ((opcode & 0x00ff) == 0x33) {
                mem[I] = (reg[x]/100) % 10;
                mem[I + 1] = (reg[x]/10) % 10;
                mem[I + 2] = reg[x] % 10;
            }
            else if ((opcode & 0x00ff) == 0x55) {
                for(unsigned char tmp = 0; tmp <= ((opcode & 0x0f00) >> 8); tmp++) {
                     mem[I + tmp] = reg[tmp];
                }

                I = I + x + 1;
            } else if ((opcode & 0x00ff) == 0x65) {
                for(unsigned char tmp = 0; tmp <= ((opcode & 0x0f00) >> 8); tmp++) {
                    reg[tmp] = mem[I + tmp];
                }

                I = I + x + 1;
            }

            pc = pc + 2;
            break;

        default:
            pc = pc + 2;
            printf("debug: unsupported instruction (0x%x), skipping...\n", opcode);
    }

    return rc;
}

