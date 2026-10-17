#include <lcom/lcf.h>

#include "game.h"
#include "menu_state.h"
#include "sp_protocol.h"
#include "word_picker.h"

#include "../controllers/rtc/rtc.h"
#include "../controllers/rtc/macro_rtc.h"

#include "../graphics/paint_window.h"
#include "../graphics/drawing_state_graphics.h"

#include "../controllers/keyboard_mouse/make_codes.h"
#include "../controllers/keyboard_mouse/macro_kb_mouse.h"

#include "../controllers/serial_port/queue.h"
#include "../controllers/serial_port/serial_port.h"

#include "../cursor.h"
#include "../getters.h"

#include "../img/cursor.xpm"
#include "../img/screens/drawingLight.xpm"
#include "../img/screens/drawingDark.xpm"
#include "../img/screens/drawingLightSingle.xpm"
#include "../img/screens/drawingDarkSingle.xpm"
#include "../img/screens/winLight.xpm"
#include "../img/screens/winDark.xpm"
#include "../img/screens/loseLight.xpm"
#include "../img/screens/loseDark.xpm"
#include "../img/guessBox.xpm"
#include "../img/guessCross.xpm"
#include "../img/timer_numbers/timer_0.xpm"
#include "../img/timer_numbers/timer_1.xpm"
#include "../img/timer_numbers/timer_2.xpm"
#include "../img/timer_numbers/timer_3.xpm"
#include "../img/timer_numbers/timer_4.xpm"
#include "../img/timer_numbers/timer_5.xpm"
#include "../img/timer_numbers/timer_6.xpm"
#include "../img/timer_numbers/timer_7.xpm"
#include "../img/timer_numbers/timer_8.xpm"
#include "../img/timer_numbers/timer_9.xpm"
#include "../img/timer_numbers/blank.xpm"
#include "../img/letters/A.xpm"
#include "../img/letters/B.xpm"
#include "../img/letters/C.xpm"
#include "../img/letters/D.xpm"
#include "../img/letters/E.xpm"
#include "../img/letters/F.xpm"
#include "../img/letters/G.xpm"
#include "../img/letters/H.xpm"
#include "../img/letters/I.xpm"
#include "../img/letters/J.xpm"
#include "../img/letters/K.xpm"
#include "../img/letters/L.xpm"
#include "../img/letters/M.xpm"
#include "../img/letters/N.xpm"
#include "../img/letters/O.xpm"
#include "../img/letters/P.xpm"
#include "../img/letters/Q.xpm"
#include "../img/letters/R.xpm"
#include "../img/letters/S.xpm"
#include "../img/letters/T.xpm"
#include "../img/letters/U.xpm"
#include "../img/letters/V.xpm"
#include "../img/letters/W.xpm"
#include "../img/letters/X.xpm"
#include "../img/letters/Y.xpm"
#include "../img/letters/Z.xpm"
#include "../img/letters/delete_letter.xpm"


extern int timer_counter;

