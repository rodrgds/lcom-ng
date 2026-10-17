#include <lcom/lcf.h>

#include "game/game.h"

extern GameState gameState;
extern int timer_cnt;
extern uint8_t scancode;
extern int option;
extern int laser_index;
extern int selected_cannon;
extern int ammo;
extern bool out_of_ammo;

extern int settings_option;
extern int resolution_option;
extern int game_over_option;
extern int pause_option;

extern int byte_index;
extern struct packet pp;
extern bool is_mouse_over;

extern powerup_t powerups[100];
extern int powerup_index;

bool rtc_active = true;

int freq = 40;

int run_game() {
    if (mouse_write(SET_STREAM_MODE) != 0) return 1;
    if (mouse_write(ENABLE_DATA_REPORT) != 0) return 1;
    
    if (timer_set_frequency(0, 60) != 0) return 1;

    int ipc_status;
    message msg;
    uint8_t irq_timer, irq_mouse, irq_keyboard, irq_rtc;

    if (timer_subscribe_int(&irq_timer) != 0) return 1;
    if (mouse_subscribe_int(&irq_mouse) != 0) return 1;
    if (keyboard_subscribe_int(&irq_keyboard) != 0) return 1;
    if (rtc_subscribe_int(&irq_rtc) != 0) {printf("error subscribing rtc\n");return 1;}

    while (gameState != EXIT) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) {
            printf("Error driver_receive");
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_mouse) {
                        mouse_ih();
                        handle_mouse();
                        update_mouse();
                        interpret_mouse(); 
                        draw_mouse();                      
                    }

                    if (msg.m_notify.interrupts & irq_timer) {
                        timer_int_handler();
                        switch (gameState) {
                            case MENU:
                                draw_menu();
                                if (rtc_active) {
                                    get_date_time();
                                    draw_date_time();
                                }
                                break;
                            case GAME:
                                update_lasers();
                                update_asteroids();
                                update_difficulty();
                                if (timer_cnt % freq == 0) create_asteroid();
                                create_powerup();
                                update_powerups();
                                draw_game();
                                break;
                            case SETTINGS:
                                draw_settings();
                                if (rtc_active) {
                                    get_date_time();
                                    draw_date_time();
                                }
                                break;
                            case INSTRUCTIONS:
                                draw_instructions();
                                if (rtc_active) {
                                    get_date_time();
                                    draw_date_time();
                                }
                                break;
                            case GAME_OVER:
                                draw_game();
                                draw_game_over();
                                break;
                            case PAUSE:
                                draw_game();
                                draw_pause();
                                break;
                            case LEADERBOARD:
                                draw_leaderboard();
                                if (rtc_active) {
                                    get_date_time();
                                    draw_date_time();
                                }
                                break;
                            case EXIT:
                                break;
                        }
                        draw_mouse();
                        swap_buffers();
                    }

                    if (msg.m_notify.interrupts & irq_keyboard) {
                        kbc_ih();
                        interpret_scancode();
                    }

                    if (msg.m_notify.interrupts & irq_rtc) {
                       printf("rtc interrupt\n");
                    }

                    break;
                default:
                    break;
            }
        }
    } 

    if (timer_unsubscribe_int() != 0) return 1;
    if (mouse_unsubscribe_int() != 0) return 1;
    if (keyboard_unsubscribe_int() != 0) return 1;
    if (rtc_unsubscribe_int() != 0) return 1;

    if (mouse_write(DISABLE_DATA_REPORT) != 0) return 1;

    return 0;
}

