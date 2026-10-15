#include <lcom/lcf.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../../lab1/rtc.h"
#include "../../lab2/timer.h"
#include "../../lab3/i8042.h"
#include "../../lab3/kbc.h"
#include "../../lab4/mouse.h"
#include "../../lab5/video_gr.h"
#include "../include/game/game.h"
#include "../include/game/game_loop.h"
#include "../include/init.h"
#include "../include/macros/menu.h"
#include "../include/ui/game_over_draw.h"
#include "../include/ui/menu_draw.h"
#include "../include/ui/mouse_trail.h"
#include "../include/ui/pause_menu_draw.h"
#include "../include/ui/session_timer.h"
#include "../include/ui/ui.h"

typedef enum {
    GAME_LOOP_EVENT_KEYBOARD,
    GAME_LOOP_EVENT_MOUSE,
    GAME_LOOP_EVENT_TIMER
} game_loop_event_t;

typedef struct {
    Assets assets;
    GameInfo game;
    MouseGameState mouse_state;
    GameSessionTimer session_timer;

    uint32_t timer_irq_set;
    uint32_t kbc_irq_set;
    uint32_t mouse_irq_set;

    bool assets_loaded;
    bool timer_subscribed;
    bool kbc_subscribed;
    bool mouse_ready;
    bool running;
    bool prev_lb_pressed;
    bool gameplay_active;

    uint8_t scancode[2];
    bool scancode_ready;
    int last_sushi_spawn_tick;

    uint32_t current_fps;
    uint32_t frames_since_fps_update;
    struct timespec last_fps_time;
    int last_rtc_tick;

    rtc_date status_date;
    rtc_time_t status_time;
} game_loop_ctx_t;

static int game_loop_init(game_loop_ctx_t *ctx);
static int game_loop_cleanup(game_loop_ctx_t *ctx);
static void game_loop_general_ih(game_loop_ctx_t *ctx, game_loop_event_t event);
static void game_loop_menu_ih(game_loop_ctx_t *ctx, game_loop_event_t event);
static void game_loop_game_ih(game_loop_ctx_t *ctx, game_loop_event_t event);
static void game_loop_pause_ih(game_loop_ctx_t *ctx, game_loop_event_t event);
static void game_loop_game_over_ih(game_loop_ctx_t *ctx, game_loop_event_t event);
static void game_loop_handle_keyboard_irq(game_loop_ctx_t *ctx);
static void game_loop_handle_mouse_irq(game_loop_ctx_t *ctx);
static void game_loop_handle_timer_irq(game_loop_ctx_t *ctx);
static void game_loop_enter_game(game_loop_ctx_t *ctx);
static void game_loop_enter_game_same_seed(game_loop_ctx_t *ctx, uint32_t seed);
static void game_loop_enter_menu(game_loop_ctx_t *ctx);
static void game_loop_enter_game_over(game_loop_ctx_t *ctx, int current_tick);
static void game_loop_update_fps(game_loop_ctx_t *ctx);
static void game_loop_update_status_clock(game_loop_ctx_t *ctx, int current_tick);
static void game_loop_draw_cursor(game_loop_ctx_t *ctx);
static StatusBarInfo game_loop_status(const game_loop_ctx_t *ctx);

int game_loop(void) {
    game_loop_ctx_t ctx;
    int r;
    int ipc_status;
    message msg;

    if (game_loop_init(&ctx) != 0) {
        game_loop_cleanup(&ctx);
        return 1;
    }

    while (ctx.running) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }

        if (!is_ipc_notify(ipc_status)) {
            continue;
        }

        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:
                if (msg.m_notify.interrupts & ctx.kbc_irq_set) {
                    game_loop_handle_keyboard_irq(&ctx);
                }
                if (msg.m_notify.interrupts & ctx.mouse_irq_set) {
                    game_loop_handle_mouse_irq(&ctx);
                }
                if (ctx.running && (msg.m_notify.interrupts & ctx.timer_irq_set)) {
                    game_loop_handle_timer_irq(&ctx);
                }
                break;
            default:
                break;
        }
    }

    return game_loop_cleanup(&ctx);
}

