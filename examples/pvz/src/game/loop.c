#include "pvz_platform.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "loop.h"


static void draw_current_frame(void);
static void init_game(void);
static void handle_timer_interrupt(void);
static bool session_played = false;

static bool should_draw_mouse_cursor(GameState state) {
    return state != STATE_PLAYING || !is_keyboard_input_mode_active();
}

static void draw_current_frame(void) {
    GameState state = get_game_state();
    Zombie *zombies = get_zombies();

    if (state == STATE_PLAYING) {
        draw_world(zombies, MAX_ZOMBIES);
    }
    else if (state == STATE_PAUSED) {
        draw_paused_world(zombies, MAX_ZOMBIES);
    }
    else if (state == STATE_PLANT_SELECTION) {
        render_plant_selection();
    }
    else {
        render_scene();
    }

    if (should_draw_mouse_cursor(state)) {
        draw_cursor(get_mouse_x(), get_mouse_y());
    }
    vg_flush();
}

static void init_game(void) {
    init_board();
    clear_zombies();
    session_played = false;

    srand(get_timer_counter());
    init_wave_system();
    init_lawnmowers();

    init_game_info();
    set_game_state(STATE_MAIN_MENU);
    rtc_update_cached_datetime();
    rtc_load_last_played();

    draw_current_frame();
}

static void handle_timer_interrupt(void) {
    timer_int_handler();

    if (get_game_state() != STATE_PLAYING &&
        get_timer_counter() % (int) FPS == 0) {
        rtc_update_cached_datetime();
    }

    if (get_game_state() == STATE_PLAYING) {
        update_world(get_zombies(), MAX_ZOMBIES);
    }
    else if (get_game_state() == STATE_PLANT_SELECTION) {
        tick_deck_full_flash();
    }

    if (get_timer_counter() % 1 == 0) {
        draw_current_frame();
    }
}

int game_loop(void) {
    uint8_t timer_bit_no, kbd_bit_no, mouse_bit_no;
    uint32_t timer_irq_set, kbd_irq_set, mouse_irq_set;
    if (init_systems(&timer_bit_no, &kbd_bit_no, &mouse_bit_no) != 0) {
        return 1;
    }

    timer_irq_set = BIT(timer_bit_no);
    kbd_irq_set = BIT(kbd_bit_no);
    mouse_irq_set = BIT(mouse_bit_no);

    if (load_all_sprites() != 0) {
        printf("Failed to load sprites!\n");
        free_all_sprites();
        cleanup_systems();
        return 1;
    }

    init_game();

    while (get_game_state() != STATE_QUIT) {
        lcom_event_t event;
        if (lcom_event_wait(&event) != LCOM_OK) {
            continue;
        }

        if (event.irq_mask & mouse_irq_set) {
            handle_mouse_interrupt();
        }

        if (event.irq_mask & kbd_irq_set) {
            handle_keyboard_interrupt();
        }

        if (event.irq_mask & timer_irq_set) {
            handle_timer_interrupt();
        }

        if (get_game_state() == STATE_PLAYING) {
            session_played = true;
        }
    }

    if (session_played) {
        rtc_save_last_played();
    }
    free_all_sprites();
    cleanup_systems();
    return 0;
}
