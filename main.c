#include <stdio.h>
#include <errno.h>

#include "keypad.h"
#include "display.h"
#include "emulator.h"

void help(void);

int main(int argc, char** argv) {
    int rc = 0;
    FILE *rom = NULL;

    if (argc < 2) {
        help();
        goto cleanup;
    }


    rc = load_rom(argv[1]);
    if (rc)
        goto cleanup;

    rc = init_display();
    if (rc)
        goto cleanup;

    run_emu();
    // printf("debug: Success\n");

cleanup:
    destroy_display();
    // close_emulator();

    return rc;
}

void help() {
    printf("CHIP-8 EMULATOR\n");
    printf("Usage:\n");
    printf("\t./chip8 <file>\n");
}
