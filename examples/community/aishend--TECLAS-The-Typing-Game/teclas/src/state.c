#include <lcom/lcf.h>
#include <stdlib.h>
#include <time.h>

#include "state.h"
#include "i8254.h"
#include "kbc.h"
#include "mouse.h"
#include "video.h"
#include "cursor.h"
#include "font.h"
#include "config.h"
#include "states/fast_teclas_state.h"
#include "states/fast_teclas_end_state.h"
#include "states/survival.h"
#include "states/config_state.h"
#include "states/start_state.h"
#include "states/main_menu_state.h"
#include "states/game_over_state.h"
#include "states/dino.h"
#include "states/leaderboard_state.h"
#include "states/config_state_survival.h"

#define VIDEO_MODE 0x118
#define KEY_SPACE_MAKE 0x39
#define FADE_TICKS 90

uint8_t get_scancode_byte();

static GameConfig current_config = {1, 0, "player"};
static GameConfig survival_config = {1, 0, "player"};

static struct { bool active; int tick; GameState next; } fade;

static void start_fade(GameState next) {
    video_start_dissolve();
    fade.active = true;
    fade.tick = 0;
    fade.next = next;
}

static void draw_state_content(GameState state) {
    switch (state) {
        case START_SCREEN:
            start_state_draw();
            break;
        case MAIN_MENU:
            main_menu_state_draw();
            break;
        case TYPING_TEST:
            fast_teclas_state_draw();
            break;
        case FAST_TECLAS_END:
            fast_teclas_end_state_draw();
            break;
        case CONFIG:
            config_state_draw(&current_config);
            break;
        case SURVIVAL_CONFIG:
            config_state_survival_draw(&survival_config);
            break;
        case DINO_GAME:
            dino_game_draw();
            break;
        case SURVIVAL_GAME:
            survival_draw();
            break;
        case LEADERBOARD:
            leaderboard_state_draw();
            break;
        case GAME_OVER:
            game_over_state_draw();
            break;
        default:
            break;
    }
}

static void tick_and_draw(GameState *state) {
    if (fade.active) {
        fade.tick++;
        if (fade.next == EXIT) video_clear(0x000000);
        else draw_state_content(fade.next);

        cursor_draw();
        video_draw_dissolve(fade.tick, FADE_TICKS);
        video_flip();

        if (fade.tick >= FADE_TICKS) {
            *state = fade.next;
            fade.active = false;
        }

        return;
    }

    if (*state == TYPING_TEST && fast_teclas_state_tick(current_config)) start_fade(FAST_TECLAS_END);

    if (*state == SURVIVAL_GAME) survival_tick();

    if (*state == DINO_GAME) dino_game_tick();

    draw_state_content(*state);
    cursor_draw();
    video_flip();
}

static void handle_mouse_click(GameState *state, GameConfig *cfg) {
    if (*state == MAIN_MENU) {
        GameState next = main_menu_state_handle_click();
        if (next == *state) return;
        if (next == TYPING_TEST) { start_fade(CONFIG); return; }
        if (next == SURVIVAL_GAME) { survival_reset(); survival_set_config(current_config); start_fade(SURVIVAL_CONFIG); return; }
        if (next == DINO_GAME) { dino_game_reset(); start_fade(DINO_GAME); return; }
        if (next == LEADERBOARD) leaderboard_state_reload();

        start_fade(next);

        return;
    }
    if (*state == FAST_TECLAS_END) {
        if (fast_teclas_end_state_handle_click()) {
            fast_teclas_state_reset(current_config);
            start_fade(TYPING_TEST);
        }
        return;
    }
    if (*state == TYPING_TEST) {
        if (fast_teclas_state_handle_click()) start_fade(CONFIG);

        return;
    }
    if (*state == CONFIG) {
        bool confirmed = config_state_handle_click(cfg);
        if (confirmed) {
            fast_teclas_state_reset(*cfg);
            start_fade(TYPING_TEST);
        }
    }
    if (*state == SURVIVAL_CONFIG) {
        bool confirmed = config_state_survival_handle_click(&survival_config);
        if (confirmed) {
            survival_set_config(survival_config);
            start_fade(SURVIVAL_GAME);
        }
    }
    if (*state == DINO_GAME) {
        dino_game_handle_click();
    }
    if (*state == SURVIVAL_GAME) {
        survival_handle_click();
    }
}

