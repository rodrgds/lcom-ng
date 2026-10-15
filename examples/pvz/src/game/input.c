#include "pvz_platform.h"

#include "input.h"
#include "board.h"
#include "constants.h"
#include "keyboard.h"
#include "lawnmower.h"
#include "mouse.h"
#include "scene.h"
#include "seed_bank.h"
#include "state.h"
#include "wave.h"
#include "zombie.h"
#include "plant_select.h"

typedef enum {
    INPUT_MODE_MOUSE,
    INPUT_MODE_KEYBOARD
} InputMode;

static void reset_game(void);
static void activate_keyboard_plant(PlantType type);
static void activate_keyboard_shovel(void);
static void activate_keyboard_input(void);
static void activate_mouse_input(void);
static void cancel_active_action(void);
static void confirm_keyboard_cell(void);
static void move_keyboard_cell(int row_delta, int col_delta);
static void set_keyboard_cell(int row, int col);
static bool has_active_placement_action(void);
static void collect_sun_at_selected_cell(void);

static int mouse_x = 400;
static int mouse_y = 300;
static bool was_lb_pressed = false;
static bool was_rb_pressed = false;
static bool shovel_mode = false;
static bool waiting_extended_scancode = false;
static int keyboard_selected_row = KEYBOARD_START_ROW;
static int keyboard_selected_col = KEYBOARD_START_COL;
static InputMode input_mode = INPUT_MODE_MOUSE;
static MouseState mouse_state = MOUSE_STATE_IDLE;
static PlantType selected_seed = PEASHOOTER;
static PlantType selected_plant_type = PEASHOOTER;
static bool dragging = false;
static PlantType dragged_plant;
static bool ignore_menu_enter_release = true;

bool is_dragging_plant(void)           { return dragging; }
PlantType get_dragged_plant_type(void) { return dragged_plant; }

static int seed_slot_from_key(uint8_t scancode) {
    switch (scancode) {
        case KEY_1_BREAKCODE: return 0;
        case KEY_2_BREAKCODE: return 1;
        case KEY_3_BREAKCODE: return 2;
        case KEY_4_BREAKCODE: return 3;
        case KEY_5_BREAKCODE: return 4;
        case KEY_6_BREAKCODE: return 5;
        default: return -1;
    }
}

static void clamp_keyboard_cell(void) {
    if (keyboard_selected_row < 0) keyboard_selected_row = 0;
    if (keyboard_selected_row >= BOARD_ROWS) keyboard_selected_row = BOARD_ROWS - 1;
    if (keyboard_selected_col < 0) keyboard_selected_col = 0;
    if (keyboard_selected_col >= BOARD_COLS) keyboard_selected_col = BOARD_COLS - 1;
}

void handle_keyboard_interrupt(void) {
    kbc_ih();

    if (!get_kbd_error()) {
        handle_keyboard(get_scancode());
    }
}

void handle_mouse_interrupt(void) {
    static uint8_t packet_bytes[3];
    static int byte_index = 0;
    struct packet pp;
    uint8_t byte;

    mouse_ih();

    if (get_mouse_error()) {
        byte_index = 0;
        return;
    }

    byte = get_mouse_byte();

    if (byte_index == 0 && (byte & BIT(3)) == 0) {
        return;
    }

    packet_bytes[byte_index++] = byte;

    if (byte_index == 3) {
        mouse_build_packet(packet_bytes, &pp);
        byte_index = 0;
        handle_mouse(&pp);
    }
}

static void handle_extended_playing_keyboard(uint8_t scancode) {
    activate_keyboard_input();

    switch (scancode) {
        case ARROW_UP_MAKE:    move_keyboard_cell(-1, 0); break;
        case ARROW_DOWN_MAKE:  move_keyboard_cell(1,  0); break;
        case ARROW_LEFT_MAKE:  move_keyboard_cell(0, -1); break;
        case ARROW_RIGHT_MAKE: move_keyboard_cell(0,  1); break;
        default: break;
    }
}

static void handle_extended_selection_keyboard(uint8_t scancode) {
    switch (scancode) {
        case ARROW_LEFT_MAKE:  move_selection_cursor(-1,  0); break;
        case ARROW_RIGHT_MAKE: move_selection_cursor( 1,  0); break;
        case ARROW_UP_MAKE:    move_selection_cursor( 0, -1); break;
        case ARROW_DOWN_MAKE:  move_selection_cursor( 0,  1); break;
        default: break;
    }
}

