#include <lcom/lcf.h>
#include "game.h"


Spot matrix[MATRIX_SIZE][MATRIX_SIZE];
Player p1;
Player p2;

Boat *boat_2p; 
Boat *boat_3p1;
Boat *boat_3p2;
Boat *boat_4p;
Boat *boat_5p;

void savePlayer1() {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            p1.matrix[i][j] = matrix[i][j];
        }
    }
    p1.boatA = *boat_2p;
    adjustBoard(p1.boatA);

    p1.boatB = *boat_3p1;
    adjustBoard(p1.boatB);

    p1.boatC = *boat_3p2;
    adjustBoard(p1.boatC);

    p1.boatD = *boat_4p;
    adjustBoard(p1.boatD);

    p1.boatE = *boat_5p;
    adjustBoard(p1.boatE);
}

void savePlayer2() {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            p2.matrix[i][j] = matrix[i][j];
        }
    }
    p2.boatA = *boat_2p;
    adjustBoard(p2.boatA);

    p2.boatB = *boat_3p1;
    adjustBoard(p2.boatB);

    p2.boatC = *boat_3p2;
    adjustBoard(p2.boatC);

    p2.boatD = *boat_4p;
    adjustBoard(p2.boatD);

    p2.boatE = *boat_5p;
    adjustBoard(p2.boatE);
}

void adjustBoard(Boat boat){
    boat.sprite->x -= 32;
    boat.sprite->y -= 9;
    boat.s_sprite->x -= 32;
    boat.s_sprite->y -= 9;
    boat.v_sprite->x -= 32;
    boat.v_sprite->y -= 9;
    boat.sv_sprite->x -= 32;
    boat.sv_sprite->y -= 9;
}

void clearBoard() {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrix[i][j].taken = 0;
        }
    }
}

void createMatrix(int x, int y, int size) {
    Spot *spot = (Spot*)malloc(sizeof(Spot));
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            spot->line = i;
            spot->column = j;
            spot->x_pixel = x + j * size;
            spot->y_pixel = y + i * size;
            spot->small_x_pixel = SMALL_BOARD_X + j * SMALL_BOARD_SQUARE;
            spot->small_y_pixel = SMALL_BOARD_Y + i * SMALL_BOARD_SQUARE;
            spot->taken = 0;
            spot->attacked = 0;
            spot->l = size;
            spot->missed = create_sprite((xpm_map_t) miss_xpm, 85 + j * BIG_BOARD_SQUARE , 126 + i * BIG_BOARD_SQUARE);
            spot->hit = create_sprite((xpm_map_t) explosion_xpm, 85 + j * BIG_BOARD_SQUARE, 126 + i * BIG_BOARD_SQUARE);
            spot->small_missed = create_sprite((xpm_map_t) small_miss_xpm, SMALL_BOARD_X + j * SMALL_BOARD_SQUARE , SMALL_BOARD_Y + i * SMALL_BOARD_SQUARE);
            spot->small_hit = create_sprite((xpm_map_t) small_explosion_xpm, SMALL_BOARD_X + j * SMALL_BOARD_SQUARE, SMALL_BOARD_Y + i * SMALL_BOARD_SQUARE);
            matrix[i][j] = *spot;
        }
    }
}

void updateDamage(int taken, int p){
    if(p == 1){
        if(taken == 2) p1.boatA.damage++;
        else if(taken == 31) p1.boatB.damage++;
        else if(taken == 32) p1.boatC.damage++;
        else if(taken == 4) p1.boatD.damage++;
        else if(taken == 5) p1.boatE.damage++;
    }
    else if(p == 2){
        if(taken == 2) p2.boatA.damage++;
        else if(taken == 31) p2.boatB.damage++;
        else if(taken == 32) p2.boatC.damage++;
        else if(taken == 4) p2.boatD.damage++;
        else if(taken == 5) p2.boatE.damage++;
    }
}