static int game_loop_init(game_loop_ctx_t *ctx) {
    uint8_t kbc_bit_no;
    uint8_t timer_bit_no;
    uint8_t mouse_bit_no;
    int current_tick;

    if (ctx == NULL) return 1;

    memset(ctx, 0, sizeof(*ctx));
    ctx->running = true;

    if (assets_init(&ctx->assets) != 0) return 1;
    ctx->assets_loaded = true;

    ctx->game.state = MENU;
    ctx->game.score = 0;
    ctx->game.lives = LIVES;
    menu_init(&ctx->game.data.menu);

    if (kbc_subscribe_int(&kbc_bit_no) != 0) return 1;
    ctx->kbc_subscribed = true;

    if (timer_subscribe_int(&timer_bit_no) != 0) return 1;
    ctx->timer_subscribed = true;

    if (mouse_init(&ctx->mouse_state, SCREEN_WIDTH, SCREEN_HEIGHT, &mouse_bit_no) != 0) return 1;
    ctx->mouse_ready = true;

    ctx->kbc_irq_set = BIT(kbc_bit_no);
    ctx->timer_irq_set = BIT(timer_bit_no);
    ctx->mouse_irq_set = BIT(mouse_bit_no);
    ctx->scancode_ready = keyboard_get_scancode(ctx->scancode);

    current_tick = get_int_counter();
    session_timer_reset(&ctx->session_timer, current_tick);
    ctx->last_rtc_tick = current_tick;
    rtc_read_date(&ctx->status_date);
    rtc_read_time(&ctx->status_time);

    if (clock_gettime(CLOCK_MONOTONIC, &ctx->last_fps_time) != 0) return 1;

    return 0;
}

static int game_loop_cleanup(game_loop_ctx_t *ctx) {
    int status = 0;

    if (ctx == NULL) return 1;

    if (ctx->timer_subscribed && timer_unsubscribe_int() != 0) status = 1;
    if (ctx->kbc_subscribed && kbc_unsubscribe_int() != 0) status = 1;
    if (ctx->mouse_ready && mouse_cleanup() != 0) status = 1;

    if (ctx->gameplay_active) {
        gameplay_clear(&ctx->game);
        ctx->gameplay_active = false;
    }

    if (ctx->assets_loaded) {
        assets_destroy(&ctx->assets);
        ctx->assets_loaded = false;
    }

    return status;
}

static void game_loop_handle_keyboard_irq(game_loop_ctx_t *ctx) {
    keyboard_ih();
    ctx->scancode_ready = keyboard_get_scancode(ctx->scancode);
    if (ctx->scancode_ready) {
        game_loop_general_ih(ctx, GAME_LOOP_EVENT_KEYBOARD);
    }
}

static void game_loop_handle_mouse_irq(game_loop_ctx_t *ctx) {
    if (mouse_proj_ih(&ctx->mouse_state, SCREEN_WIDTH, SCREEN_HEIGHT)) {
        mouse_trail_update(&ctx->mouse_state);
        game_loop_general_ih(ctx, GAME_LOOP_EVENT_MOUSE);
    }
}

static void game_loop_handle_timer_irq(game_loop_ctx_t *ctx) {
    timer_ih_proj();
    game_loop_general_ih(ctx, GAME_LOOP_EVENT_TIMER);
}

static void game_loop_general_ih(game_loop_ctx_t *ctx, game_loop_event_t event) {
    switch (ctx->game.state) {
        case MENU:
            game_loop_menu_ih(ctx, event);
            break;
        case GAME:
            game_loop_game_ih(ctx, event);
            break;
        case PAUSED:
            game_loop_pause_ih(ctx, event);
            break;
        case GAME_OVER:
            game_loop_game_over_ih(ctx, event);
            break;
        case EXIT:
            ctx->running = false;
            break;
        case SINGLEPLAYER:
        case MULTIPLAYER:
        case WAITING:
        default:
            break;
    }
}