void interpret_scancode() {
    switch (gameState) {
        case MENU: {
            switch (scancode) {
                case ESC_BREAK:
                    gameState = EXIT;
                    break;
                case ARROW_UP_BREAK:
                    if (option > 0) option--;
                    else option = 4;
                    break;
                case ARROW_DOWN_BREAK:
                    if (option < 4) option++;
                    else option = 0;
                    break;
                case R_BREAK:
                    rtc_active = !rtc_active;
                    break;
                case ENTER_BREAK:
                    switch (option) {
                        case 0:
                            gameState = GAME;
                            break;
                        case 1:
                            gameState = SETTINGS;
                            break;
                        case 2:
                            gameState = INSTRUCTIONS;
                            break;
                        case 3:
                            gameState = EXIT;
                            break;
                        case 4:
                            gameState = LEADERBOARD;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
            break;
        }

        case GAME: {
            switch (scancode) {
                case ESC_BREAK:
                    gameState = PAUSE;
                    break;
                case ARROW_LEFT_BREAK:
                    selected_cannon--;
                    if (selected_cannon < 0) selected_cannon = 7;
                    break;
                case ARROW_RIGHT_BREAK:
                    selected_cannon++;
                    if (selected_cannon > 7) selected_cannon = 0;
                    break;
                case SPACE_BREAK:
                    if (!out_of_ammo)
                        create_laser();
                    break;
                case ARROW_DOWN_BREAK:
                    out_of_ammo = false;
                    ammo = 5;
                    break;
                default:
                    break;
            }
            break;
        }

        case SETTINGS: {
            switch (scancode) {
                case ARROW_UP_BREAK:
                    if (settings_option > 0) settings_option--;
                    else settings_option = 2;
                    break;
                case ARROW_DOWN_BREAK:
                    if (settings_option < 2) settings_option++;
                    else settings_option = 0;
                    break;
                case ARROW_LEFT_BREAK:
                    if (settings_option == 0) {
                        if (resolution_option > 0) resolution_option--;
                        else resolution_option = 4;
                    }
                    break;
                case ARROW_RIGHT_BREAK:
                    if (settings_option == 0) {
                        if (resolution_option < 4) resolution_option++;
                        else resolution_option = 0;
                    }
                    break;
                case R_BREAK:
                    rtc_active = !rtc_active;
                    break;
                case ENTER_BREAK:
                    switch (settings_option) {
                        case 0:
                            switch (resolution_option) {
                                case 0:
                                    printf("640x480\n");
                                    break;
                                case 1:
                                    printf("800x600\n");
                                    break;
                                case 2:
                                    printf("1024x768\n");
                                    break;
                                case 3:
                                    printf("1152x864\n");
                                    break;
                                case 4:
                                    printf("1280x1024\n");
                                    break;
                            }
                            break;
                        case 1:
                            printf("Leaderboard cleared");
                            break;
                        case 2:
                            gameState = MENU;
                            break;
                        default:
                            break;
                    }
                    break;
                case ESC_BREAK:
                    gameState = MENU;
                    break;
                default:
                    break;
            }
            break;
        }

        case INSTRUCTIONS: {
            if (scancode == ESC_BREAK) gameState = MENU;
            else if (scancode == R_BREAK) rtc_active = !rtc_active;
            break;
        }

        case GAME_OVER:
            switch (scancode) {
                case ARROW_UP_BREAK:
                    if (game_over_option > 0) game_over_option--;
                    else game_over_option = 1;
                    break;
                case ARROW_DOWN_BREAK:
                    if (game_over_option < 1) game_over_option++;
                    else game_over_option = 0;
                    break;
                case ENTER_BREAK:
                    switch (game_over_option) {
                        case 0:
                            gameState = GAME;
                            reset_game();
                            break;
                        case 1:
                            gameState = MENU;
                            reset_game();
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
            break;

        case LEADERBOARD:
            if (scancode == ESC_BREAK) gameState = MENU;
            else if (scancode == R_BREAK) rtc_active = !rtc_active;
            break;

        case PAUSE:
            switch (scancode) {
                case ARROW_UP_BREAK:
                    if (pause_option > 0) pause_option--;
                    else pause_option = 1;
                    break;
                case ARROW_DOWN_BREAK:
                    if (pause_option < 1) pause_option++;
                    else pause_option = 0;
                    break;
                case ENTER_BREAK:
                    switch (pause_option) {
                        case 0:
                            gameState = GAME;
                            break;
                        case 1:
                            gameState = MENU;
                            reset_game();
                            break;
                        default:
                            break;
                    }
                    break;
            }
            break;
        default:
            break;
    }
}

void interpret_mouse() {
    if (is_mouse_over && pp.lb) {
        switch (gameState) {
            case MENU:
                switch (option) {
                    case 0:
                        gameState = GAME;
                        break;
                    case 1:
                        gameState = SETTINGS;
                        break;
                    case 2:
                        gameState = INSTRUCTIONS;
                        break;
                    case 3:
                        gameState = EXIT;
                        break;
                    case 4:
                        gameState = LEADERBOARD;
                        break;
                    default:
                        break;
                }
                break;
            case SETTINGS:
                switch (settings_option) {
                    case 0:
                        switch (resolution_option) {
                            case 0:
                                printf("640x480\n");
                                break;
                            case 1:
                                printf("800x600\n");
                                break;
                            case 2:
                                printf("1024x768\n");
                                break;
                            case 3:
                                printf("1152x864\n");
                                break;
                            case 4:
                                printf("1280x1024\n");
                                break;
                        }
                        break;
                    case 1:
                        printf("Leaderboard cleared");
                        break;
                    case 2:
                        gameState = MENU;
                        break;
                    default:
                        break;
                }
                break;

            case GAME_OVER:
                switch (game_over_option) {
                    case 0:
                        gameState = GAME;
                        reset_game();
                        break;
                    case 1:
                        gameState = MENU;
                        reset_game();
                        break;
                    default:
                        break;
                }
                break;

            case PAUSE:
                switch (pause_option) {
                    case 0:
                        gameState = GAME;
                        break;
                    case 1:
                        gameState = MENU;
                        reset_game();
                        break;
                    default:
                        break;
                }
                break;

            case GAME:
                break;
            default:
                break;
        }
    }
    else if (pp.lb) {
        switch (gameState) {
            case GAME:
                for (int i = 0; i < powerup_index; i++) {
                    if (cursor->x > powerups[i].x - 50 && cursor->x < powerups[i].x + 50 && cursor->y > powerups[i].y - 50 && cursor->y < powerups[i].y + 50) {
                        powerups[i].is_consumed = true;
                    }
                }
                break;
            default:
                break;
        }
    }
}
