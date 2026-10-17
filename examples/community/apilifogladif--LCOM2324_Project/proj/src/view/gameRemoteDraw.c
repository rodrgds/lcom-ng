#include <lcom/lcf.h>
#include "gameRemoteDraw.h"

#include "../view/assetDraw.h"

#include "controllers/keyboard/kbd.h"
#include "controllers/mouse/mouse.h"
#include "controllers/video_graphics/graphics.h"
#include "controllers/timer/timer.h"
#include "controllers/serialport/serialport.h"
#include "controllers/sprites/sprite.h"
#include "controllers/macros.h"

#include "handler/gameRemoteController.h"

#include "model/boats.h"
#include "model/game.h"

#include "handler/menuController.h"

extern Player p1;
extern bool cursorIsActive;
extern bool keyboardIsActive;
extern int x;
extern int y;
extern Sprite *crosshair;
bool boatsDrawn = false;
extern bool oneIsSelected;
extern Spot matrix[10][10];
extern enum MODE menu;

extern enum THISPLAYER player_status;

void drawGameRemote() {
    if (player_status == WAITING || player_status == CONNECTING) {
        cursorIsActive = false;
        keyboardIsActive = true;
        drawRemoteBoard();
        drawWaiting();
    } 
    else if (player_status == PLACING) {
        cursorIsActive = false;
        keyboardIsActive = true;
        drawBoard();
        if(!boatsDrawn){load_boats(); boatsDrawn = true;}
        drawBoats();
    }
    else if (player_status == MYTURN) {
        cursorIsActive = true;
        keyboardIsActive = true;
        drawRemoteBoard();
        drawMyTurn();
        drawGameRemoteCursor();
    }
    swap_buffers();
}

void drawRemoteBoard() {
    drawGameBoard();
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (p1.matrix[i][j].attacked == 2) draw_sprite(p1.matrix[i][j].missed);
            else if (p1.matrix[i][j].attacked == 1) {
                draw_sprite(p1.matrix[i][j].hit);
            }
        }
    }
}

void drawGameRemoteCursor() {
    if (!cursorIsActive) return;
    if (player_status == MYTURN) drawCrosshair();
}