static void handle_key(uint8_t sc, GameState *state, GameConfig *cfg) {
    if (*state == CONFIG) {
        if (sc == ESC_BREAK_CODE) { start_fade(MAIN_MENU); return; }

        bool confirmed = config_state_handle_key(sc, cfg);
        if (confirmed) {
            fast_teclas_state_reset(*cfg);
            start_fade(TYPING_TEST);
        }
        return;
    }
    if (*state == SURVIVAL_CONFIG) {
        if (sc == ESC_BREAK_CODE) { start_fade(MAIN_MENU); return; }

        bool confirmed = config_state_survival_handle_key(sc, &survival_config);
        if (confirmed) {
            survival_set_config(survival_config);
            start_fade(SURVIVAL_GAME);
        }
        return;
    }
    if (sc == ESC_BREAK_CODE) {
        GameState next = (*state == START_SCREEN || *state == MAIN_MENU) ? EXIT : MAIN_MENU;
        start_fade(next);
        return;
    }
    if (sc == KEY_SPACE_MAKE && *state == START_SCREEN) {
        start_fade(MAIN_MENU);
        return;
    }
    if (*state == TYPING_TEST) {
        fast_teclas_state_handle_key(sc);
    }
    if (*state == SURVIVAL_GAME) {
        survival_handle_key(sc);
    }
    if (*state == DINO_GAME) {
        dino_game_handle_key(sc);
        return;
    }
}

int start_game_loop() {
    srand((unsigned int)time(NULL));
    GameState current_state = START_SCREEN;

    int ipc_status;
    message msg;
    uint8_t timer_bit, kbd_bit, mouse_bit;

    if (timer_subscribe_int(&timer_bit) != 0) return 1;
    if (timer_set_frequency(0, 60) != 0) return 1;
    if (kbd_subscribe_int(&kbd_bit) != 0) return 1;
    if (mouse_subscribe_int(&mouse_bit) != 0) return 1;
    if (mouse_disable_int() != 0) return 1;
    if (my_mouse_enable_data_reporting() != 0) return 1;
    if (mouse_enable_int() != 0) return 1;

    if (video_init(VIDEO_MODE) != 0) return 1;
    cursor_init();

    if (font_get() == NULL) return 1;

    if (start_state_init() != 0) return 1;
    if (main_menu_state_init() != 0) return 1;
    if (fast_teclas_state_init() != 0) return 1;
    if (survival_init() != 0) return 1;
    if (dino_game_init() != 0) return 1;
    if (leaderboard_state_init() != 0) return 1;

    uint32_t timer_irq_set = BIT(timer_bit);
    uint32_t kbd_irq_set = BIT(kbd_bit);
    uint32_t mouse_irq_set = BIT(mouse_bit);
    bool lb_prev = false;

    while (current_state != EXIT) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;
        if (!is_ipc_notify(ipc_status)) continue;
        if (_ENDPOINT_P(msg.m_source) != HARDWARE) continue;

        uint32_t intr = msg.m_notify.interrupts;

        if (intr & mouse_irq_set) {
            mouse_ih();
            if (byte_tracking == 3) {
                struct packet pkt;
                mouse_parse_packet(&pkt);
                byte_tracking = 0;
                cursor_move(pkt.delta_x, pkt.delta_y);
                if (pkt.lb && !lb_prev && !fade.active) handle_mouse_click(&current_state, &current_config);
                lb_prev = pkt.lb;
            }
        }

        if (intr & kbd_irq_set) {
            kbc_ih();
            if (!fade.active) handle_key(get_scancode_byte(), &current_state, &current_config);
        }

        if (intr & timer_irq_set) {
            timer_int_handler();
            tick_and_draw(&current_state);
        }
    }

    cursor_destroy();
    font_destroy();
    start_state_destroy();
    main_menu_state_destroy();
    survival_destroy();
    dino_game_destroy();
    leaderboard_state_destroy();

    if (mouse_disable_int() != 0) return 1;
    if (my_mouse_disable_data_reporting() != 0) return 1;
    if (mouse_unsubscribe_int() != 0) return 1;
    if (kbd_unsubscribe_int() != 0) return 1;
    if (timer_unsubscribe_int() != 0) return 1;

    vg_exit();
    return 0;
}