static void game_loop_menu_ih(game_loop_ctx_t *ctx, game_loop_event_t event) {
    if (event == GAME_LOOP_EVENT_KEYBOARD && ctx->game.data.menu.entering_seed) {
        char key_pressed;

        if (!keyboard_is_scancode_make()) return;
        key_pressed = keyboard_scancode_to_ascii();

        if (key_pressed >= '0' && key_pressed <= '9') {
            menu_handle_key(&ctx->game.data.menu, key_pressed);
        }
        else if (ctx->scancode[0] == 0x0E) {
            menu_handle_key(&ctx->game.data.menu, '\b');
        }
        else if (ctx->scancode[0] == 0x1C) {
            menu_handle_key(&ctx->game.data.menu, '\n');
        }
        else if (ctx->scancode[0] == 0x01) {
            menu_handle_key(&ctx->game.data.menu, 27);
        }
    }
    else if (event == GAME_LOOP_EVENT_MOUSE) {
        bool clicked = ctx->mouse_state.lb_pressed && !ctx->prev_lb_pressed;
        MenuOption hovered = ui_menu_hit_test(ctx->mouse_state.x, ctx->mouse_state.y);
        MenuAction action;

        ctx->prev_lb_pressed = ctx->mouse_state.lb_pressed;
        action = menu_update(&ctx->game.data.menu, hovered, clicked);

        switch (action) {
            case MENU_ACTION_START_SINGLEPLAYER:
                game_loop_enter_game(ctx);
                break;
            case MENU_ACTION_EXIT:
                if (ctx->gameplay_active) {
                    session_timer_pause(&ctx->session_timer, get_int_counter());
                    gameplay_clear(&ctx->game);
                    ctx->gameplay_active = false;
                }
                ctx->running = false;
                break;
            case MENU_ACTION_ENTER_SEED:
            case MENU_ACTION_NONE:
            default:
                break;
        }
    }
    else if (event == GAME_LOOP_EVENT_TIMER) {
        int current_tick = get_int_counter();
        StatusBarInfo status;

        game_loop_update_status_clock(ctx, current_tick);
        anim_update(&ctx->assets.mascot_anim, current_tick);
        video_clear_page();
        status = game_loop_status(ctx);
        ui_menu_draw(&ctx->game.data.menu, &ctx->assets, &status);
        game_loop_draw_cursor(ctx);
        if (video_swap_pages() == 0) game_loop_update_fps(ctx);
    }
}

