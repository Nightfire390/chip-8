#ifndef _EMULATOR_H_
#define _EMULATOR_H_

#define MEMORY 4096
#define USABLE (MEMORY - 0x200 - 352)
#define REGISTER 16

#define SPEED 1000

extern unsigned short opcode;
extern unsigned char mem[MEMORY];
extern unsigned char reg[REGISTER];
extern unsigned short I;
extern unsigned short pc;

extern unsigned short stack[16];
extern unsigned short sp;

extern unsigned char delay_timer;
extern unsigned char sound_timer;

void run_emu();
int load_rom(char*);
int emu_tick();

#endif
