/**
 * @file controller.c
 * @brief Main controller for the Minesweeper game on Minix
 * 
 * This file implements the main controller that manages game loops,
 * mouse and timer interrupts, and control logic between different
 * game states (menu, game, game over, victory).
 */

#include "controller.h"
#include "../model/game.h"

/// Previous cursor position on X axis (-1 indicates invalid position)
static int prev_x = -1, prev_y = -1;

/// Initial mouse cursor position on screen
static int mouse_x = 400, mouse_y = 300;

/// Flag to temporarily ignore mouse events
static bool ignore_mouse_events = false;

/// Flag indicating if it's the first click of the game (for initial reveal)
static bool first_click = true;

/// External variables for mouse and timer IRQs
extern uint8_t mouse_irq, timer_irq;

/// External variables for mouse and timer IRQ sets
extern int irq_set_mouse, irq_set_timer;

int init_devices(uint8_t *mouse_irq, uint8_t *timer_irq, int *irq_set_mouse, int *irq_set_timer) {
    if (set_frame_buffer(VBE_600p_DC) != 0) return 1;
    if (set_graphic_mode(VBE_600p_DC) != 0) return 1;
    allocateImgBuffer();

    if (mouse_subscribe_int(mouse_irq) != 0) return 1;
    *irq_set_mouse = BIT(*mouse_irq);

    if (mouse_send_command(MOUSE_DATA_REPORT_ON) != 0) return 1;

    if (timer_subscribe_int(timer_irq) != 0) return 1;

    *irq_set_timer = BIT(*timer_irq);

    timer_set_frequency(0, 60);
    init_allsprites();

    return 0;
}

void cleanup_devices() {
    mouse_send_command(MOUSE_DATA_REPORT_OFF);
    mouse_unsubscribe_int();
    timer_unsubscribe_int();
    freeImgBuffer();
    destroy_allsprites();
    set_text_mode();
}

int menu_loop() {
    int ipc_status, r;
    message msg;
    bool running = true;

    if (!menu) {
        printf("Error creating menu\n");
        cleanup_devices();
        return 1;
    }

    // Initial menu draw
    clearImgBuffer();
    draw_menu(menu);
    draw_cursor(mouse_x, mouse_y);
    copyBufferToVideo();

    ignore_mouse_events = true;

    while (running) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set_mouse) {

                        int result = handle_mouse_interrupt_menu(&mouse_x, &mouse_y, &running);

                        if (result == 1) { // Start game
                            return 0; // Signals main to start the game
                        }
                    }

                    if (msg.m_notify.interrupts & irq_set_timer) {
                        handle_timer_interrupt_menu(mouse_x, mouse_y);
                    }
                    break;

                default:
                    break;
            }
        }
    }

    return -1; // Quit game
}

int game_loop(GameDifficulty difficulty) {
    int ipc_status, r;
    message msg;
    bool running = true;
    int win = -1; // -1 = running, 0 = lose, 1 = win, -2 = return to menu

    init_game(difficulty);

    ignore_mouse_events = true;
    first_click = true;
    
    while (running) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set_mouse) {
                        handle_mouse_interrupt_game(&mouse_x, &mouse_y, &running, &win);
                    }
                    if (msg.m_notify.interrupts & irq_set_timer) {
                        handle_timer_interrupt_game(mouse_x, mouse_y);
                    }
                    break;
                default:
                    break;
            }
        }
        if (win != -1) running = false;
    }
    if (win == -2) return -2;

    return win;
}

int game_over_loop() {
    int ipc_status, r;
    message msg;
    bool running = true;

    ignore_mouse_events = true;

    while (running) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set_mouse) {
                        handle_mouse_interrupt_game_over_win(&mouse_x, &mouse_y, &running);
                    }

                    if (msg.m_notify.interrupts & irq_set_timer) {
                        handle_timer_interrupt_game_over(mouse_x, mouse_y);
                    }
                    break;

                default:
                    break;
            }
        }
    }

    return 0; // Return to menu
}

