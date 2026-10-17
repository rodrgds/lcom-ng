#include "proj.h"


int (proj_main_loop)(int argc, char *argv[]) {
    Game game;
    game.score = 0;
    game.curState = GAME_STATE_MENU;

    if (keyboard_subscribe_int(&kb_mask) != 0) return 1;
    if (timer_subscribe_int(&timer_mask) != 0) return 1;
    if (mouse_subscribe_int(&mouse_mask) != 0) return 1;
    if (mouse_write_cmd(ENABLE_DATA_REPORT) != 0) return 1;

    message msg;
    int ipc_status;

    while (game.curState != GAME_STATE_GAMEOVER) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;

        if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {
            if (msg.m_notify.interrupts & kb_mask) kbc_ih();
            if (msg.m_notify.interrupts & mouse_mask) proj_mouse_ih();
        }

        switch (game.curState) {
            case GAME_STATE_MENU:
                run_menu(&game, kb_mask); break;
            case GAME_STATE_INSTRUCTIONS:
                draw_instructions(&game, kb_mask); break;
            case GAME_STATE_LEADERBOARD:
                // TODO: draw_leaderboard();
                game.curState = GAME_STATE_MENU;
                break;
            case GAME_STATE_GAME:
                run(&game,kb_mask, timer_mask, mouse_mask); break;
            case GAME_STATE_EXIT:
                game.curState = GAME_STATE_GAMEOVER;
                break;
            default:
                game.curState = GAME_STATE_GAMEOVER;
                break;
        }
    }

    mouse_write_cmd(DISABLE_DATA_REPORT);
    keyboard_unsubscribe_int();
    timer_unsubscribe_int();
    mouse_unsubscribe_int();
    set_text_mode();

    return 0;
}

