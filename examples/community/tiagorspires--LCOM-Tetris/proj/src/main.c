#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "video.h"
#include "keyboard.h"
#include <lcom/timer.h>
#include "pictures.h"
#include "mouse.h"
#include "game.h"

extern int counter;
extern uint8_t scancode;
extern int max_x, max_y; 
extern struct packet pp;
extern bool update_count;
extern int x, y;

int main(int argc, char *argv[]) {
    lcf_set_language("EN-US");
    if (lcf_start(argc, argv)) return 1;
    lcf_cleanup();
    return 0; 
}

int (proj_main_loop) (int argc, char **argv) {

    GameState state = MAIN_MENU;
    TetrisPiece pieces[100]; 
    char screen[24][32];
    int colorScreen[24][32];

    if(set_frame_buffer(0x105)) return 1;
    if(set_video_mode(0x105)) return 1;
    if(timer_set_frequency(0, 30)) return 1;

    int ipc_status, r;
    uint8_t irq_set_keyboard;
    uint8_t irq_set_timer;
    uint8_t irq_set_mouse;
    
    message msg;

    if(change_data_report_mode(0xF4)) return 1;
    if(change_data_report_mode(0xEA)) return 1;

    if(timer_subscribe_int(&irq_set_timer)) return 1;
    if(keyboard_subscribe(&irq_set_keyboard)) return 1;
    if(mouse_subscribe_int(&irq_set_mouse)) return 1;

    int countpiece = 1;
    while (scancode != ESC_BREAK_CODE && countpiece < 100) { 
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */

                if (msg.m_notify.interrupts & irq_set_timer) {
                    if(state == GAME){
                        timer_int_handler();
                        if (counter % 30 == 0) {
                            if (is_piece_at_bottom(&pieces[countpiece - 1], screen, colorScreen)) {
                                check_and_clear_full_lines(screen, colorScreen, pieces, countpiece);
                                if (game_over(screen)) {
                                    handle_game_over(&state, screen, colorScreen, pieces, &countpiece);
                                } else if (countpiece < 50) {
                                    pieces[countpiece] = generate_random_piece();
                                    pieces[countpiece].isActive = true;
                                    countpiece++;
                                }
                            } else {
                                move_piece(&pieces[countpiece - 1], 0, 1, screen, colorScreen);
                            }
                        }
                    }
                }

                if (msg.m_notify.interrupts & irq_set_keyboard) {
                    kbc_ih_keyboard();
                    TetrisPiece* current_piece = &pieces[countpiece - 1];
                    if(state == GAME){
                        if (!game_over(screen)){
                            switch (scancode) {
                                case A_BREAK_CODE:
                                    move_piece(current_piece, -1, 0, screen, colorScreen);
                                    break;
                                case D_BREAK_CODE:
                                    move_piece(current_piece, 1, 0, screen, colorScreen);
                                    break;
                                case S_BREAK_CODE:
                                    move_piece(current_piece, 0, 1, screen, colorScreen);
                                    break;
                                case W_BREAK_CODE:
                                    rotate_piece(current_piece, screen, colorScreen);
                                    break;
                                case SPACE_BREAK_CODE:
                                    drop_until_bottom(current_piece, screen, colorScreen);
                                    break;
                                default:
                                    break;
                            }
                        } else {
                            state = GAME_OVER;
                        }
                    }
                    if (state == GAME && scancode == Q_BREAK_CODE) {
                        state = GAME_OVER;
                    }
                }
                if (msg.m_notify.interrupts & irq_set_mouse) {
                    mouse_ih();
                    if (!update_count) break;
                    update_count = false;
                    mouse_event_handler(&pp);
                    if (state == MAIN_MENU && pp.lb == 1) {
                        if (is_within_rectangle(x, y, (max_x / 2) - 150, (max_y / 2) - 130, 300, 100)) {
                            TetrisPiece piece = generate_random_piece();
                            pieces[0] = piece;
                            reset_screen(screen, colorScreen);
                            countpiece = 1;
                            state = GAME;
                        } else if (is_within_rectangle(x, y, (max_x / 2) - 150, (max_y / 2) + 30, 300, 100)) {
                            if (timer_unsubscribe_int()) return 1;
                            if (keyboard_unsubscribe()) return 1;
                            if (mouse_unsubscribe_int(&irq_set_mouse)) return 1;
                            if (vg_exit()) return 1;
                            return 0;
                        }
                    } else if (state == GAME_OVER && pp.lb == 1) {
                        if (is_within_rectangle(x, y, (max_x / 2) - 150, (max_y / 2) - 130, 300, 100)) {
                            // Reinicializar o estado do jogo
                            TetrisPiece piece = generate_random_piece();
                            pieces[0] = piece;
                            reset_screen(screen, colorScreen);
                            countpiece = 1;
                            state = GAME;
                        } else if (is_within_rectangle(x, y, (max_x / 2) - 150, (max_y / 2) + 30, 300, 100)) {
                            if (timer_unsubscribe_int()) return 1;
                            if (keyboard_unsubscribe()) return 1;
                            if (mouse_unsubscribe_int(&irq_set_mouse)) return 1;
                            if (vg_exit()) return 1;
                            return 0;
                        }
                    }
                }
                break;
            default:
                break; /* no other notifications expected: do nothing */
            }
            if (state == MAIN_MENU) {
                clean_buffer();
                draw_xpm(x, y, mouse_cursor);
                if (draw_centered_rectangles(max_x, max_y) != 0) {
                    printf("Failed to draw rectangles.\n");
                    return 1;  
                }
                xpm_row_t const *play_word[] = {P, L, A, Y};
                xpm_row_t const *exit_word[] = {E, X, I, T};

                int top_rect_x = (max_x / 2) - 150;
                int top_rect_y = (max_y / 2) - 130;

                int bottom_rect_x = (max_x / 2) - 150;
                int bottom_rect_y = (max_y / 2) + 30;

                draw_word_in_rectangle(top_rect_x, top_rect_y, play_word, 4);
                draw_word_in_rectangle(bottom_rect_x, bottom_rect_y, exit_word, 4);

                swap_buffer();
            } else if (state == GAME_OVER) { 
                clean_buffer();
                draw_xpm(x, y, mouse_cursor);
                if (draw_centered_rectangles(max_x, max_y) != 0) {
                    printf("Failed to draw rectangles.\n");
                    return 1;
                }

                xpm_row_t const *restart_word[] = {R, E, S, T, A, R, T};
                xpm_row_t const *exit_word[] = {E, X, I, T};

                int top_rect_x = (max_x / 2) - 150;
                int top_rect_y = (max_y / 2) - 130;

                int bottom_rect_x = (max_x / 2) - 150;
                int bottom_rect_y = (max_y / 2) + 30;

                draw_word_in_rectangle(top_rect_x, top_rect_y, restart_word, 7);
                draw_word_in_rectangle(bottom_rect_x, bottom_rect_y, exit_word, 4);

                swap_buffer();
            } else {
                draw(screen, colorScreen, pieces, countpiece);
                swap_buffer();
            }
        } else { 
        }
    }

    if (timer_unsubscribe_int()) return 1;
    if (keyboard_unsubscribe()) return 1;
    if (mouse_unsubscribe_int(&irq_set_mouse)) return 1;
    if (change_data_report_mode(0xF5)) return 1;  
    if (escape_key()) return 1;
    if (vg_exit()) return 1;
    return 0;
}