static void game_loop_game_ih(game_loop_ctx_t *ctx, game_loop_event_t event) {
    if (event == GAME_LOOP_EVENT_KEYBOARD) {
        char key_pressed;

        if (ctx->scancode_ready && ctx->scancode[0] == ESC_BREAK_CODE) {
            session_timer_pause(&ctx->session_timer, get_int_counter());
            ctx->game.state = PAUSED;
            pause_menu_init(&ctx->game.data.pause_menu);
            return;
        }

        if (!keyboard_is_scancode_make()) return;
        key_pressed = keyboard_scancode_to_ascii();
        if (key_pressed != 0 && gameplay_boss_active(&ctx->game)) {
            gameplay_handle_boss_key(&ctx->game,
                                     key_pressed,
                                     session_timer_ticks(&ctx->session_timer));
            if (ctx->game.state == GAME_OVER) {
                game_loop_enter_game_over(ctx, get_int_counter());
            }
        }
    }
    else if (event == GAME_LOOP_EVENT_MOUSE) {
        ctx->prev_lb_pressed = ctx->mouse_state.lb_pressed;
        gameplay_handle_cut(&ctx->game,
                            &ctx->mouse_state,
                            session_timer_ticks(&ctx->session_timer));
        if (ctx->game.state == GAME_OVER) {
            game_loop_enter_game_over(ctx, get_int_counter());
        }
    }
    else if (event == GAME_LOOP_EVENT_TIMER) {
        int current_tick = get_int_counter();

        video_clear_page();

        if (!ctx->gameplay_active) {
            game_loop_enter_game(ctx);
        }

        session_timer_update(&ctx->session_timer, current_tick);
        gameplay_update(&ctx->game,
                        (int)session_timer_ticks(&ctx->session_timer),
                        &ctx->last_sushi_spawn_tick);

        if (ctx->game.state != GAME) {
            game_loop_enter_game_over(ctx, current_tick);
            ui_game_over_draw(&ctx->game.data.game_over, &ctx->assets);
        }
        else {
            ui_draw_background(&ctx->assets.background_sprite);
            gameplay_draw(&ctx->game,
                          &ctx->assets,
                          session_timer_ticks(&ctx->session_timer));
            hud_draw(&ctx->assets.game_font,
                     ctx->game.score,
                     ctx->game.lives,
                     &ctx->assets.heart,
                     session_timer_seconds(&ctx->session_timer));
        }

        game_loop_draw_cursor(ctx);
        if (video_swap_pages() == 0) game_loop_update_fps(ctx);
    }
}

static void game_loop_pause_ih(game_loop_ctx_t *ctx, game_loop_event_t event) {
    if (event == GAME_LOOP_EVENT_MOUSE) {
        bool clicked = ctx->mouse_state.lb_pressed && !ctx->prev_lb_pressed;
        PauseOption hovered = ui_pause_menu_hit_test(ctx->mouse_state.x, ctx->mouse_state.y);
        PauseAction action;

        ctx->prev_lb_pressed = ctx->mouse_state.lb_pressed;
        action = pause_menu_update(&ctx->game.data.pause_menu, hovered, clicked);

        switch (action) {
            case PAUSE_ACTION_RESUME:
                ctx->gameplay_active = true;
                ctx->last_sushi_spawn_tick = get_int_counter();
                session_timer_start(&ctx->session_timer, ctx->last_sushi_spawn_tick);
                ctx->game.state = GAME;
                break;
            case PAUSE_ACTION_MAIN_MENU:
                game_loop_enter_menu(ctx);
                break;
            case PAUSE_ACTION_EXIT:
                gameplay_clear(&ctx->game);
                ctx->gameplay_active = false;
                ctx->running = false;
                break;
            case PAUSE_ACTION_NONE:
            default:
                break;
        }
    }
    else if (event == GAME_LOOP_EVENT_TIMER) {
        int current_tick = get_int_counter();
        StatusBarInfo status;

        game_loop_update_status_clock(ctx, current_tick);
        anim_update(&ctx->assets.mascot_anim, current_tick);
        video_clear_page();
        status = game_loop_status(ctx);
        ui_draw_background(&ctx->assets.background_sprite);
        ui_pause_menu_draw(&ctx->game.data.pause_menu, &ctx->assets, &status);
        game_loop_draw_cursor(ctx);
        if (video_swap_pages() == 0) game_loop_update_fps(ctx);
    }
}

