#include <lcom/lcf.h>
#pragma once
#include "game_macros.h"
#include "game_structs.h"
#include "grafics.h"
#include "jogo_funcs.h"
#include "collisions.h"
#include "img_files/spiky-0.xpm"
#include "img_files/spiky-1.xpm"
#include "img_files/spiky-2.xpm"
#include "img_files/spiky-dead-2.xpm"
#include "img_files/spiky-dead-1.xpm"
#include "img_files/spiky-dead-0.xpm"
#include "img_files/zeekling-dead-0.xpm"
#include "img_files/zeekling-dead-1.xpm"
#include "img_files/zeekling-dead-2.xpm"
#include "img_files/zeekling-dead-3.xpm"
#include "img_files/zeekling-dead-4.xpm"
#include "img_files/zeekling-dead-5.xpm"
#include "img_files/zeekling-0.xpm"
#include "img_files/zeekling-1.xpm"
#include "img_files/zeekling-2.xpm"
#include "img_files/zeekling-diving.xpm"
#include "img_files/spike-broken.xpm"
#include "img_files/spike-falling.xpm"
#include "img_files/kamikaze-left.xpm"
#include "img_files/kamikaze-melting-1.xpm"
#include "img_files/kamikaze-melting-2.xpm"

/**
 * @brief Draws all the active enemies in the game at the moment.
 * 
 * @param g - Game pointer to a Game struct object that contains all the necessary game information
 */
void drawEnemies(Game *g);

/**
 * @brief Creates a new Enemy object.
 * 
 * @param type - 
 * @return Enemy* - Enemy pointer to the enemy created
 */
Enemy * newEnemy(int type);

/**
 * @brief Creates a new Enemy object and inserts it into the array of enemies "enemies" of the Game object "g" that's passed in the parameter.
 * 
 * @param g - Game pointer to a Game struct object that contains all the necessary game information
 * @param type - Type of enemy to be created
 * @return int - 0 if successful
 */
int createEnemy(Game * g,int type);

/**
 * @brief Updates the enemy state, for example, whether it's dead or alive or if it's collinding with something.
 * 
 * @param e - Enemy pointer to a Enemy struct object that contains all the necessary enemy information
 * @param t - Tux pointer to a Tux struct object that contains all the necessary tux information
 * @param g - Game pointer to a Game struct object that contains all the necessary game information
 * @return int 
 */
int updateEnemy(Enemy* e,Tux *t,Game *g);

/**
 * @brief Draws a certain Enemy object.
 * 
 * @param e - Enemy pointer to a Enemy struct object that contains all the necessary enemy information
 */
void drawEnemy(Enemy* e);

/**
 * @brief Deletes a certain enemy.
 * 
 * @param e - Enemy pointer to a Enemy struct object that contains all the necessary enemy information
 */
void deleteEnemy(Enemy* e);

/**
 * @brief Manages the active enemies in the enimy array.
 * 
 * @param g - Game pointer to a Game struct object that contains all the necessary game information
 * @param t - Tux pointer to a Tux struct object that contains all the necessary tux information
 */
void updateEnemies(Game * g,Tux* t);

