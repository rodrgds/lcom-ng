#include "state_machine.h"

#include <math.h>
#include <stddef.h>

#include <lcom/lcf.h>

#include "../../engine/input/input.h"
#include "../../engine/graphics/renderer.h"
#include "../cursor/cursor_viewer.h"
#include "../../menu/menu.h"
#include "../../menu/pause_menu.h"
#include "../../menu/controls.h"
#include "../../menu/splash_menu.h"
#include "../game_viewer.h"
#include "../game_controller.h"
#include "../worm/worm_controller.h"
#include "../weapons/bazooka/bazooka_controller.h"
#include "../weapons/grenade/grenade_controller.h"
#include "../hud/hud_controller.h"
#include "../../config.h"
#include "../../menu/game_over.h"
#include "../terrain/terrain_controller.h"

static GameViewer *viewer;
static int menu_cooldown = 0;
static int pause_cooldown = 0;
static int controls_cooldown = 0;
static double aim_angle = 0.0;
static int aim_power = 0;
static bool space_held = false;

static GameState controls_origin = STATE_MENU;

static GameState state_splash(GameContext *context);
static GameState state_menu(GameContext *context);
static GameState state_controls(GameContext *context);
static GameState state_game(GameContext *context);
static GameState state_aim(GameContext *context);
static GameState state_flight(GameContext *context);
static GameState state_explosion(GameContext *context);
static GameState state_turn_end(GameContext *context);
static GameState state_pause(GameContext *context);
static GameState state_game_over(GameContext *context);

typedef GameState (*StateFunction)(GameContext *);

static StateFunction dispatch[STATE_COUNT] = {
    [STATE_SPLASH] = state_splash,
    [STATE_MENU] = state_menu,
    [STATE_CONTROLS] = state_controls,
    [STATE_GAME] = state_game,
    [STATE_AIM] = state_aim,
    [STATE_FLIGHT] = state_flight,
    [STATE_EXPLOSION] = state_explosion,
    [STATE_TURN_END] = state_turn_end,
    [STATE_PAUSE] = state_pause,
    [STATE_GAME_OVER] = state_game_over,
};

static void sync_cursor_from_input(GameContext *context) {
    Cursor *cursor = game_context_get_cursor(context);
    if (cursor == NULL) return;

    cursor_set_position(cursor, input_get_mouse_x(), input_get_mouse_y());
}

static void collect_events(EventQueue *eventQueue, uint32_t irqs, uint32_t kbd_bit, uint32_t mouse_bit) {
    GameEvent event;

    if (irqs & kbd_bit) {
        input_handle_keyboard();
    }

    if (irqs & mouse_bit) {
        int16_t px = input_get_mouse_x(), py = input_get_mouse_y();
        bool plb = input_is_mouse_lb(), prb = input_is_mouse_rb();

        input_handle_mouse();

        int16_t nx = input_get_mouse_x(), ny = input_get_mouse_y();
        bool nlb = input_is_mouse_lb(), nrb = input_is_mouse_rb();

        if (nx != px || ny != py) {
            event.type = EVENT_MOUSE_MOVE;
            event.data.mouse.x = nx;
            event.data.mouse.y = ny;
            event.data.mouse.lb = nlb;
            event.data.mouse.rb = nrb;
            event_queue_push(eventQueue, &event);
        }
        if (nlb != plb || nrb != prb) {
            event.type = EVENT_MOUSE_BTN;
            event.data.mouse.x = nx;
            event.data.mouse.y = ny;
            event.data.mouse.lb = nlb;
            event.data.mouse.rb = nrb;
            event_queue_push(eventQueue, &event);
        }
    }
}

