#include <lcom/lcf.h>
#include <stdint.h>

#include "src/config.h"

#include "src/controller/keyboard-mouse/kbc.h"
#include "src/controller/keyboard-mouse/mouse.h"
#include "src/controller/keyboard-mouse/i8042.h"
#include "src/controller/video/graphics.h"
#include "src/view/menu.h"
#include "src/view/font.h"
#include "src/game/game.h"
#include "src/game/assets_loader.h"
#include "src/game/scoreboard.h"

int main(int argc, char* argv[]) {
    lcf_set_language("EN-US");
    lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
    lcf_log_output("/home/lcom/labs/proj/output.txt");

    if (lcf_start(argc, argv)) return 1;
    
    lcf_cleanup();
    return 0;
}

int (start_up_routine)(uint8_t *kb_irq_set, uint8_t *mouse_irq_set, uint8_t *timer_irq_set) {
    write_command_to_mouse(ENABLE_MOUSE_DATA_REPORTING);

    if (mouse_subscribe_int(mouse_irq_set) != 0) return 1;
    if (kb_subscribe_int(kb_irq_set) != 0) return 1;
    if (timer_subscribe_int(timer_irq_set) != 0) return 1;
    if (vram_start(SCREEN_RES_MODE) != 0) return 1;
    if (set_video_mode(SCREEN_RES_MODE) != 0) return 1;
    if (load_font() != 0) return 1;
    set_mouse_positions(get_h_res() / 2, get_v_res() / 2);
    
    return 0;
}

int (shutdown_routine)() {
    kb_unsubscribe_int();
    mouse_unsubscribe_int();
    timer_unsubscribe_int();
    write_command_to_mouse(DISABLE_MOUSE_DATA_REPORTING);
    destroy_font();
    vg_exit();
    
    return 0;
}

int (proj_main_loop)(int argc, char* argv[]) {
    uint8_t kb_irq_set, mouse_irq_set, timer_irq_set;

    if (start_up_routine(&kb_irq_set, &mouse_irq_set, &timer_irq_set) != 0) return 1;

    bool keep_playing = true;
    
    while (keep_playing) {
        current_note_speed = BASE_SPEED;
        int menu_res = menu_run(kb_irq_set, mouse_irq_set, timer_irq_set);
        if (menu_res != 0) break;

        int game_completed = game_run(kb_irq_set, mouse_irq_set, timer_irq_set);
        if (game_completed) get_scoreboard(kb_irq_set, mouse_irq_set, timer_irq_set, score);

        destroy_sprites();
        delete_static_background();
    }

    if (shutdown_routine() != 0) return 1;

    return 0;
}
