#include <lcom/lcf.h>
#include "gameLocalController.h"

extern bool cursor_active;
extern int global_counter;
extern bool changeTurn;
extern enum GAMESTATE gameState;
extern Player p1;
extern Player p2;
extern Sprite *crosshair;
int winner;

int checkKeyboardInputGameLocal(uint8_t scancode) {
    switch (scancode) {
        case ESC_BREAKCODE:
            return 1;
        default:
            return 0;
    }
}

void checkGameLocalClick() {
    int coor_x = (crosshair->x - 53)/BIG_BOARD_SQUARE;
    int coor_y = (crosshair->y - 95)/BIG_BOARD_SQUARE;
    if (coor_x < 0 || coor_x >= MATRIX_SIZE || coor_y < 0 || coor_y >= MATRIX_SIZE) return;
    cursor_active = false;
    drawGameLocal();
    
    if (gameState == TURN1) {
        if (p2.matrix[coor_y][coor_x].attacked != 0) return;        
        
        if (p2.matrix[coor_y][coor_x].taken == 0) {
            p2.matrix[coor_y][coor_x].attacked = 2;
            gameState = TURN2;

        }
        else {
            p2.matrix[coor_y][coor_x].attacked = 1;
            updateDamage(p2.matrix[coor_y][coor_x].taken, 2);
        }

        if(drawPlayerSecretBoard(2)) {
            winner = 1;
            gameState = END;
        }

        global_counter = 0;
        changeTurn = false;
    }
    else if (gameState == TURN2) {
        if (p1.matrix[coor_y][coor_x].attacked != 0) return;

        if (p1.matrix[coor_y][coor_x].taken == 0) {
            p1.matrix[coor_y][coor_x].attacked = 2;
            gameState = TURN1;
        }
        else {
            p1.matrix[coor_y][coor_x].attacked = 1;
            updateDamage(p1.matrix[coor_y][coor_x].taken, 1);
        }

        if(drawPlayerSecretBoard(1)) {
            winner = 2;
            gameState = END;
        }

        global_counter = 0;
        changeTurn = false;
    }

    swap_buffers();
    sleep(1);
    cursor_active = true;
}