void state_machine_run(GameContext *context) {
    if (context == NULL) return;

    viewer = game_viewer_create();
    if (viewer == NULL) return;

    uint8_t kbd_bit, mouse_bit, timer_bit;
    if (input_init(&kbd_bit, &mouse_bit) != 0) {
        game_viewer_destroy(viewer);
        return;
    }
    if (timer_subscribe_int(&timer_bit) != 0) {
        input_cleanup();
        game_viewer_destroy(viewer);
        return;
    }

    uint32_t irq_kbd = BIT(kbd_bit);
    uint32_t irq_mouse = BIT(mouse_bit);
    uint32_t irq_timer = BIT(timer_bit);

    int r, ipc_status;
    message msg;

    EventQueue *eventQueue = game_context_get_queue(context);
    GameState cur = STATE_SPLASH;
    splash_menu_reset();

    bool run = true;
    while (run) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) continue;

        if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {
            collect_events(eventQueue, msg.m_notify.interrupts, irq_kbd, irq_mouse);
            if (input_is_key_pressed(KEY_Q)) run = false;

            if (msg.m_notify.interrupts & irq_timer) {
                timer_int_handler();

                GameEvent event;
                event.type = EVENT_TICK;
                event_queue_push(eventQueue, &event);

                GameState next = dispatch[cur](context);
                if (next != cur) {
                    if (next == STATE_MENU) menu_cooldown = PAUSE_COOLDOWN_TICKS;
                    if (next == STATE_PAUSE) pause_cooldown = PAUSE_COOLDOWN_TICKS;
                    if (next == STATE_CONTROLS) controls_cooldown = PAUSE_COOLDOWN_TICKS;
                }
                cur = next;
                if (cur == STATE_QUIT) run = false;
            }
        }
    }

    timer_unsubscribe_int();
    input_cleanup();
    game_viewer_destroy(viewer);
    viewer = NULL;
}

static GameState state_splash(GameContext *context) {
    EventQueue *queue = game_context_get_queue(context);
    GameEvent event;
    while (!event_queue_empty(queue)) event_queue_pop(queue, &event);

    if (splash_menu_update() == SPLASH_ACTION_DONE ||
        input_is_key_pressed(KEY_ENTER)) {
        return STATE_MENU;
    }

    splash_menu_draw();
    return STATE_SPLASH;
}

static GameState state_menu(GameContext *context) {
    EventQueue *queue = game_context_get_queue(context);
    static int nav_cooldown = 0;
    if (menu_cooldown > 0) menu_cooldown--;
    if (nav_cooldown > 0) nav_cooldown--;

    bool lb_click = false;
    int nav_delta = 0;
    bool confirm = false;

    GameEvent event;
    while (!event_queue_empty(queue)) {
        event_queue_pop(queue, &event);
        if (event.type == EVENT_MOUSE_BTN && event.data.mouse.lb) lb_click = true;
    }

    if (menu_cooldown == 0) {
        if (nav_cooldown == 0) {
            if (input_is_key_pressed(KEY_UP))        { nav_delta = -1; nav_cooldown = MENU_NAV_COOLDOWN_TICKS; }
            else if (input_is_key_pressed(KEY_DOWN)) { nav_delta =  1; nav_cooldown = MENU_NAV_COOLDOWN_TICKS; }
        }
        confirm = input_is_key_pressed(KEY_ENTER) || input_is_key_pressed(KEY_SPACE);
    } else {
        lb_click = false;
    }

    MenuAction action = menu_update(input_get_mouse_x(), input_get_mouse_y(), lb_click, nav_delta, confirm);

    sync_cursor_from_input(context);
    renderer_clear();
    menu_draw();
    cursor_viewer_draw(game_context_get_cursor(context));
    renderer_flip();

    if (action == MENU_ACTION_PLAY) return STATE_GAME;
    if (action == MENU_ACTION_CONTROLS) { controls_origin = STATE_MENU; return STATE_CONTROLS; }
    if (action == MENU_ACTION_EXIT) return STATE_QUIT;
    return STATE_MENU;
}

static GameState state_controls(GameContext *context) {
    EventQueue *queue = game_context_get_queue(context);

    if (controls_cooldown > 0) controls_cooldown--;

    bool lb_click = false;

    GameEvent event;
    while (!event_queue_empty(queue)) {
        event_queue_pop(queue, &event);
        if (event.type == EVENT_MOUSE_BTN && event.data.mouse.lb) lb_click = true;
    }

    if (controls_cooldown > 0) {
        sync_cursor_from_input(context);
        renderer_clear();
        controls_draw();
        cursor_viewer_draw(game_context_get_cursor(context));
        renderer_flip();
        return STATE_CONTROLS;
    }

    if (input_is_key_pressed(KEY_ESC)) return controls_origin;

    bool confirm = input_is_key_pressed(KEY_ENTER) || input_is_key_pressed(KEY_SPACE);
    ControlsAction action = controls_update(input_get_mouse_x(), input_get_mouse_y(), lb_click, confirm);

    sync_cursor_from_input(context);
    renderer_clear();
    controls_draw();
    cursor_viewer_draw(game_context_get_cursor(context));
    renderer_flip();

    if (action == CONTROLS_ACTION_BACK) return controls_origin;
    return STATE_CONTROLS;
}

static GameState state_game(GameContext *context) {
    terrain_controller_init();
    game_context_reset(context);
    aim_angle = 0.0;
    aim_power = 0;
    space_held = false;
    return STATE_AIM;
}

