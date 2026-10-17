#include <lcom/lcf.h>
#include "menuController.h"
#include "controllers/serialport/serialport.h"


extern Sprite *button_local;
extern Sprite *button_help;
extern Sprite *button_quit;
extern Sprite *button_remote;

extern enum MODE menu;
extern enum PLACINGSTATE state;
extern bool drawn;

// mouse position
extern int x;
extern int y;

bool checkMainClick() {
    if (y > button_local->y && y < button_local->y + button_local->height) {
        if (x > button_local->x && x < button_local->x + button_local->width) {
            clearBoard();
            drawn = false;
            state = PLAYER1;
            menu = PLACINGMENU;
            return true;
        }
    }
    else if (y > button_remote->y && y < button_remote->y + button_remote->height) {
        if (x > button_remote->x && x < button_remote->x + button_remote->width) {
            menu = GAMEREMOTE;
            send(0xef);
            return true;
        }
    }
    else if (y > button_help->y && y < button_help->y + button_help->height) {
        if (x > button_help->x && x < button_help->x + button_help->width) {
            menu = HELP;
            return true;
        }
    }
    else if (y > button_quit->y && y < button_quit->y + button_quit->height) {
        if (x > button_quit->x && x < button_quit->x + button_quit->width) {
            menu = QUIT;
            return true;
        }
    }
    return false;
}

int checkKeyboardInputMainMenu(uint8_t scancode) {
    if (scancode == ESC_BREAKCODE) {
        x = 575;
        y = 735;
        changeMousePosition();
        drawMouse();
    }
    else if (scancode == LOCAL) {
        x = 576;
        y = 385;
        changeMousePosition();
        drawMouse();
    }
    else if (scancode == REMOTE) {
        x = 576;
        y = 500;
        changeMousePosition();
        drawMouse();
    }
    else if (scancode == H) {
        x = 576;
        y = 615;
        changeMousePosition();
        drawMouse();
    }
    return 0;
}