int game_win_loop() {
    int ipc_status, r;
    message msg;
    bool running = true;

    on_game_end();
    int gw_x = (SCREEN_WIDTH - GAME_WIN_WIDTH) / 2;
    int gw_y = (SCREEN_HEIGHT - GAME_WIN_HEIGHT) / 2;
    int timer_x = gw_x + (GAME_WIN_WIDTH - 5 * CELL_SIZE) / 2;
    int timer_y = gw_y + GAME_WIN_HEIGHT + 10;
    draw_game_win_screen(get_elapsed_seconds(), gw_x, gw_y, timer_x, timer_y, mouse_x, mouse_y);
    copyBufferToVideo();

    ignore_mouse_events = true;

    while (running) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set_mouse) {
                        handle_mouse_interrupt_game_over_win(&mouse_x, &mouse_y, &running);
                    }
                    if (msg.m_notify.interrupts & irq_set_timer) {
                        draw_game_win_screen(get_elapsed_seconds(), gw_x, gw_y, timer_x, timer_y, mouse_x, mouse_y);
                        copyBufferToVideo();
                    }
                    break;
                default:
                    break;
            }
        }
    }
    return 0; // Return to menu
}

/**
 * @brief Limits mouse position within screen boundaries
 * 
 * Ensures the mouse cursor coordinates stay within the valid
 * screen area by clamping values to screen dimensions.
 * 
 * @param x Pointer to X coordinate to limit
 * @param y Pointer to Y coordinate to limit
 */
static void limit_mouse_position(int *x, int *y) {
    if (*x < 0) *x = 0;
    if (*x > SCREEN_WIDTH-1) *x = SCREEN_WIDTH-1;
    if (*y < 0) *y = 0;
    if (*y > SCREEN_HEIGHT-1) *y = SCREEN_HEIGHT-1;
}

int handle_mouse_interrupt_menu(int *x, int *y, bool *running) {
    mouse_ih();
    mouse_sync_packet();

    if (byte_cnt == 3) {
        mouse_parse_packet();

        if (ignore_mouse_events) {
            if (!mouse_packet.lb && !mouse_packet.rb && !mouse_packet.mb) {
                ignore_mouse_events = false; // Only start accepting clicks after all buttons are released
            }
            byte_cnt = 0;
            return 0;
        }

        int new_x = *x + mouse_packet.delta_x;
        int new_y = *y - mouse_packet.delta_y;
        limit_mouse_position(&new_x, &new_y);
        if (new_x != *x || new_y != *y) {
            *x = new_x;
            *y = new_y;
        }

        reset_menu_buttons(menu);
        handle_mouse_hover(menu, *x, *y);
        
        if (mouse_packet.lb) {
            if (handle_mouse_left_click_menu(menu, *x, *y, running) == 1) {
                reset_menu_buttons(menu);
                return 1; // Start game
            }
            
        }

        byte_cnt = 0;
    }
    return 0;
}

void handle_timer_interrupt_menu(int x, int y) {
    clearImgBuffer();

    draw_menu(menu);
    draw_cursor(x, y);
    

    copyBufferToVideo();

    prev_x = x;
    prev_y = y;
}

int handle_mouse_interrupt_game(int *x, int *y, bool *running, int *win) {
    mouse_ih();
    mouse_sync_packet();

    if (byte_cnt == 3) {
        mouse_parse_packet();

        if (ignore_mouse_events) {
            if (!mouse_packet.lb && !mouse_packet.rb && !mouse_packet.mb) {
                ignore_mouse_events = false; // Only start accepting clicks after all buttons are released
            }
            byte_cnt = 0;
            return 0;
        }

        int new_x = *x + mouse_packet.delta_x;
        int new_y = *y - mouse_packet.delta_y;
        limit_mouse_position(&new_x, &new_y);
        if (new_x != *x || new_y != *y) {
            *x = new_x;
            *y = new_y;
        }
        int leave_x = SCREEN_WIDTH - 50;
        int leave_y = 0;
        if (mouse_packet.lb && *x >= leave_x && *x < leave_x + 50 && *y >= leave_y && *y < leave_y + 50) {
            *win = -2; 
            *running = false;
            byte_cnt = 0;
            return 0;
        }
        if (mouse_packet.lb) {
            handle_mouse_left_click_game(map, *x, *y, win);
        }
        if (mouse_packet.rb) {
            handle_mouse_right_click_game(map, *x, *y);
        }
        if (*win == 0 || *win == 1) {
            *running = false;
        }
        byte_cnt = 0;
    }
    return 0;
}

