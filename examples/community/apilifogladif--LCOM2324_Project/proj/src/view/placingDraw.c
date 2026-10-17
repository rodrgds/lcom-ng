#include <lcom/lcf.h>
#include "placingDraw.h"


enum PLACINGSTATE state = PLAYER1;
enum PLACINGSTATE previousState = PLAYER1;
bool cursorActive = false;
bool keyboardActive = true;
bool drawn = false;

extern int x;
extern int y;

void drawPlacingMenu() {
    if (state == ASKINGQUIT) {
        drawPreviousPlacingMenu();
        drawQuitText();
        drawPlacingCursor();
    }
    else if (state == PLAYER1) {
        cursorActive = false;
        drawBoard();
        drawNamePlayer1();
        if(!drawn){load_boats(); drawn = true;}
        drawBoats();
    }
    else if (state == ASKINGPLAYER2) {
        drawPreviousPlacingMenu();
        drawNextPlayerText();
        drawPlacingCursor();
    }
    else if (state == PLAYER2) {
        cursorActive = false;
        drawBoard();
        drawNamePlayer2();
        if(!drawn){load_boats(); drawn = true;}
        drawBoats();
    }
    else if (state == ASKINGGAME) {
        drawPreviousPlacingMenu();
        drawGoGameText();
        drawPlacingCursor();
    }
}

void drawPreviousPlacingMenu() {
    if (previousState == PLAYER1) {
        drawBoard();
        drawNamePlayer1();
        if(!drawn){load_boats(); drawn = true;}
        drawBoats();
    }
    else if (previousState == PLAYER2) {
        drawBoard();
        drawNamePlayer2();
        if(!drawn){load_boats(); drawn = true;}
        drawBoats();
    }
}

void drawPlacingCursor() {
    if (!cursorActive) return;
    
    if (state == ASKINGGAME) {
        if (y > 488 && y < 534) {
            if (x > 222 && x < 345) {
                drawHandCursor();
                cursorActive = true;
                keyboardActive = false;
                return;
            }
            else if (x > 836 && x < 936) {
                drawHandCursor();
                cursorActive = true;
                keyboardActive = false;
                return;
            }
        }
    }
    else if (state == ASKINGPLAYER2) {
        if (y > 488 && y < 534) {
            if (x > 222 && x < 345) {
                drawHandCursor();
                cursorActive = true;
                keyboardActive = false;
                return;
            }
            else if (x > 836 && x < 936) {
                drawHandCursor();
                cursorActive = true;
                keyboardActive = false;
                return;
            }
        }
    }
    else if (state == ASKINGQUIT) {
        if (y > 488 && y < 534) {
            if (x > 222 && x < 345) {
                drawHandCursor();
                cursorActive = true;
                keyboardActive = false;
                return;
            }
            else if (x > 836 && x < 936) {
                drawHandCursor();
                cursorActive = true;
                keyboardActive = false;
                return;
            }
        }
    }
    drawCursor();
    cursorActive = true;
    keyboardActive = false;
    return;
}