static bool handle_playing_keyboard(uint8_t scancode) {
    int seed_slot = seed_slot_from_key(scancode);

    if (seed_slot >= 0) {
        activate_keyboard_plant(get_seed_slot_type(seed_slot));
        return true;
    }

    if (scancode == S_BREAKCODE) {
        if (is_keyboard_grid_active() && shovel_mode) {
            cancel_active_action();
        } else {
            activate_keyboard_shovel();
        }
        return true;
    }

    if (scancode == SPACE_BREAKCODE || scancode == ENTER_BREAKCODE) {
        confirm_keyboard_cell();
        return true;
    }

    return false;
}

void handle_keyboard(uint8_t scancode) {
    GameState state = get_game_state();

    if (scancode == EXTENDED_SCANCODE) {
        waiting_extended_scancode = true;
        return;
    }

    if (waiting_extended_scancode) {
        waiting_extended_scancode = false;
        if (state == STATE_PLAYING) {
            handle_extended_playing_keyboard(scancode);
        } else if (state == STATE_PLANT_SELECTION) {
            handle_extended_selection_keyboard(scancode);
        }
        return;
    }


    switch (state) {
        case STATE_MAIN_MENU:
            if (scancode == ENTER_BREAKCODE) {
                if (ignore_menu_enter_release) {
                    ignore_menu_enter_release = false;
                    break;
                }
                init_plant_selection();
                set_game_state(STATE_PLANT_SELECTION);
            }

            if (scancode == ESC_BREAKCODE) {
                set_game_state(STATE_QUIT);
            }
            break;

        case STATE_PLANT_SELECTION:
            if (scancode == ESC_BREAKCODE) {
                ignore_menu_enter_release = true;
                set_game_state(STATE_MAIN_MENU);
            } else if (scancode == SPACE_BREAKCODE) {
                if (get_selected_deck_size() == 6) {
                    confirm_plant_selection();
                    reset_game();
                    set_game_state(STATE_PLAYING);
                }
            } else if (scancode == ENTER_BREAKCODE) {
                if (is_cursor_on_button() && get_selected_deck_size() == 6) {
                    confirm_plant_selection();
                    reset_game();
                    set_game_state(STATE_PLAYING);
                } else if (!is_cursor_on_button()) {
                    int cursor = get_selection_cursor();
                    if (cursor < TOTAL_AVAILABLE_PLANTS) {
                        PlantType cursor_plant = get_available_plants()[cursor];
                        toggle_plant_in_deck(cursor_plant);
                    } else {
                        trigger_deck_full_flash();
                    }
                }
            } else {
                int slot = seed_slot_from_key(scancode);
                if (slot >= 0) {
                    int cursor = get_selection_cursor();
                    if (cursor < TOTAL_AVAILABLE_PLANTS) {
                        PlantType cursor_plant = get_available_plants()[cursor];
                        if (get_deck_slot_for_type(cursor_plant) == slot) {
                            remove_plant_from_deck_slot(slot);
                        } else {
                            assign_plant_to_slot(cursor_plant, slot);
                        }
                    } else {
                        trigger_deck_full_flash();
                    }
                } else if (scancode == KEY_7_BREAKCODE ||
                           scancode == KEY_8_BREAKCODE ||
                           scancode == KEY_9_BREAKCODE ||
                           scancode == KEY_0_BREAKCODE) {
                    /* Slot inválido — flash vermelho */
                    trigger_deck_full_flash();
                }
            }
            break;

        case STATE_PLAYING:
            if (scancode == ESC_BREAKCODE) {
                if (has_active_placement_action()) {
                    cancel_active_action();
                    break;
                }
                set_game_state(STATE_PAUSED);
                break;
            }
            if (handle_playing_keyboard(scancode)) {
                break;
            }
            break;

        case STATE_PAUSED:
            if (scancode == ENTER_BREAKCODE) {
                set_game_state(STATE_PLAYING);
            }

            if (scancode == ESC_BREAKCODE) {
                set_game_state(STATE_QUIT);
            }

            if (scancode == Q_BREAKCODE) {
                reset_game();
                ignore_menu_enter_release = true;
                set_game_state(STATE_MAIN_MENU);
            }
            break;

        case STATE_GAME_OVER:
        case STATE_VICTORY:
            if (scancode == ENTER_BREAKCODE || scancode == ESC_BREAKCODE) {
                reset_game();
                ignore_menu_enter_release = true;
                set_game_state(STATE_MAIN_MENU);
            }
            break;

        default:
            break;
    }
}