void handle_timer_interrupt_game(int x, int y) {
    static int frame_counter = 0;
    frame_counter++;
    if (frame_counter >= 30) { 
        rtc_update_time();
        int now_seconds = time_info.hours * 3600 + time_info.minutes * 60 + time_info.seconds;
        int elapsed = now_seconds - game_start_seconds;
        if (elapsed < 0) elapsed += 24 * 3600;
        set_elapsed_seconds(elapsed);
        frame_counter = 0;
    }
    clearImgBuffer();
    draw_game();
    draw_cursor(x, y);
    copyBufferToVideo();
    prev_x = x;
    prev_y = y;
}

int handle_mouse_interrupt_game_over_win(int *x, int *y, bool *running) {
    mouse_ih();
    mouse_sync_packet();

    if (byte_cnt == 3) {
        mouse_parse_packet();

        if (ignore_mouse_events) {
            if (!mouse_packet.lb && !mouse_packet.rb && !mouse_packet.mb) {
                ignore_mouse_events = false; // Only start accepting clicks after all buttons are released
            }
            byte_cnt = 0;
            return 0;
        }

        int new_x = *x + mouse_packet.delta_x;
        int new_y = *y - mouse_packet.delta_y;
        limit_mouse_position(&new_x, &new_y);
        if (new_x != *x || new_y != *y) {
            *x = new_x;
            *y = new_y;
        }

        if (mouse_packet.lb || mouse_packet.mb || mouse_packet.rb) {
            *running = false; // any button click goes back to menu
        }

        byte_cnt = 0;
    }
    return 0;
}

void handle_timer_interrupt_game_over(int x, int y) {
    int go_x = (SCREEN_WIDTH - GAME_OVER_WIDTH) / 2;
    int go_y = (SCREEN_HEIGHT - GAME_OVER_HEIGHT) / 2;

    clearImgBuffer();

    draw_game();
    draw_game_over(go_x, go_y);
    draw_cursor(x, y);

    copyBufferToVideo();

    prev_x = x;
    prev_y = y;
}

void handle_timer_interrupt_game_win(int x, int y) {
    int gw_x = (SCREEN_WIDTH - GAME_WIN_WIDTH) / 2;
    int gw_y = (SCREEN_HEIGHT - GAME_WIN_HEIGHT) / 2;
    int timer_x = gw_x + (GAME_WIN_WIDTH - 5 * CELL_SIZE) / 2;
    int timer_y = gw_y + GAME_WIN_HEIGHT + 10;
    draw_game_win_screen(get_elapsed_seconds(), gw_x, gw_y, timer_x, timer_y, x, y);
}