static void game_loop_game_over_ih(game_loop_ctx_t *ctx, game_loop_event_t event) {
    if (event == GAME_LOOP_EVENT_MOUSE) {
        bool clicked = ctx->mouse_state.lb_pressed && !ctx->prev_lb_pressed;
        GameOverOption hovered = ui_game_over_hit_test(ctx->mouse_state.x, ctx->mouse_state.y);
        GameOverAction action;

        ctx->prev_lb_pressed = ctx->mouse_state.lb_pressed;
        action = game_over_update(&ctx->game.data.game_over, hovered, clicked);

        switch (action) {
            case GAME_OVER_ACTION_RESTART_SAME_SEED:
                game_loop_enter_game_same_seed(ctx, ctx->game.data.game_over.seed);
                break;
            case GAME_OVER_ACTION_MAIN_MENU:
                game_loop_enter_menu(ctx);
                break;
            case GAME_OVER_ACTION_NONE:
            default:
                break;
        }
    }
    else if (event == GAME_LOOP_EVENT_TIMER) {
        int current_tick = get_int_counter();
        anim_update(&ctx->assets.mascot_lose_anim, current_tick);
        video_clear_page();
        ui_game_over_draw(&ctx->game.data.game_over, &ctx->assets);
        game_loop_draw_cursor(ctx);
        if (video_swap_pages() == 0) game_loop_update_fps(ctx);
    }
}

static void game_loop_enter_game(game_loop_ctx_t *ctx) {
    ctx->last_sushi_spawn_tick = get_int_counter();
    gameplay_start(&ctx->game);
    ctx->gameplay_active = true;
    session_timer_reset(&ctx->session_timer, ctx->last_sushi_spawn_tick);
    session_timer_start(&ctx->session_timer, ctx->last_sushi_spawn_tick);
    ctx->game.state = GAME;
}

static void game_loop_enter_game_same_seed(game_loop_ctx_t *ctx, uint32_t seed) {
    gameplay_clear(&ctx->game);
    menu_init(&ctx->game.data.menu);
    ctx->game.data.menu.seed_val = seed;
    snprintf(ctx->game.data.menu.seed_str,
             sizeof(ctx->game.data.menu.seed_str),
             "%u",
             (unsigned)seed);
    game_loop_enter_game(ctx);
}

static void game_loop_enter_menu(game_loop_ctx_t *ctx) {
    gameplay_clear(&ctx->game);
    ctx->gameplay_active = false;
    ctx->game.state = MENU;
    menu_init(&ctx->game.data.menu);
}

static void game_loop_enter_game_over(game_loop_ctx_t *ctx, int current_tick) {
    session_timer_pause(&ctx->session_timer, current_tick);
    ctx->gameplay_active = false;
    ctx->assets.mascot_lose_anim.cur_frame = 0;
    ctx->assets.mascot_lose_anim.last_tick = current_tick;
}

static void game_loop_update_fps(game_loop_ctx_t *ctx) {
    struct timespec now;
    ctx->frames_since_fps_update++;
    if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) return;

    int64_t elapsed_ns = ((int64_t)now.tv_sec - ctx->last_fps_time.tv_sec) *
                         INT64_C(1000000000) + now.tv_nsec - ctx->last_fps_time.tv_nsec;
    if (elapsed_ns < INT64_C(1000000000)) return;

    ctx->current_fps = (uint32_t)(((uint64_t)ctx->frames_since_fps_update *
                                 UINT64_C(1000000000)) / (uint64_t)elapsed_ns);
    ctx->frames_since_fps_update = 0;
    ctx->last_fps_time = now;
}

static void game_loop_update_status_clock(game_loop_ctx_t *ctx, int current_tick) {
    if (current_tick - ctx->last_rtc_tick < 60) return;

    rtc_read_date(&ctx->status_date);
    rtc_read_time(&ctx->status_time);
    ctx->last_rtc_tick = current_tick;
}

static void game_loop_draw_cursor(game_loop_ctx_t *ctx) {
    if (ctx->game.state == GAME) {
        mouse_trail_draw(&ctx->mouse_state);
    }
    ui_draw_sprite(&ctx->assets.mouse_cursor,
                   ctx->mouse_state.x,
                   ctx->mouse_state.y);
}

static StatusBarInfo game_loop_status(const game_loop_ctx_t *ctx) {
    StatusBarInfo status = {
        .date = &ctx->status_date,
        .time = &ctx->status_time,
        .fps = ctx->current_fps
    };

    return status;
}