bool is_shovel_mode_active(void) {
    return shovel_mode;
}

bool is_keyboard_grid_active(void) {
    return input_mode == INPUT_MODE_KEYBOARD;
}

bool is_keyboard_input_mode_active(void) {
    return input_mode == INPUT_MODE_KEYBOARD;
}

int get_keyboard_selected_row(void) {
    return keyboard_selected_row;
}

int get_keyboard_selected_col(void) {
    return keyboard_selected_col;
}

PlantType get_selected_seed_type(void) {
    return selected_seed;
}

MouseState get_mouse_state(void) {
    return mouse_state;
}

PlantType get_selected_plant_type(void) {
    return selected_plant_type;
}

int get_mouse_x(void) { return mouse_x; }
int get_mouse_y(void) { return mouse_y; }

static bool is_inside_rect(int x, int y, int rx, int ry, int rw, int rh) {
    return x >= rx && x < rx + rw &&
           y >= ry && y < ry + rh;
}

static bool is_inside_shovel(int x, int y) {
    return is_inside_rect(x, y, SHOVEL_X, SHOVEL_Y, SHOVEL_W, SHOVEL_H);
}

static bool can_afford_plant(PlantType type) {
    return get_game_info()->sun_count >= get_plant_cost(type);
}

static void activate_keyboard_input(void) {
    input_mode = INPUT_MODE_KEYBOARD;
}

static void activate_mouse_input(void) {
    input_mode = INPUT_MODE_MOUSE;
}

static bool has_active_placement_action(void) {
    return mouse_state == MOUSE_STATE_HOLDING_PLANT || shovel_mode;
}

static void activate_keyboard_plant(PlantType type) {
    activate_keyboard_input();
    selected_seed = type;

    if (!can_afford_plant(type)) {
        cancel_active_action();
        return;
    }

    selected_plant_type = type;
    mouse_state = MOUSE_STATE_HOLDING_PLANT;
    shovel_mode = false;
}

static void activate_keyboard_shovel(void) {
    activate_keyboard_input();
    mouse_state = MOUSE_STATE_IDLE;
    shovel_mode = true;
}

static void move_keyboard_cell(int row_delta, int col_delta) {
    set_keyboard_cell(keyboard_selected_row + row_delta,
                      keyboard_selected_col + col_delta);
    collect_sun_at_selected_cell();
}

static void set_keyboard_cell(int row, int col) {
    keyboard_selected_row = row;
    keyboard_selected_col = col;
    clamp_keyboard_cell();
}

static void confirm_keyboard_cell(void) {
    if (!is_keyboard_input_mode_active()) return;

    if (shovel_mode) {
        remove_plant(keyboard_selected_row, keyboard_selected_col);
        shovel_mode = false;
        return;
    }

    if (mouse_state == MOUSE_STATE_HOLDING_PLANT &&
        place_plant(keyboard_selected_row, keyboard_selected_col, selected_plant_type)) {
        mouse_state = MOUSE_STATE_IDLE;
    }
}

static void collect_sun_at_selected_cell(void) {
    check_sun_collect_at_grid(keyboard_selected_row, keyboard_selected_col);
}

static bool is_inside_start_button(int x, int y) {
    return is_inside_rect(x, y, MENU_START_X, MENU_START_Y,
                          MENU_START_W, MENU_START_H);
}

static bool is_inside_exit_button(int x, int y) {
    return is_inside_rect(x, y, MENU_EXIT_X, MENU_EXIT_Y,
                          MENU_EXIT_W, MENU_EXIT_H);
}

static void hold_seed(PlantType type) {
    activate_mouse_input();
    selected_seed = type;
    selected_plant_type = type;
    mouse_state = MOUSE_STATE_HOLDING_PLANT;
    shovel_mode = false;
}

static void cancel_active_action(void) {
    mouse_state = MOUSE_STATE_IDLE;
    shovel_mode = false;
}