int handle_mouse_left_click_menu(Menu* menu, int mouse_x, int mouse_y, bool *running) {
    // Check if click is inside start button
    if (mouse_x >= menu->start_button.x && mouse_x < menu->start_button.x + menu->start_button.width &&
        mouse_y >= menu->start_button.y && mouse_y < menu->start_button.y + menu->start_button.height) {
        *running = false; // Start game
        return 1;
    }

    // Check if click is on left mode arrow
    if (mouse_x >= menu->left_mode_arrow.x && mouse_x < menu->left_mode_arrow.x + menu->left_mode_arrow.width &&
        mouse_y >= menu->left_mode_arrow.y && mouse_y < menu->left_mode_arrow.y + menu->left_mode_arrow.height) {
        if (menu->selected_mode == DECEIVER) {
            menu->selected_mode = NORMAL;
        } else {
            menu->selected_mode = DECEIVER;
        }

        draw_mode_button(menu);
    }

    // Check if click is on right mode arrow
    if (mouse_x >= menu->right_mode_arrow.x && mouse_x < menu->right_mode_arrow.x + menu->right_mode_arrow.width &&
        mouse_y >= menu->right_mode_arrow.y && mouse_y < menu->right_mode_arrow.y + menu->right_mode_arrow.height) {
        if (menu->selected_mode == DECEIVER) {
            menu->selected_mode = NORMAL;
        } else {
            menu->selected_mode = DECEIVER;
        }

        draw_mode_button(menu);
    }

    // Check if click is on left difficulty arrow
    if (mouse_x >= menu->left_difficulty_arrow.x && mouse_x < menu->left_difficulty_arrow.x + menu->left_difficulty_arrow.width &&
        mouse_y >= menu->left_difficulty_arrow.y && mouse_y < menu->left_difficulty_arrow.y + menu->left_difficulty_arrow.height) {
        if (menu->selected_difficulty == HARD) {
            menu->selected_difficulty = MEDIUM;
        } else if (menu->selected_difficulty == MEDIUM) {
            menu->selected_difficulty = EASY;
        }
        else {
            menu->selected_difficulty = HARD;
        }

        draw_difficulty_button(menu);
    }

    // Check if click is on right difficulty arrow
    if (mouse_x >= menu->right_difficulty_arrow.x && mouse_x < menu->right_difficulty_arrow.x + menu->right_difficulty_arrow.width &&
        mouse_y >= menu->right_difficulty_arrow.y && mouse_y < menu->right_difficulty_arrow.y + menu->right_difficulty_arrow.height) {
        if (menu->selected_difficulty == HARD) {
            menu->selected_difficulty = EASY;
        } else if (menu->selected_difficulty == MEDIUM) {
            menu->selected_difficulty = HARD;
        }
        else {
            menu->selected_difficulty = MEDIUM;
        }

        draw_difficulty_button(menu);
    }

    // Check if click is inside exit button
    if (mouse_x >= menu->exit_button.x && mouse_x < menu->exit_button.x + menu->exit_button.width &&
        mouse_y >= menu->exit_button.y && mouse_y < menu->exit_button.y + menu->exit_button.height) {
        *running = false; // Exit game
    }

    return 0; // Don't start game
}

void handle_mouse_hover(Menu* menu, int mouse_x, int mouse_y) {
    // Check if hovering over start button
    if (mouse_x >= menu->start_button.x && mouse_x < menu->start_button.x + menu->start_button.width &&
        mouse_y >= menu->start_button.y && mouse_y < menu->start_button.y + menu->start_button.height) {
        menu->start_button.hovered = true;

        draw_start_button(menu);
    }

    // Check if hovering over left mode arrow
    if (mouse_x >= menu->left_mode_arrow.x && mouse_x < menu->left_mode_arrow.x + menu->left_mode_arrow.width &&
        mouse_y >= menu->left_mode_arrow.y && mouse_y < menu->left_mode_arrow.y + menu->left_mode_arrow.height) {
        menu->left_mode_arrow.hovered = true;

        draw_mode_button(menu);
    }

    // Check if hovering over right mode arrow
    if (mouse_x >= menu->right_mode_arrow.x && mouse_x < menu->right_mode_arrow.x + menu->right_mode_arrow.width &&
        mouse_y >= menu->right_mode_arrow.y && mouse_y < menu->right_mode_arrow.y + menu->right_mode_arrow.height) {
        menu->right_mode_arrow.hovered = true;

        draw_mode_button(menu);
    }

    // Check if hovering over left difficulty arrow
    if (mouse_x >= menu->left_difficulty_arrow.x && mouse_x < menu->left_difficulty_arrow.x + menu->left_difficulty_arrow.width &&
        mouse_y >= menu->left_difficulty_arrow.y && mouse_y < menu->left_difficulty_arrow.y + menu->left_difficulty_arrow.height) {
        menu->left_difficulty_arrow.hovered = true;

        draw_difficulty_button(menu);
    }

    // Check if hovering over right difficulty arrow
    if (mouse_x >= menu->right_difficulty_arrow.x && mouse_x < menu->right_difficulty_arrow.x + menu->right_difficulty_arrow.width &&
        mouse_y >= menu->right_difficulty_arrow.y && mouse_y < menu->right_difficulty_arrow.y + menu->right_difficulty_arrow.height) {
        menu->right_difficulty_arrow.hovered = true;

        draw_difficulty_button(menu);
    }

    // Check if hovering over exit button
    if (mouse_x >= menu->exit_button.x && mouse_x < menu->exit_button.x + menu->exit_button.width &&
        mouse_y >= menu->exit_button.y && mouse_y < menu->exit_button.y + menu->exit_button.height) {
        menu->exit_button.hovered = true;
        
        draw_exit_button(menu);
    }


}

