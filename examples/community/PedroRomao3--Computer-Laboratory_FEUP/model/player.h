#ifndef __PLAYER_H
#define __PLAYER_H

#include <stdio.h>
#include <lcom/lcf.h>
#include "settings.h"


typedef struct {
    char name[MAX_LENGTH];
    int money;
    int c1;
    int c2;
    int c3;
    int a1;
    int a2;
    int a3;
    int seconds;
} Player;

typedef struct {
    int coin5;
    int coin25;
    int coin10;
    int coin100;
} GameCoins;


/**
 @brief Calculates the total bet amount.
This function calculates the total bet amount based on the number of coins of each denomination.
The denominations are 5, 25, 10, and 100, and the number of each denomination is stored in the g_coins structure.
 @return The total bet amount.
*/
int get_bet_amount();


/**
 @brief Gets the coin index based on the coordinates.
This function determines the index of the coin based on the given coordinates (x, y) on the screen.
It checks the specified regions on the screen and returns the corresponding index for the coin.
If the coordinates do not fall within any valid region, it returns -1.
 @param x The x-coordinate.
 @param y The y-coordinate.
 @return The coin index if the coordinates match a valid region, -1 otherwise.
*/
int get_coin_index(int x, int y);


/**
 @brief Takes coins from a player's money based on the given coin type. 
The function will check if the player has enough money to cover the coin type.
If yes, it will decrease the player's money by the coin's value and increase the
respective global coin count. The coin types are identified by the following id: 
 - 4: Coin with value of 5.
 - 5: Coin with value of 25.
 - 6: Coin with value of 10.
 - 7: Coin with value of 100. 
If the player doesn't have enough money or if the id is not recognized, the function
will not perform any operations.
 @note This function manipulates the global objects: g_coins and p1. Make sure these
objects are properly initialized and usable.
 @param id The id of the coin type to be taken.
*/
void take_coin(int id);

/**
 @brief Adds a coin to the player's car.
This function generates a random number and assigns it to an available car slot of the player.
 @param carnum Pointer to an integer variable to store the car number where the coin was added.
 @return The number of coins successfully added (0 or 1).
*/
int add_coin(int *carnum);

/**
 @brief Ends the game and resets all game-related variables to their initial state.
This function resets the game by performing the following operations:
 - Sets the player's `seconds` to 100.
 - Resets the coin slots of player `p1` and computer player `pc` to 0.
 - Resets the counts of all types of coins in the global coin store `g_coins` to 0.
 @note This function manipulates the global objects `p1`, `pc`, and `g_coins`. Make sure 
these objects are properly initialized and usable. 
The function does not return a value.
*/
void end_game();

#endif