static void handle_playing_click(void) {
    PlantType clicked_seed;
    int row, col;

    activate_mouse_input();

    if (check_sun_click(mouse_x, mouse_y)) {
        return;
    }

    if (is_inside_shovel(mouse_x, mouse_y)) {
        shovel_mode = !shovel_mode;
        mouse_state = MOUSE_STATE_IDLE;
        return;
    }

    if (get_seed_from_position(mouse_x, mouse_y, &clicked_seed)) {
        if (mouse_state == MOUSE_STATE_HOLDING_PLANT && clicked_seed == selected_plant_type) {
            cancel_active_action();
            return;
        }
        selected_seed = clicked_seed;
        if (can_afford_plant(clicked_seed)) {
            hold_seed(clicked_seed);
        }
        return;
    }

    if (get_grid_from_mouse(mouse_x, mouse_y, &row, &col)) {
        set_keyboard_cell(row, col);

        if (shovel_mode) {
            remove_plant(row, col);
            shovel_mode = false;
        } else if (mouse_state == MOUSE_STATE_HOLDING_PLANT &&
                   place_plant(row, col, selected_plant_type)) {
            mouse_state = MOUSE_STATE_IDLE;
        }
    }
}

static void handle_playing_release(void) {
    int row, col;

    if (mouse_state != MOUSE_STATE_HOLDING_PLANT) {
        return;
    }

    if (get_grid_from_mouse(mouse_x, mouse_y, &row, &col) &&
        place_plant(row, col, selected_plant_type)) {
        set_keyboard_cell(row, col);
        mouse_state = MOUSE_STATE_IDLE;
    }
}

void handle_mouse(struct packet *p) {
    int dx = p->delta_x;
    int dy = p->delta_y;
    bool pointer_moved;

    if (p->x_ov || p->y_ov) {
        dx = 0;
        dy = 0;
    }

    mouse_x += dx;
    mouse_y -= dy;

    if (mouse_x < 0) mouse_x = 0;
    if (mouse_x >= SCREEN_W) mouse_x = SCREEN_W - 1;
    if (mouse_y < 0) mouse_y = 0;
    if (mouse_y >= SCREEN_H) mouse_y = SCREEN_H - 1;

    pointer_moved = (dx != 0 || dy != 0);
    if (pointer_moved) {
        activate_mouse_input();
    }

    bool is_lb_clicked = (p->lb && !was_lb_pressed);
    bool is_lb_released = (!p->lb && was_lb_pressed);
    bool is_rb_clicked = (p->rb && !was_rb_pressed);

    was_lb_pressed = p->lb;
    was_rb_pressed = p->rb;

    switch (get_game_state()) {
        case STATE_MAIN_MENU:
            if (is_lb_clicked) {
                if (is_inside_start_button(mouse_x, mouse_y)) {
                    init_plant_selection();
                    set_game_state(STATE_PLANT_SELECTION);
                } else if (is_inside_exit_button(mouse_x, mouse_y)) {
                    set_game_state(STATE_QUIT);
                }
            }
            break;

        case STATE_PLANT_SELECTION:
            if (is_lb_clicked) {
                PlantType inv_plant;
                int slot;
                if (get_plant_from_inventory_click(mouse_x, mouse_y, &inv_plant)) {
                    dragging      = true;
                    dragged_plant = inv_plant;
                } else if (get_deck_slot_from_position(mouse_x, mouse_y, &slot)) {
                    remove_plant_from_deck_slot(slot);
                    dragging = false;
                } else if (is_start_button_clicked(mouse_x, mouse_y) &&
                           get_selected_deck_size() == 6) {
                    dragging = false;
                    confirm_plant_selection();
                    reset_game();
                    set_game_state(STATE_PLAYING);
                }
            }
            if (is_lb_released && dragging) {
                int slot;
                if (get_deck_slot_from_position(mouse_x, mouse_y, &slot)) {
                    assign_plant_to_slot(dragged_plant, slot);
                }
                dragging = false;
            }
            break;

        case STATE_PLAYING:
            if (is_rb_clicked) {
                activate_mouse_input();
                cancel_active_action();
            }

            if (is_lb_clicked) {
                handle_playing_click();
            }

            if (is_lb_released) {
                handle_playing_release();
            }
            break;

        default:
            break;
    }
}

static void reset_game(void) {
    init_board();
    init_game_info();
    init_wave_system();
    init_lawnmowers();

    shovel_mode = false;
    dragging = false;
    input_mode = INPUT_MODE_MOUSE;
    waiting_extended_scancode = false;
    keyboard_selected_row = KEYBOARD_START_ROW;
    keyboard_selected_col = KEYBOARD_START_COL;
    mouse_state = MOUSE_STATE_IDLE;
    selected_seed = PEASHOOTER;
    selected_plant_type = PEASHOOTER;
    was_lb_pressed = false;
    was_rb_pressed = false;

    clear_zombies();
}
