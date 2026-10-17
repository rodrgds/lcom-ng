#include <lcom/lcf.h>

#include "game/game.c"

GameState gameState = MENU;

int (main)(int argc, char *argv[]) {
    lcf_set_language("EN-US");
    lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
    lcf_log_output("/home/lcom/labs/proj/output.txt");
    if (lcf_start(argc, argv)) return 1;

    lcf_cleanup();

    return 0;
}

int (proj_main_loop)(int argc, char **argv) {
    if (config_frame_buffer(VBE_800x600_MODE) != 0) return 1;
    if (vg_set_mode(VBE_800x600_MODE) != 0) return 1;

    allocate_buffers();

    load_xpms();

    if (run_game() != 0) return 1;

    destroy_xpms();

    deallocate_buffers();

    if (vg_exit() != 0) return 1;

    return 0;
}
