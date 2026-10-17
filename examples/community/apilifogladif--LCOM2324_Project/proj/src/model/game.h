#ifndef _LCOM_GAME_H_
#define _LCOM_GAME_H_

/**
 * @file game.h
 * 
 * @brief This file contains the game functions.
*/

#include "controllers/sprites/sprite.h"
#include "controllers/macros.h"
#include "assets/popups/explosion.xpm"
#include "assets/popups/miss.xpm"
#include "assets/popups/small_explosion.xpm"
#include "assets/popups/small_miss.xpm"

/**
 * @struct Spot
 * @brief struct that has information about a spot in the board
*/
typedef struct {
    int line;
    int column;
    int x_pixel;
    int y_pixel;
    int small_x_pixel;
    int small_y_pixel;
    int l;
    int taken;
    int attacked;
    Sprite *missed;
    Sprite *hit;
    Sprite *small_missed;
    Sprite *small_hit;
} Spot;

/**
 * @struct Boat
 * @brief struct that has information about a boat
*/
typedef struct {
    bool fixed;
    bool rotated;
    bool selected;
    bool sunk;
    int id;
    int health;
    int damage;
    int height;
    int width;
    Sprite *sprite;
    Sprite *s_sprite;
    Sprite *v_sprite;
    Sprite *sv_sprite;
    Sprite *small_sprite;
    Sprite *small_v_sprite;
    Spot first;
} Boat; 

/**
 * @struct Player
 * @brief struct that has information about a player
*/
typedef struct {
    Spot matrix[MATRIX_SIZE][MATRIX_SIZE];
    Boat boatA;
    Boat boatB;
    Boat boatC;
    Boat boatD;
    Boat boatE;
} Player;

/**
 * @brief This function is used to save the current matrix in matrix from player 1 struct
*/
void savePlayer1();

/**
 * @brief This function is used to save the current matrix in matrix from player 2 struct
*/
void savePlayer2();

/**
 * @brief This function is used to adjust boats positions from the spots in placing menu and in game
 * 
 * @param boat : boat to adjust position.
*/
void adjustBoard(Boat boat);

/**
 * @brief This function is used to clear the board matrix, so the next player can choose their boats' positions
*/
void clearBoard();

/**
 * @brief This function is used to create the board matrix
 * 
 * @param x : x coordinate.
 * @param y : y coordinate.
 * @param size : size of spot.
*/
void createMatrix(int x, int y, int size);

/**
 * @brief This function is used to increase the damage in the boat hit, so that when the damage is max it sinks
 * 
 * @param taken : id of the boat that was hit.
 * @param p : id of the player.
*/
void updateDamage(int taken, int p);


#endif