static void reset_aim(Worm *active_worm) {
    if (active_worm == NULL) return;
    aim_angle = (worm_get_direction(active_worm) == WORM_DIR_RIGHT) ? 0.0 : M_PI;
    aim_power = 0;
    space_held = false;
}

static GameState handle_aim_click(GameContext *context, Cursor *cursor, Worm **active_worm, WormState *wormState) {
    if (context == NULL || cursor == NULL || active_worm == NULL || wormState == NULL) return STATE_AIM;
    if (!cursor_consume_left_click(cursor)) return STATE_AIM;

    WeaponType prev_weapon = (*active_worm != NULL) ? worm_get_weapon(*active_worm) : WEAPON_NONE;
    if (hud_controller_handle_click(context, cursor_get_x(cursor), cursor_get_y(cursor))) {
        if (*active_worm != NULL && worm_get_weapon(*active_worm) != prev_weapon) {
            reset_aim(*active_worm);
            *wormState = worm_get_state(*active_worm);
        }
        return STATE_AIM;
    }

    if (*wormState == WORM_WEAPON_PULL) {
        return STATE_AIM;
    }

    if (game_context_select_worm_at_position(context, cursor_get_x(cursor), cursor_get_y(cursor)) == 0) {
        if (*active_worm != NULL) worm_set_vx(*active_worm, 0);
        *active_worm = game_context_get_active_worm(context);
        if (*active_worm == NULL || !worm_is_alive(*active_worm)) return STATE_AIM;
        *wormState = worm_get_state(*active_worm);
    }

    return STATE_AIM;
}

static void handle_weapon_keys(Worm *active_worm, WormState *wormState) {
    if (active_worm == NULL) return;

    static bool prev_key1 = false, prev_key2 = false;

    bool key1 = input_is_key_pressed(KEY_1);
    bool key2 = input_is_key_pressed(KEY_2);
    if (key1 && !prev_key1) worm_controller_apply_command(active_worm, WORM_COMMAND_EQUIP_BAZOOKA);
    if (key2 && !prev_key2) worm_controller_apply_command(active_worm, WORM_COMMAND_EQUIP_GRENADE);
    if ((key1 && !prev_key1) || (key2 && !prev_key2)) {
        reset_aim(active_worm);
    }
    prev_key1 = key1;
    prev_key2 = key2;

    if (wormState != NULL) *wormState = worm_get_state(active_worm);
}

static void handle_active_worm_movement(Worm *active_worm, WormState wormState) {
    if (active_worm == NULL) return;
    if (wormState == WORM_WEAPON_PULL || wormState == WORM_WEAPON_HOLD || wormState == WORM_WEAPON_RETRACT) return;

    worm_controller_apply_keys(active_worm);
}

static GameState state_aim(GameContext *context) {
    EventQueue *queue = game_context_get_queue(context);
    Cursor *cursor = game_context_get_cursor(context);
    Worm *active_worm = game_context_get_active_worm(context);

    GameEvent event;
    while (!event_queue_empty(queue)) {
        event_queue_pop(queue, &event);
        game_controller_handle_event(context, &event);
    }

    if (input_is_key_pressed(KEY_ESC)) return STATE_PAUSE;
    if (active_worm == NULL || !worm_is_alive(active_worm)) return STATE_TURN_END;

    WormState wormState = worm_get_state(active_worm);
    GameState click_result = handle_aim_click(context, cursor, &active_worm, &wormState);
    if (click_result != STATE_AIM) return click_result;

    handle_weapon_keys(active_worm, &wormState);

    if (wormState == WORM_WEAPON_PULL || wormState == WORM_WEAPON_HOLD || wormState == WORM_WEAPON_RETRACT) {
        if (worm_get_weapon(active_worm) == WEAPON_GRENADE) {
            if (grenade_controller_handle_aim(context, active_worm, &aim_angle, &aim_power, &space_held))
                return STATE_FLIGHT;
        } else {
            if (bazooka_controller_handle_aim(context, active_worm, &aim_angle, &aim_power, &space_held))
                return STATE_FLIGHT;
        }
    }

    handle_active_worm_movement(active_worm, wormState);

    if (game_context_ticks_left(context) == 0) {
        if (active_worm != NULL) worm_set_vx(active_worm, 0);
        return STATE_TURN_END;
    }

    if (cursor != NULL) {
        cursor_set_position(cursor, input_get_mouse_x(), input_get_mouse_y());
    }
    game_viewer_draw_aimed(viewer, context, aim_angle, aim_power);
    return STATE_AIM;
}

