#include <lcom/lcf.h>

#include <lcom/timer.h>
#include <stdbool.h>
#include "controllers/gpu/gpu.h"
#include "controllers/KBC/KBC.h"
#include "controllers/KBC/i8042.h"
#include "controllers/KBC/keyboard/keyboard.h"
#include "controllers/KBC/mouse/mouse.h"
#include "controllers/sp/sp.h"
#include "menu/menu.h"
#include "game/game.h"
#include "game/UTILS.h"
#include "menu/choose_input_method.h"
#include "menu/select_mode.h"
#include "menu/local_or_remote.h"

extern uint8_t scancode;
int game_state = STATE_MENU;
uint8_t irq_set_keyboard, irq_set_timer, irq_set_mouse, irq_set_sp;

//ssh lcom@127.0.0.1 -p 2222

int main(int argc, char* argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");
    
    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
    
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

int setup() {
    if (keyboard_subscribe_int(&irq_set_keyboard) != 0) return 1;
    if (timer_subscribe_int(&irq_set_timer) != 0) return 1;
    if (mouse_subscribe_int(&irq_set_mouse) != 0) return 1;
    if (sp_subscribe_int(&irq_set_sp) != 0) return 1;
    if (vg_init(0x14C) == NULL) return 1;
    return 0;
}

int teardown() {
    freebuffer();
    if (keyboard_unsubscribe_int() != 0) return 1;
    if (timer_unsubscribe_int() != 0) return 1;
    if (mouse_unsubscribe_int() != 0) return 1;
    if (sp_unsubscribe_int() != 0) return 1;
    return vg_exit();
}

int (proj_main_loop)(int argc, char* argv[]) {
    if (setup() != 0) return 1;

    int option = 0;
    bool isMultiplayer = false;
    bool isClassic = false;
    while (game_state != STATE_EXIT) {
        switch (game_state) {
            case STATE_MENU:
                game_state = run_menu(&isMultiplayer); 
                break;
            case STATE_GAME_SINGLEPLAYER:
                game_state = run_select_mode(&isClassic); 
                break;
            case STATE_SELECTMODE:
                game_state = run_choose_input_method(&option, isMultiplayer, isClassic); 
                break;
            case STATE_GAME_SINGLEPLAYER_IO: {
                game_state = run_game(option);
                break;
            }
            case STATE_GAME_SINGLEPLAYER_CLASSIC: {
                game_state = run_game_classic(option);
                break;
            }
            case STATE_GAME_MULTIPLAYER: {
                game_state = run_local_or_remote();
                break;
            }
            case STATE_GAME_MULTIPLAYER_LOCAL: {
                game_state = run_game_multiplayer_local(2); 
                break;
            }
            case STATE_GAME_MULTIPLAYER_REMOTE: {
                bool isRemote = (strcmp(argv[0], "remote") == 0) ? true : false;
                game_state = run_game_multiplayer(option, isRemote); 
                break;
            }
            default:
                game_state = STATE_EXIT;
                break;
        }
    }
    
    return teardown();
}
