#include <lcom/lcf.h>
#include "gameLocalDraw.h"
#include "controllers/timer/timer.h"


bool cursor_active = true;
bool keyboard = false;
extern int global_counter;
extern Player p1;
extern Player p2;

enum GAMESTATE gameState = TURN1;
bool changeTurn = false;
int winner;

void drawGameLocal() {
    if (gameState == TURN1) {
        if (changeTurn) {
            gameState = TURN2;
            changeTurn = false;
            global_counter = 0;
        }
        drawGameBoard();
        drawNamePlayer1();
        drawPlayerSecretBoard(2);
        drawPlayerSmallSecretBoard(1);
        drawTime();
        drawGameCursor();
    }
    else if (gameState == TURN2) {
        if (changeTurn) {
            gameState = TURN1;
            changeTurn = false;
            global_counter = 0;
        }
        drawGameBoard();
        drawNamePlayer2();
        drawPlayerSecretBoard(1);
        drawPlayerSmallSecretBoard(2);
        drawTime();
        drawGameCursor();
    }
    else if (gameState == END) {
        cursor_active = false;
        keyboard = true;
        drawEndGame(winner);
    }
}

void drawGameCursor() {
    if (!cursor_active) return;
    drawCrosshair();
}

bool drawPlayerSecretBoard(int p) {
    int end = 0;
    drawSunken(p);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (p == 1) {
                if (p1.matrix[i][j].attacked == 2) draw_sprite(p1.matrix[i][j].missed);
                else if (p1.matrix[i][j].attacked == 1) {
                    draw_sprite(p1.matrix[i][j].hit);
                    end++;
                }
            }
            else if (p == 2) {
                if (p2.matrix[i][j].attacked == 2) draw_sprite(p2.matrix[i][j].missed);
                else if (p2.matrix[i][j].attacked == 1) {
                    draw_sprite(p2.matrix[i][j].hit);
                    end++;
                }
            }
        }
    }
    return end == 17;
}

void drawPlayerSmallSecretBoard(int p) {
    drawSmallSunken(p);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (p == 1) {
                if (p1.matrix[i][j].attacked == 2) draw_sprite(p1.matrix[i][j].small_missed);
                else if (p1.matrix[i][j].attacked == 1) draw_sprite(p1.matrix[i][j].small_hit);
            }
            else if (p == 2) {
                if (p2.matrix[i][j].attacked == 2) draw_sprite(p2.matrix[i][j].small_missed);
                else if (p2.matrix[i][j].attacked == 1) draw_sprite(p2.matrix[i][j].small_hit);
            }
        }
    }
}

void drawSunkenBoat(Boat boat) {
    if (!boat.rotated) draw_sprite(boat.sprite);
    else if (boat.rotated) draw_sprite(boat.v_sprite);
}

void drawSunken(int p) {
    if (p == 1) {
        if (p1.boatA.damage == p1.boatA.health) drawSunkenBoat(p1.boatA);
        if (p1.boatB.damage == p1.boatB.health) drawSunkenBoat(p1.boatB);
        if (p1.boatC.damage == p1.boatC.health) drawSunkenBoat(p1.boatC);
        if (p1.boatD.damage == p1.boatD.health) drawSunkenBoat(p1.boatD);
        if (p1.boatE.damage == p1.boatE.health) drawSunkenBoat(p1.boatE);
    }
    else if (p == 2) {
        if (p2.boatA.damage == p2.boatA.health) drawSunkenBoat(p2.boatA);
        if (p2.boatB.damage == p2.boatB.health) drawSunkenBoat(p2.boatB);
        if (p2.boatC.damage == p2.boatC.health) drawSunkenBoat(p2.boatC);
        if (p2.boatD.damage == p2.boatD.health) drawSunkenBoat(p2.boatD);
        if (p2.boatE.damage == p2.boatE.health) drawSunkenBoat(p2.boatE);
    }
}

void drawSmallSunkenBoat(Boat boat) {
    if (!boat.rotated) draw_sprite(boat.small_sprite);
    else if (boat.rotated) draw_sprite(boat.small_v_sprite);
}

void drawSmallSunken(int p) {
    if (p == 1) {
        if (p1.boatA.damage == p1.boatA.health) drawSmallSunkenBoat(p1.boatA);
        if (p1.boatB.damage == p1.boatB.health) drawSmallSunkenBoat(p1.boatB);
        if (p1.boatC.damage == p1.boatC.health) drawSmallSunkenBoat(p1.boatC);
        if (p1.boatD.damage == p1.boatD.health) drawSmallSunkenBoat(p1.boatD);
        if (p1.boatE.damage == p1.boatE.health) drawSmallSunkenBoat(p1.boatE);
    }
    else if (p == 2) {
        if (p2.boatA.damage == p2.boatA.health) drawSmallSunkenBoat(p2.boatA);
        if (p2.boatB.damage == p2.boatB.health) drawSmallSunkenBoat(p2.boatB);
        if (p2.boatC.damage == p2.boatC.health) drawSmallSunkenBoat(p2.boatC);
        if (p2.boatD.damage == p2.boatD.health) drawSmallSunkenBoat(p2.boatD);
        if (p2.boatE.damage == p2.boatE.health) drawSmallSunkenBoat(p2.boatE);
    }
}