static GameState state_flight(GameContext *context) {
    EventQueue *queue = game_context_get_queue(context);

    GameEvent event;
    while (!event_queue_empty(queue)) {
        event_queue_pop(queue, &event);
        game_controller_handle_event(context, &event);
    }

    Bazooka *bazooka = game_context_get_bazooka(context);
    Grenade *grenade = game_context_get_grenade(context);
    bool any_active = false;
    if (bazooka != NULL && bazooka_is_active(bazooka)) any_active = true;
    if (grenade != NULL && grenade_is_active(grenade)) any_active = true;

    if (!any_active) return STATE_EXPLOSION;

    sync_cursor_from_input(context);
    game_viewer_draw(viewer, context);
    return STATE_FLIGHT;
}

static GameState state_explosion(GameContext *context) {
    EventQueue *queue = game_context_get_queue(context);

    GameEvent event;
    while (!event_queue_empty(queue)) {
        event_queue_pop(queue, &event);
        game_controller_handle_event(context, &event);
    }

    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        Explosion *e = game_context_get_explosion(context, i);
        if (e != NULL && explosion_is_active(e)) {
            game_viewer_draw(viewer, context);
            return STATE_EXPLOSION;
        }
    }

    return STATE_TURN_END;
}

static GameState state_turn_end(GameContext *context) {
    int living_teams = 0;
    for (int team = 0; team < MAX_TEAMS; team++) {
        if (game_context_is_team_alive(context, team)) living_teams++;
    }
    if (living_teams <= 1) return STATE_GAME_OVER;

    Worm *active = game_context_get_active_worm(context);
    if (active != NULL) {
        WormState st = worm_get_state(active);
        if (st == WORM_WEAPON_HOLD || st == WORM_WEAPON_PULL || st == WORM_WEAPON_RETRACT) {
            worm_set_state(active, WORM_IDLE);
        }
    }

    game_context_next_team_turn(context);
    return STATE_AIM;
}

static GameState state_pause(GameContext *context) {
    EventQueue *queue = game_context_get_queue(context);
    static int nav_cooldown = 0;

    if (pause_cooldown > 0) pause_cooldown--;
    if (nav_cooldown > 0) nav_cooldown--;

    bool lb_click = false;
    int nav_delta = 0;
    bool confirm = false;

    GameEvent event;
    while (!event_queue_empty(queue)) {
        event_queue_pop(queue, &event);
        if (event.type == EVENT_MOUSE_BTN && event.data.mouse.lb) lb_click = true;
    }

    if (pause_cooldown == 0) {
        if (input_is_key_pressed(KEY_ESC)) return STATE_AIM;
        if (nav_cooldown == 0) {
            if (input_is_key_pressed(KEY_UP))        { nav_delta = -1; nav_cooldown = MENU_NAV_COOLDOWN_TICKS; }
            else if (input_is_key_pressed(KEY_DOWN)) { nav_delta =  1; nav_cooldown = MENU_NAV_COOLDOWN_TICKS; }
        }
        confirm = input_is_key_pressed(KEY_ENTER) || input_is_key_pressed(KEY_SPACE);
    }

    PauseAction action = pause_menu_update(input_get_mouse_x(), input_get_mouse_y(), lb_click, nav_delta, confirm);

    sync_cursor_from_input(context);
    game_viewer_draw_scene(viewer, context);
    pause_menu_draw();
    cursor_viewer_draw(game_context_get_cursor(context));
    renderer_flip();

    if (action == PAUSE_ACTION_RESUME) return STATE_AIM;
    if (action == PAUSE_ACTION_CONTROLS) { controls_origin = STATE_PAUSE; return STATE_CONTROLS; }
    if (action == PAUSE_ACTION_EXIT_MENU) return STATE_MENU;
    return STATE_PAUSE;
}

static GameState state_game_over(GameContext *context) {
    EventQueue *queue = game_context_get_queue(context);

    GameEvent event;
    while (!event_queue_empty(queue)) {
        event_queue_pop(queue, &event);
    }

    bool t0 = game_context_is_team_alive(context, 0);
    bool t1 = game_context_is_team_alive(context, 1);
    int winner = (t0 && !t1) ? 0 : (t1 && !t0) ? 1 : -1;

    bool confirm = input_is_key_pressed(KEY_ENTER) || input_is_key_pressed(KEY_ESC);
    GameOverAction action = game_over_update(confirm);

    renderer_clear();
    game_over_draw(winner);
    renderer_flip();

    if (action == GAME_OVER_ACTION_QUIT) return STATE_MENU;
    return STATE_GAME_OVER;
}