int game_state_handler(interrupt interrupt, Cursor* cursor) {
    static state currentState = MAIN_MENU;
    static state player2State = MULTIPLAYER_MENU;
    static drawing_state drawingState = INIT_DRAWING;

    static char** timer_numbers[11] = {timer_0, timer_1, timer_2, timer_3, timer_4, timer_5, timer_6, timer_7, timer_8, timer_9,blank};
    static int remaining_time = 99;

    static uint8_t make_codes[26] = {A_MAKE, B_MAKE, C_MAKE, D_MAKE, E_MAKE, F_MAKE, G_MAKE, H_MAKE, I_MAKE, J_MAKE, 
                                K_MAKE, L_MAKE, M_MAKE, N_MAKE, O_MAKE, P_MAKE, Q_MAKE, R_MAKE, S_MAKE, T_MAKE, U_MAKE,
                                    V_MAKE, W_MAKE, X_MAKE, Y_MAKE, Z_MAKE};

    static char **letter_xpm[26] = {A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z};
    static int counter = 0;
    static char guess_word[MAX_WORD_LENGTH];
    static char* answer_word;

    static uint8_t serial_data[8];
    static int serial_i = 0;
    static sp_received_data dataState = UNKNOWN_DATA;

    uint8_t sp_data = 0;
    bool sp_state_data = true;

    if (queue_pop(QUEUE_RECEIVE, &sp_data) == 0) {

        if (sp_data == READY_DRAW && player2State == MULTIPLAYER_MENU) player2State = WAITING_DRAWING;
        else if (sp_data == READY_GUESS && player2State == MULTIPLAYER_MENU) player2State = WAITING_GUESSING;
        else if (sp_data == UNREADY && currentState != GUESSING) {
            queue_pop(QUEUE_RECEIVE, &sp_data);
            player2State = MULTIPLAYER_MENU;
        }
        else sp_state_data = false;
    }
    
    switch (currentState) {
        case MAIN_MENU:
            if (interrupt == MOUSE) {
                move_cursor(cursor, get_packet().delta_x, get_packet().delta_y);
                if(process_singleplayer_button(cursor)){
                    if (get_color_theme() == LIGHTMODE)
                        paint_image(drawingLight_single, 0, 0);
                    else paint_image(drawingDark_single, 0, 0);

                    cursor->pencilSize = 2;
                    cursor->pencilColor = get_color_theme() == LIGHTMODE ? 0x000000 : 0xffffff;

                    currentState = DRAWING_SINGLE_PLAYER;
                }   
                if(process_multiplayer_button(cursor)) {
                    paint_multiplayer_menu();
                    process_start_button(cursor);
                    process_guess_button(cursor);
                    process_exit_button(cursor);
                    currentState = MULTIPLAYER_MENU;
                }
                if (process_exit_button(cursor))
                    return 'e';
            } else if (interrupt == KEYBOARD){
                uint8_t scancode = *get_scancode();
                if (scancode == ESC_BREAK)
                    return 'e';
            } else if (interrupt == RTC) {
                paint_main_menu();
                process_singleplayer_button(cursor);
                process_multiplayer_button(cursor);
            }
            break;
        case MULTIPLAYER_MENU:
            if (interrupt == MOUSE) {
                move_cursor(cursor, get_packet().delta_x, get_packet().delta_y);
                if(process_start_button(cursor)) {
                    currentState = WAITING_DRAWING;
                    paint_waiting_drawing();
                    queue_push(QUEUE_TRANSMIT, READY_DRAW);
                    sp_write();
                }
                if (process_guess_button(cursor)) {
                    currentState = WAITING_GUESSING;
                    paint_waiting_guessing();
                    queue_push(QUEUE_TRANSMIT, READY_GUESS);
                    sp_write();
                }
                if (process_exit_button(cursor)){
                    currentState = MAIN_MENU;
                    paint_main_menu();
                    process_singleplayer_button(cursor);
                    process_multiplayer_button(cursor);
                }
            } else if (interrupt == KEYBOARD){
                uint8_t scancode = *get_scancode();
                if (scancode == ESC_BREAK){
                    currentState = MAIN_MENU;
                    paint_main_menu();
                    process_singleplayer_button(cursor);
                    process_multiplayer_button(cursor);
                }
            } else if (interrupt == RTC) {
                    paint_multiplayer_menu();
                    process_start_button(cursor);
                    process_guess_button(cursor);
                    process_exit_button(cursor);
            }
            break;
        case WAITING_GUESSING:
            if (interrupt == KEYBOARD) {
                uint8_t scancode = *get_scancode();
                if (scancode == ESC_BREAK) {
                    queue_push(QUEUE_TRANSMIT, UNREADY);
                    queue_push(QUEUE_TRANSMIT, UNREADY);
                    sp_write();

                    if (paint_multiplayer_menu() != 0)
                        return 1;
                    process_start_button(cursor);
                    process_guess_button(cursor);
                    process_exit_button(cursor);
                    currentState = MULTIPLAYER_MENU;
                    break;
                }
            } else if (interrupt == MOUSE) {
                move_cursor(cursor, get_packet().delta_x, get_packet().delta_y);
                if (get_color_theme() == LIGHTMODE) cursor->cursorMap = cursor_day;
                else cursor->cursorMap = cursor_night;
            }
            else if (interrupt == RTC) {
                paint_waiting_guessing();
            }

            if (player2State == WAITING_DRAWING){
                if (paint_guessing() != 0) return 1;

                cursor->xpmCode = 3;

                cursor->pencilSize = 2;
                cursor->pencilColor = get_color_theme() == LIGHTMODE ? 0x000000 : 0xffffff;
                
                timer_counter = 0;
                remaining_time = 99;
                counter = 0;
                paint_image(timer_numbers[9], 686, 33);
                paint_image(timer_numbers[9], 708, 33);
                paint_image(guessBox, 550, 50);

                dataState = UNKNOWN_DATA;
                currentState = GUESSING;
            }
            break;
        case WAITING_DRAWING:
            if (interrupt == KEYBOARD) {
                uint8_t scancode = *get_scancode();
                if (scancode == ESC_BREAK) {
                    queue_push(QUEUE_TRANSMIT, UNREADY);
                    queue_push(QUEUE_TRANSMIT, UNREADY);
                    sp_write();

                    if (paint_multiplayer_menu() != 0)
                        return 1;
                    process_start_button(cursor);
                    process_guess_button(cursor);
                    process_exit_button(cursor);
                    currentState = MULTIPLAYER_MENU;
                    break;
                }
            } else if (interrupt == MOUSE) {
                move_cursor(cursor, get_packet().delta_x, get_packet().delta_y);
                if (get_color_theme() == LIGHTMODE) cursor->cursorMap = cursor_day;
                else cursor->cursorMap = cursor_night;
            }
            else if (interrupt == RTC) {
                paint_waiting_drawing();
            }
            
            if (player2State == WAITING_GUESSING){
                if (get_color_theme() == LIGHTMODE)
                    paint_image(drawingLight, 0, 0);
                else paint_image(drawingDark, 0, 0);

                cursor->pencilSize = 2;
                cursor->pencilColor = get_color_theme() == LIGHTMODE ? 0x000000 : 0xffffff;
                
                timer_counter = 0;
                remaining_time = 99;
                counter = 0;
                answer_word = get_word();
                for(size_t j = 0; j < strlen(answer_word); j++) {
                    paint_image(letter_xpm[answer_word[j] - 'a'], 250 + j*15, 25);
                }
                paint_image(timer_numbers[9], 686, 33);
                paint_image(timer_numbers[9], 708, 33);

                dataState = UNKNOWN_DATA;
                currentState = DRAWING;
            }

            break;
        case GUESSING:
            if (interrupt == TIMER) {
                if((timer_counter % sys_hz()) == 0) {
                    remaining_time--;
                    if (remaining_time < 0) {
                        remaining_time = 5;
                        if (get_color_theme() == LIGHTMODE) {
                            paint_image(loseLight, 0, 0);
                            cursor->cursorMap = cursor_day;
                        }
                        else {
                            paint_image(loseDark, 0, 0);
                            cursor->cursorMap = cursor_night;
                        }
                        cursor->xpmCode = 0;
                        currentState = FINISH;
                    } else {
                        int tens = remaining_time / 10;
                        int ones = remaining_time % 10;
                        paint_image(blank, 697, 33);
                        paint_image(timer_numbers[tens], 686, 33);
                        paint_image(timer_numbers[ones], 708, 33);
                    }
                }
            } else if (interrupt == KEYBOARD) {
                uint8_t scancode = *get_scancode();
                if (scancode == ESC_BREAK) {
                    queue_push(QUEUE_TRANSMIT, UNREADY);
                    queue_push(QUEUE_TRANSMIT, UNREADY);
                    sp_write();

                    cursor->xpmCode = 0;
                    if (paint_multiplayer_menu() != 0)
                        return 1;
                    process_start_button(cursor);
                    process_guess_button(cursor);
                    process_exit_button(cursor);
                    currentState = MULTIPLAYER_MENU;
                    break;
                }

                if (scancode == BACKSPACE_MAKE && counter > 0) {
                    counter--;
                    paint_image(delete_letter, 120 + counter*15, 50);
                    paint_image(guessBox, 550, 50);

                    queue_push(QUEUE_TRANSMIT, DEL_MESSAGE);
                    sp_write();
                    break;
                }

                if (scancode == ENTER_MAKE) {
                    while (counter > 0) {
                        counter--;
                        paint_image(delete_letter, 120 + counter*15, 50);
                    }
                    queue_push(QUEUE_TRANSMIT, SUBMIT_MESSAGE);
                    sp_write();
                    break;
                }

                for (uint8_t i = 0; i < 26; i++) {
                    if (scancode == make_codes[i] && counter < 26){
                        paint_image(letter_xpm[i], 120 + counter*15, 50);
                        counter++;

                        paint_image(guessBox, 550, 50);

                        queue_push(QUEUE_TRANSMIT, i);
                        sp_write();
                        break;
                    }
                }
            }

            if (!sp_state_data) {
                switch (dataState) {
                case UNKNOWN_DATA:
                    if (serial_i == 0) {
                        serial_data[0] = sp_data;
                        serial_i++;
                    } else if (serial_i == 1) {
                        serial_data[1] = sp_data;
                        
                        if (serial_data[0] == UNREADY && serial_data[1] == UNREADY) {
                            serial_i = 0;
                            player2State = MULTIPLAYER_MENU;
                        }
                        else if (serial_data[0] == SCREEN_CLEAR && serial_data[1] == SCREEN_CLEAR) {
                            serial_i = 0;
                            vg_draw_rectangle(4, 114, 792, 482, get_color_theme() == LIGHTMODE ? 0xffffff : 0x121212);
                        }
                        else if (serial_data[0] == PENCIL_SIZE && serial_data[1] == PENCIL_SIZE) {
                            serial_i = 0;
                            dataState = PENCIL_SIZE_DATA;
                        }
                        else if (serial_data[0] == PENCIL_COLOR && serial_data[1] == PENCIL_COLOR) {
                            serial_i = 0;
                            dataState = PENCIL_COLOR_DATA;
                        }
                        else if (serial_data[0] == RIGHT_GUESS && serial_data[1] == RIGHT_GUESS) {
                            serial_i = 0;
                            remaining_time = 5;
                            if (get_color_theme() == LIGHTMODE) {
                                paint_image(winLight, 0, 0);
                                cursor->cursorMap = cursor_day;
                            }
                            else {
                                paint_image(winDark, 0, 0);
                                cursor->cursorMap = cursor_night;
                            }
                            cursor->xpmCode = 0;
                            currentState = FINISH;
                            player2State = FINISH;
                        } 
                        else if (serial_data[0] == WRONG_GUESS && serial_data[1] == WRONG_GUESS) {
                            serial_i = 0;
                            paint_image(guessCross, 550, 50);
                        }
                        else {
                            serial_i++;
                            dataState = MOUSE_DATA;
                        }
                    }
                    break;

                case PENCIL_SIZE_DATA:
                    cursor->pencilSize = sp_data;
                    dataState = UNKNOWN_DATA;
                    break;

                case PENCIL_COLOR_DATA:
                    serial_data[serial_i] = sp_data;
                    if (serial_i == 2) {
                        uint32_t color = serial_data[0] | (serial_data[1] << 8) | (serial_data[2] << 16);
                        cursor->pencilColor = color;

                        dataState = UNKNOWN_DATA;
                        serial_i = 0;
                    } else serial_i++;
                    break;

                case MOUSE_DATA:
                    serial_data[serial_i] = sp_data;
                    if (serial_i == 7) {
                        cursor->x = serial_data[0] | (serial_data[1] << 8);
                        cursor->y = serial_data[2] | (serial_data[3] << 8);
                        int16_t delta_x = (int16_t)((serial_data[4] & 0xFF) | ((serial_data[5] << 8) & 0xFF00));
                        int16_t delta_y = (int16_t)((serial_data[6] & 0xFF) | ((serial_data[7] << 8) & 0xFF00));

                        draw_and_move_cursor(cursor, delta_x, delta_y, cursor->pencilColor);

                        dataState = UNKNOWN_DATA;
                        serial_i = 0;
                    } else serial_i++;
                    break;
                default:
                    break;
                }
            }

            if (player2State == MULTIPLAYER_MENU) {
                paint_waiting_guessing();
                currentState = WAITING_GUESSING;
                break;
            }

            break;
        case DRAWING:
            if (interrupt == KEYBOARD){
                uint8_t scancode = *get_scancode();
                if (scancode == ESC_BREAK) {
                    queue_push(QUEUE_TRANSMIT, UNREADY);
                    queue_push(QUEUE_TRANSMIT, UNREADY);
                    sp_write();

                    if (get_color_theme() == LIGHTMODE) cursor->cursorMap = cursor_day;
                    else cursor->cursorMap = cursor_night;
                    
                    if (paint_multiplayer_menu() != 0)
                        return 1;
                    process_start_button(cursor);
                    process_guess_button(cursor);
                    process_exit_button(cursor);
                    currentState = MULTIPLAYER_MENU;
                    break;
                }
            }
            else if (interrupt == MOUSE) {
                drawing_gesture_handler(cursor, get_packet().lb, get_packet().delta_x, get_packet().delta_y);
                drawing_change_state(&drawingState);
                
                if (drawingState == DRAW) {
                    uint8_t lsb, msb;
                    if (util_get_MSB(cursor->x, &msb) != 0) return 1;
                    if (util_get_LSB(cursor->x, &lsb) != 0) return 1;
                    queue_push(QUEUE_TRANSMIT, lsb);
                    queue_push(QUEUE_TRANSMIT, msb);
                    if (util_get_MSB(cursor->y, &msb) != 0) return 1;
                    if (util_get_LSB(cursor->y, &lsb) != 0) return 1;
                    queue_push(QUEUE_TRANSMIT, lsb);
                    queue_push(QUEUE_TRANSMIT, msb);
                    if (util_get_MSB(get_packet().delta_x, &msb) != 0) return 1;
                    if (util_get_LSB(get_packet().delta_x, &lsb) != 0) return 1;
                    queue_push(QUEUE_TRANSMIT, lsb);
                    queue_push(QUEUE_TRANSMIT, msb);
                    if (util_get_MSB(get_packet().delta_y, &msb) != 0) return 1;
                    if (util_get_LSB(get_packet().delta_y, &lsb) != 0) return 1;
                    queue_push(QUEUE_TRANSMIT, lsb);
                    queue_push(QUEUE_TRANSMIT, msb);

                    draw_and_move_cursor(cursor, get_packet().delta_x, get_packet().delta_y, cursor->pencilColor);
                    
                } else if (drawingState == B_PAINT) {
                    switch (click_on_button(cursor)) {
                    case B_RED:
                        cursor->pencilColor = 0xff0000;
                        queue_push(QUEUE_TRANSMIT, PENCIL_COLOR);
                        queue_push(QUEUE_TRANSMIT, PENCIL_COLOR);
                        queue_push(QUEUE_TRANSMIT, 0x00);
                        queue_push(QUEUE_TRANSMIT, 0x00);
                        queue_push(QUEUE_TRANSMIT, 0xff);
                        break;
                    case B_YELLOW:
                        cursor->pencilColor = 0xebff00;
                        queue_push(QUEUE_TRANSMIT, PENCIL_COLOR);
                        queue_push(QUEUE_TRANSMIT, PENCIL_COLOR);
                        queue_push(QUEUE_TRANSMIT, 0x00);
                        queue_push(QUEUE_TRANSMIT, 0xff);
                        queue_push(QUEUE_TRANSMIT, 0xeb);
                        break;
                    case B_BLUE:
                        cursor->pencilColor = 0x00f0ff;
                        queue_push(QUEUE_TRANSMIT, PENCIL_COLOR);
                        queue_push(QUEUE_TRANSMIT, PENCIL_COLOR);
                        queue_push(QUEUE_TRANSMIT, 0xff);
                        queue_push(QUEUE_TRANSMIT, 0xf0);
                        queue_push(QUEUE_TRANSMIT, 0x00);
                        break;
                    case B_GREEN:
                        cursor->pencilColor = 0x05ff00;
                        queue_push(QUEUE_TRANSMIT, PENCIL_COLOR);
                        queue_push(QUEUE_TRANSMIT, PENCIL_COLOR);
                        queue_push(QUEUE_TRANSMIT, 0x00);
                        queue_push(QUEUE_TRANSMIT, 0xff);
                        queue_push(QUEUE_TRANSMIT, 0x05);
                        break;
                    case B_BLACK:
                        cursor->pencilColor = 0x000000;
                        queue_push(QUEUE_TRANSMIT, PENCIL_COLOR);
                        queue_push(QUEUE_TRANSMIT, PENCIL_COLOR);
                        queue_push(QUEUE_TRANSMIT, 0x00);
                        queue_push(QUEUE_TRANSMIT, 0x00);
                        queue_push(QUEUE_TRANSMIT, 0x00);
                        break;
                    case B_WHITE:
                        cursor->pencilColor = 0xffffff;
                        queue_push(QUEUE_TRANSMIT, PENCIL_COLOR);
                        queue_push(QUEUE_TRANSMIT, PENCIL_COLOR);
                        queue_push(QUEUE_TRANSMIT, 0xff);
                        queue_push(QUEUE_TRANSMIT, 0xff);
                        queue_push(QUEUE_TRANSMIT, 0xff);
                        break;
                    case B_CLEAR:
                        vg_draw_rectangle(4, 114, 792, 482, get_color_theme() == LIGHTMODE ? 0xffffff : 0x121212);
                        queue_push(QUEUE_TRANSMIT, SCREEN_CLEAR);
                        queue_push(QUEUE_TRANSMIT, SCREEN_CLEAR);
                        break;
                    case B_SMALL:
                        cursor->pencilSize = 2;
                        queue_push(QUEUE_TRANSMIT, PENCIL_SIZE);
                        queue_push(QUEUE_TRANSMIT, PENCIL_SIZE);
                        queue_push(QUEUE_TRANSMIT, 2);
                        break;
                    case B_MEDIUM:
                        cursor->pencilSize = 3;
                        queue_push(QUEUE_TRANSMIT, PENCIL_SIZE);
                        queue_push(QUEUE_TRANSMIT, PENCIL_SIZE);
                        queue_push(QUEUE_TRANSMIT, 3);
                        break;
                    case B_LARGE:
                        cursor->pencilSize = 4;
                        queue_push(QUEUE_TRANSMIT, PENCIL_SIZE);
                        queue_push(QUEUE_TRANSMIT, PENCIL_SIZE);
                        queue_push(QUEUE_TRANSMIT, 4);
                        break;
                    default:
                        break;
                    }
                    drawingState = IDLE_OTHER;
                } else {
                    move_cursor(cursor, get_packet().delta_x, get_packet().delta_y);
                }

                sp_write();

            }
            else if (interrupt == TIMER) {
                if((timer_counter % sys_hz()) == 0) {
                        remaining_time--;
                    if (remaining_time <= 0) {
                        remaining_time = 5;
                        if (get_color_theme() == LIGHTMODE) {
                            paint_image(loseLight, 0, 0);
                            cursor->cursorMap = cursor_day;
                        }
                        else {
                            paint_image(loseDark, 0, 0);
                            cursor->cursorMap = cursor_night;
                        }
                        cursor->xpmCode = 0;
                        currentState = FINISH;
                        player2State = FINISH;
                    } else {
                        int tens = remaining_time / 10;
                        int ones = remaining_time % 10;
                        paint_image(blank, 697, 33);
                        paint_image(timer_numbers[tens], 686, 33);
                        paint_image(timer_numbers[ones], 708, 33);
                    }
                }
            }

            if (player2State == MULTIPLAYER_MENU) {
                    cursor->xpmCode = 0;
                    if (get_color_theme() == LIGHTMODE)
                        cursor->cursorMap = cursor_day;
                    else cursor->cursorMap = cursor_night;
                    paint_waiting_drawing();
                    currentState = WAITING_DRAWING;
                    break;
            }

            if (!sp_state_data) {
                if(sp_data == DEL_MESSAGE) {
                    counter--;
                    guess_word[counter] = '\0';
                } else if (sp_data == SUBMIT_MESSAGE) {
                    if(strcmp(guess_word, answer_word) == 0) {
                        queue_push(QUEUE_TRANSMIT, RIGHT_GUESS);
                        queue_push(QUEUE_TRANSMIT, RIGHT_GUESS);
                        remaining_time = 5;
                        if (get_color_theme() == LIGHTMODE) {
                            paint_image(winLight, 0, 0);
                            cursor->cursorMap = cursor_day;
                        }
                        else {
                            paint_image(winDark, 0, 0);
                            cursor->cursorMap = cursor_night;
                        }
                        cursor->xpmCode = 0;
                        currentState = FINISH;
                        player2State = FINISH;
                    } else {
                        counter = 0;
                        guess_word[0] = '\0';
                        queue_push(QUEUE_TRANSMIT, WRONG_GUESS);
                        queue_push(QUEUE_TRANSMIT, WRONG_GUESS);
                    }
                    sp_write();
                } else {
                    guess_word[counter++] = sp_data + 'a';
                    guess_word[counter] = '\0';
                }
            }   
            break;
        case DRAWING_SINGLE_PLAYER:
            if (interrupt == KEYBOARD){
                uint8_t scancode = *get_scancode();
                if (scancode == ESC_BREAK) {
                    
                    if (get_color_theme() == LIGHTMODE) cursor->cursorMap = cursor_day;
                    else cursor->cursorMap = cursor_night;
                    if (paint_main_menu() != 0)
                        return 1;
                    process_singleplayer_button(cursor);
                    process_multiplayer_button(cursor);
                    currentState = MAIN_MENU;
                    break;
                }
            }
            else if (interrupt == MOUSE){
                drawing_gesture_handler(cursor, get_packet().lb, get_packet().delta_x, get_packet().delta_y);
                drawing_change_state(&drawingState);

                drawing_interpret_state(cursor,&drawingState);
            }
            break;
        case FINISH:
            if (interrupt == TIMER) {
                if((timer_counter % sys_hz()) == 0) {
                    remaining_time--;
                    if (remaining_time <= 0) {
                        currentState = MULTIPLAYER_MENU;
                        player2State = MULTIPLAYER_MENU;
                        
                        paint_multiplayer_menu();
                        process_start_button(cursor);
                        process_guess_button(cursor);
                        process_exit_button(cursor);
                    }
                }
            }
            else if (interrupt == MOUSE) {
                move_cursor(cursor, get_packet().delta_x, get_packet().delta_y);
            }
            else if (interrupt == RTC) {
                if (get_color_theme() == LIGHTMODE) {
                    paint_image(loseLight, 0, 0);
                }
                else {
                    paint_image(loseDark, 0, 0);
                }
            }

            break;

        default:
            break;
    }
    return 0;
}

