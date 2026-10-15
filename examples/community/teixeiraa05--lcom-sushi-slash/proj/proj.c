#include <lcom/lcf.h>
#include <stdio.h>

#include "../lab5/video_gr.h"
#include "../lab5/vesa-bios.h"
#include "include/game/game_loop.h"

int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

#ifdef DEBUG_LOG
    // Call tracing is opt-in because it adds overhead to interrupt handling.
    lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
#endif

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/proj/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {
    int loop_status;
    int cleanup_status = 0;

    (void)argc;
    (void)argv;

    if (video_init_page_flipping(MODE_115) != 0) {
        printf("Failed to initialize page-flipping\n");
        vg_exit();
        return 1;
    }

    loop_status = game_loop();

    if (video_cleanup_page_flipping() != 0) {
        cleanup_status = 1;
    }
    if (vg_exit() != 0) {
        cleanup_status = 1;
    }

    return loop_status != 0 || cleanup_status != 0;
}
