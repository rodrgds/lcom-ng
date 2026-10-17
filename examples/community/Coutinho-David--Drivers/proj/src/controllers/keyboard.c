#include <lcom/lcf.h>
#include "keyboard.h"
#include "../game.h"

#define ESC_MAKE        0x01
#define ESC_BREAK       0x81
#define ENTER_MAKE      0x1C
#define ENTER_BREAK     0x9C
#define ARROW_UP_MAKE   0x48
#define ARROW_UP_BREAK  0xC8
#define ARROW_DOWN_MAKE 0x50
#define ARROW_DOWN_BREAK 0xD0
#define ARROW_LEFT_MAKE 0x4B
#define ARROW_LEFT_BREAK 0xCB
#define ARROW_RIGHT_MAKE 0x4D  
#define ARROW_RIGHT_BREAK 0xCD

uint8_t codes[2];
bool expectingSecondByte = false;
uint8_t code;
bool exit_request = false;


int (keyboard_ih)() {

    uint8_t status;
    util_sys_inb(0x64, &status);
    if (status & (BIT(6) | BIT(7) | BIT(5))) return 1; 
    
    util_sys_inb(0x60, &code);

    if (code == 0xE0) {
        codes[0] = code;
        expectingSecondByte = true;
    }

    else if (expectingSecondByte) {
        codes[1] = code;
        expectingSecondByte = false;

        if (codes[0] == 0xE0) {
            switch(codes[1]) {
                case ARROW_UP_MAKE:
                    if (state == MENU_START) state = MENU_EXIT;
                    else if (state == MENU_EXIT) state = MENU_START;
                    else if (state >= 2 && state <=5) state = BET;
                    else if (state == BET) state = bet5;
                    //printf("STATE IS: %d\n", state);
                    break;
                case ARROW_UP_BREAK:
                    ////printf("KBC | Arrow UP released\n");
                    break;
                case ARROW_DOWN_MAKE:
                    if (state == MENU_START) state = MENU_EXIT;
                    else if (state == MENU_EXIT) state = MENU_START;
                    else if (state >= 2 && state <= 5) state = BET;
                    else if (state == BET) state = bet5;
                    //printf("STATE IS: %d\n", state);
                    break;
                case ARROW_DOWN_BREAK:
                    ////printf("KBC | Arrow DOWN released\n");
                    break;
                case ARROW_LEFT_MAKE:
                    if (state >= 3 && state <= 5) {
                        state--;
                    }
                    else if (state == 2) {
                        state = 5;
                    }
                    else if(state >= 8 && state <= 9) {
                        state--;
                    } 
                    else if (state == 7) {
                        state = 9;
                    }
                    else if (state == 11) {
                        state = 10;
                    }
                    else if (state == 13) {
                        state = 12;
                    }
                    else if (state == 15) {
                        state = 14;
                    }
                    else if (state == 17) {
                        state = 16;
                    }
                    //printf("STATE IS: %d\n", state);
                    break;
                case ARROW_LEFT_BREAK:
                    ////printf("KBC | Arrow LEFT released\n");
                    break;
                case ARROW_RIGHT_MAKE:
                    if (state >= 2 && state <= 4) {
                        state++;
                    }
                    else if (state == 5) {
                        state = 2;
                    }
                    else if (state >= 7 && state <= 8) {
                        state++;
                    }
                    else if (state == 9) {
                        state = 7;
                    }
                    else if (state == 10) {
                        state = 11;
                    }
                    else if (state == 12) {
                        state = 13;
                    }
                    else if (state == 14) {
                        state = 15;
                    }
                    else if (state == 16) {
                        state = 17;
                    }
                    //printf("STATE IS: %d\n", state);
                    break;
                case ARROW_RIGHT_BREAK:
                    ////printf("KBC | Arrow RIGHT released\n");
                    break;
                default:
                    // Discard other extended keys
                    ////printf("KBC | Ignored extended key\n");
                    break;
            }
        }
    }
    else {
        codes[0] = code;
        
        switch(code) {
            case ESC_MAKE:
                ////printf("KBC | ESC pressed\n");
                break;
            case ESC_BREAK:
                if (state >= 2) {
                    state = MENU_EXIT;
                }
                //printf("STATE IS: %d\n", state);
                break;
            case ENTER_MAKE:
                ////printf("KBC | ENTER pressed\n");
                break;
            case ENTER_BREAK:
                if (state >= 2 && state <=5) change_bet();
                else if (state == BET) {
                    game_start();
                }
                else if (state == MENU_START || state == 10 || state == 12 || state == 14 || state == 16) {
                    state = bet5;
                }
                else if (state == MENU_EXIT) {
                    exit_request = true;
                }
                else if (state == 7) {
                    game_player_stand();
                }
                else if (state == 8) {
                    game_player_hit();
                }
                else if (state == 9) {
                    game_player_double();
                }
                else if (state == 11 || state == 13 || state == 15 || state == 17) 
                    state = 0;
                break;
            default:
                break;
        }
    }

    return 0;
}
