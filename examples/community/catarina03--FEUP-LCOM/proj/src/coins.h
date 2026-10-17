#include <lcom/lcf.h>
#include "img_files/coin-0.xpm"
#include "img_files/coin-1.xpm"
#include "img_files/coin-2.xpm"
#include "img_files/coin-3.xpm"
#include "img_files/coin-4.xpm"
#include "img_files/coin-5.xpm"
#include "img_files/coin-6.xpm"
#include "img_files/coin-7.xpm"
#pragma once
#include "game_structs.h"
#include "i8042.h"
#include "game_macros.h"
#include "grafics.h"
#include "collisions.h"


/**
 * @brief Draws all the active coins in the game at the moment.
 * 
 * @param g - Game pointer to a Game struct object that contains all the necessary game information
 */
void drawCoins(Game *g);

/**
 * @brief Creates a new Coin object.
 * 
 * @return Coin pointer to the coin created
 */
Coin * newCoin();

/**
 * @brief Creates a new Coin object and inserts it into the array of coins "coins" of the Game object "g" that's passed in the parameter.
 * 
 * @param g - Game pointer to a Game struct object that contains all the necessary game information
 * @return 0 if succesfull
 */
int createCoin(Game * g);

/**
 * @brief Updates a coin's position on the screen.
 * 
 * @param c - Coin pointer to a Coin struct object that cointains all the necessary coin information
 */
void updateCoin(Coin* c);

/**
 * @brief Draws a certain Coin object.
 * 
 * @param c - Coin pointer to a Coin struct object that cointains all the necessary coin information
 */
void drawCoin(Coin* c);

/**
 * @brief Deletes a certain coin.
 * 
 * @param c - Coin pointer to a Coin struct object that cointains all the necessary coin information
 */
void deleteCoin(Coin* c);

/**
 * @brief Manages the active coins in the coin array.
 *
 * @param g - Game pointer to a Game struct object that contains all the necessary game information
 * @param t - Tux pointer to a Tux struct Object that contain all the necessary Tux information
 */
void updateCoins(Game * g,Tux* t);

