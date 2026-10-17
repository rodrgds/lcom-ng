#include <lcom/lcf.h>
#include "gameRemoteController.h"

#include "../view/assetDraw.h"

#include "controllers/keyboard/kbd.h"
#include "controllers/mouse/mouse.h"
#include "controllers/video_graphics/graphics.h"
#include "controllers/timer/timer.h"
#include "controllers/serialport/serialport.h"
#include "controllers/sprites/sprite.h"
#include "controllers/macros.h"

#include "view/gameRemoteDraw.h"

#include "model/boats.h"
#include "model/game.h"

#include "menuController.h"

extern Player p1;
bool cursorIsActive = true;
bool keyboardIsActive = false;
extern int x;
extern int y;
extern Sprite *crosshair;
extern bool oneIsSelected;
extern Spot matrix[10][10];
extern enum MODE menu;

int c_attack = -1;
int l_attack = -1;

enum THISPLAYER player_status = PLACING;

bool second = false;

int checkKeyboardInputGameRemote(uint8_t scancode) {
    if (keyboardIsActive) {
        if (player_status == PLACING) {
            if (oneIsSelected) {
                switch (scancode) {
                    case ENTER:
                        drawGameRemote();
                        fixTheSelected();
                        break;
                    case UP:
                        drawGameRemote();
                        moveSelected(0, -1);
                        break;
                    case DOWN:
                        drawGameRemote();
                        moveSelected(0, 1);
                        break;
                    case LEFT:
                        drawGameRemote();
                        moveSelected(-1, 0);
                        break;
                    case RIGTH:
                        drawGameRemote();
                        moveSelected(1, 0);
                        break;
                    case ROTATE:
                        drawGameRemote();
                        rotateSelected();
                        break;
                    case ESC_BREAKCODE:
                        return 1;
                    default:
                        return 0;
                }
            }
            else {
                switch (scancode) {
                    case BOAT_2P:
                        drawGameRemote();
                        drawSelected(2);
                        break;
                    case BOAT_3P_1:
                        drawGameRemote();
                        drawSelected(31);
                        break;
                    case BOAT_3P_2:
                        drawGameRemote();
                        drawSelected(32);
                        break;
                    case BOAT_4P:
                        drawGameRemote();
                        drawSelected(4);
                        break;
                    case BOAT_5P:
                        drawGameRemote();
                        drawSelected(5);
                        break;
                    case NEXT:
                        printf("clicked\n");
                        if (allBoatsFixed()) {
                            if (player_status == PLACING) {
                                savePlayer1();
                                if (second) {
                                    player_status = WAITING;
                                    drawGameRemote();
                                    send(0xfe);
                                }
                                else {
                                    player_status = CONNECTING;
                                    drawGameRemote();
                                    send(0xff);
                                }
                            }
                        }
                        break;
                    case ESC_BREAKCODE:
                        return 1;
                    default:
                        return 0;
                }
            }
        }
        else if (scancode == ESC_BREAKCODE) return 1;
    }
    return 0;
}

void checkGameRemoteClick() {
    if (cursorIsActive) {
        int c = (crosshair->x - 53)/BIG_BOARD_SQUARE;
        int l = (crosshair->y - 95)/BIG_BOARD_SQUARE;
        if (c < 0 || c >= MATRIX_SIZE || l < 0 || l >= MATRIX_SIZE) return;
        cursorIsActive = false;
        if (player_status == MYTURN && p1.matrix[l][c].attacked == 0) {
            c_attack = c;
            l_attack = l;
            uint8_t byte = ((l + 2) << 4) | (c + 2);
            send(byte);
        }
        cursorIsActive = true;
    }
}

int received (uint8_t val) {
    printf("received: %d \n", val);
    if (val == 1 || val == 0xef) return 0;
    if (val == 0xff) {
        second = true;
    }
    else if (menu == GAMEREMOTE) {
        if (val == 0xfe) {
            player_status = MYTURN;
            serial_clear();
            drawGameRemote();
        }
        else if (val == 0xdd) { // hit
            p1.matrix[l_attack][c_attack].attacked = 1;
            printf("attacked = (%d, %d) %d\n", l_attack, c_attack, p1.matrix[l_attack][c_attack].attacked);
            drawGameRemote();
        }
        else if (val == 0xee) { // miss
            p1.matrix[l_attack][c_attack].attacked = 2;
            printf("attacked = (%d, %d) %d\n", l_attack, c_attack, p1.matrix[l_attack][c_attack].attacked);
            player_status = WAITING;
            serial_clear();
            drawGameRemote();
        }
        else {
            uint8_t l = (val >> 4) - 2;
            uint8_t c = (val & 0xf) - 2;
            printf("received: %d -> %d, %d\n", val, l, c);
            printf ("check spot \n");
            if (p1.matrix[l][c].taken != 0) send(0xdd);
            else {
                send(0xee);
                player_status = MYTURN;
                serial_clear();
                drawGameRemote();
            }
        }
    }

    return 0;
}