void handle_mouse_left_click_game(Map* map, int mouse_x, int mouse_y, int *win) {
    int lamp_x_start = map->start_x;
    int lamp_y_start = map->start_y - CELL_SIZE - BORDER_SPACING;
    int lamp_x_end = map->start_x + CELL_SIZE;
    int lamp_y_end = map->start_y;

    if (!map->lamp_used && mouse_x >= lamp_x_start && mouse_x < lamp_x_end &&
        mouse_y >= lamp_y_start && mouse_y < lamp_y_end) {
        reveal_random_cell(map);
        map->lamp_used = true;
        if (map->revealed_cells == map->rows * map->cols - map->total_bombs) {
            *win = 1;
            return;
        }
        return;
    }
    if (mouse_x < (int)map->start_x || mouse_x >= (int)(map->start_x + map->cols * CELL_SIZE) ||
        mouse_y < (int)map->start_y || mouse_y >= (int)(map->start_y + map->rows * CELL_SIZE)) {
        return;
    }
    unsigned int cell_x = (mouse_x - map->start_x) / CELL_SIZE;
    unsigned int cell_y = (mouse_y - map->start_y) / CELL_SIZE;
    if (!map->cells[cell_y][cell_x].is_flagged) {
        if (first_click) {
            map_first_reveal(map, cell_y, cell_x);
            first_click = false;
            if (menu->selected_mode == DECEIVER) {
                deceiver_change_cell(map);
            }
        } else {
            map_reveal_cell(map, cell_y, cell_x);
        }
    }
    if (!map->cells[cell_y][cell_x].is_flagged && map->cells[cell_y][cell_x].is_bomb) {
        map->cells[cell_y][cell_x].is_exploded = true;
        map_reveal_all_bombs(map);
        *win = 0; // Game over
        return;
    }
    if (map->revealed_cells == map->rows * map->cols - map->total_bombs) {
        *win = 1; // Game win
        return;
    }
}

void handle_mouse_right_click_game(Map* map, int mouse_x, int mouse_y) {
    // Check if click is inside the map
    if (mouse_x < (int)map->start_x || mouse_x >= (int)(map->start_x + map->cols * CELL_SIZE) ||
        mouse_y < (int)map->start_y || mouse_y >= (int)(map->start_y + map->rows * CELL_SIZE)) {
        return;
    }

    // Convert mouse coordinates to map coordinates
    unsigned int cell_x = (mouse_x - map->start_x) / CELL_SIZE;
    unsigned int cell_y = (mouse_y - map->start_y) / CELL_SIZE;

    if (cell_x < map->cols && cell_y < map->rows) {
        map_flag_cell(map, cell_y, cell_x);
    }

}

