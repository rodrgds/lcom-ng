#include <lcom/lcf.h>
#include "placingController.h"

extern bool cursorActive;
extern bool keyboardActive;
extern bool drawn;
extern enum PLACINGSTATE state;
extern enum PLACINGSTATE previousState;
extern int x;
extern int y;
extern int global_counter;
extern bool oneIsSelected;

int checkPlacingClick() {
    if (state == ASKINGGAME) {
        if (y > 488 && y < 534) {
            if (x > 222 && x < 345) {
                savePlayer2();
                keyboardActive = true;
                cursorActive = false;
                global_counter = 0;
                return 1;
            }
            else if (x > 836 && x < 936) {
                state = previousState;
                keyboardActive = true;
                cursorActive = false;
                drawPlacingMenu();
                return 0;
            }
        }
    }
    else if (state == ASKINGPLAYER2) {
        if (y > 488 && y < 534) {
            if (x > 222 && x < 345) {
                state = PLAYER2;
                savePlayer1();
                clearBoard();

                drawn = false;
                keyboardActive = true;
                cursorActive = false;
                drawPlacingMenu();
                return 0;
            }
            else if (x > 836 && x < 936) {
                state = previousState;
                keyboardActive = true;
                cursorActive = false;
                drawPlacingMenu();
                return 0;
            }
        }
    }
    else if (state == ASKINGQUIT) {
        if (y > 488 && y < 534) {
            if (x > 222 && x < 345) {
                state = PLAYER1;
                keyboardActive = true;
                cursorActive = false;
                return -1;
            }
            else if (x > 836 && x < 936) {
                state = previousState;
                keyboardActive = true;
                cursorActive = false;
                drawPlacingMenu();
                return 0;
            }
        }
    }
    return 0;
}

void checkKeyboardInputPlacing(uint8_t scancode) {
    if (oneIsSelected) {
        switch (scancode) {
            case ENTER:
                drawPlacingMenu();
                fixTheSelected();
                break;
            case UP:
                drawPlacingMenu();
                moveSelected(0, -1);
                break;
            case DOWN:
                drawPlacingMenu();
                moveSelected(0, 1);
                break;
            case LEFT:
                drawPlacingMenu();
                moveSelected(-1, 0);
                break;
            case RIGTH:
                drawPlacingMenu();
                moveSelected(1, 0);
                break;
            case ROTATE:
                drawPlacingMenu();
                rotateSelected();
                break;
            case ESC_BREAKCODE:
                previousState = state;
                state = ASKINGQUIT;
                cursorActive = true;
                keyboardActive = false;
                drawPlacingMenu();
                break;
            default:
                return;
        }
    }
    else {
        switch (scancode) {
            case BOAT_2P:
                drawPlacingMenu();
                drawSelected(2);
                break;
            case BOAT_3P_1:
                drawPlacingMenu();
                drawSelected(31);
                break;
            case BOAT_3P_2:
                drawPlacingMenu();
                drawSelected(32);
                break;
            case BOAT_4P:
                drawPlacingMenu();
                drawSelected(4);
                break;
            case BOAT_5P:
                drawPlacingMenu();
                drawSelected(5);
                break;
            case NEXT:
                if (allBoatsFixed()) {
                    if (state == PLAYER1) {
                        previousState = state;
                        state = ASKINGPLAYER2;
                        cursorActive = true;
                        keyboardActive = false;
                        drawPlacingMenu();
                    }
                    else if (state == PLAYER2) {
                        previousState = state;
                        state = ASKINGGAME;
                        cursorActive = true;
                        keyboardActive = false;
                        drawPlacingMenu();
                    }
                }
                break;
            case ESC_BREAKCODE:
                previousState = state;
                state = ASKINGQUIT;
                cursorActive = true;
                keyboardActive = false;
                drawPlacingMenu();
                break;
            default:
                return;
        }
    }
}
