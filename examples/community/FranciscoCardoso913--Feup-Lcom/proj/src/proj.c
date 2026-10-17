#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "game.h"

int main(int argc, char* argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/proj/src/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv)) return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

int (proj_main_loop)(int argc, char* argv[]) {
    if (argc != 1 || (strcmp(argv[0], "1") != 0 && strcmp(argv[0], "2") != 0)) {
        printf("Usage: lcom_run proj 1|2. Being 1|2 the player for multiplayer mode\n");
        return 1;
    }

    if (game_init(atoi(argv[0]))) {
        printf("Error initializing game.\n");
        return 1;
    }

    if (game_loop()) {
        printf("Error running game loop.\n");
        return 1;
    }

    if (game_exit()) {
        printf("Error ending game.\n");
        return 1;
    }

    return 0;
}
