/**
 * @file game.h
 * @author Bruno Pinto and Vitor Magalhaes
 * @brief Contains functions related to the operation of the game
 */

#ifndef __GAME_H
#define __GAME_H

/** @defgroup game game
 * @{
 *
 * Functions related to the operation of the game
 */

/**
 * @brief Makes game initialization
 */
void init();

/**
 * @brief Says if there's an enemy going under the ship
 *
 * @return boolean indicating true or false
 */
int is_ship();

/**
 * @brief Says if there are still enemies
 *
 * @return boolean indicating true or false
 */
int no_enemies();

/**
 * @brief Creates the pong and switches game state
 */
void fire_pong();

/**
 * @brief Makes enemies down movement
 *
 * @return boolean indicating if the game was lost
 */
int enemy_movedown();

/**
 * @brief Makes a game round, where updating action on screen
 */
void round();

/**
 * @brief Receives drivers interruptions
 */
void start_game();

/**
 * @brief Destroys the enemy hit by the pong and desactivates it
 */
void is_enemy();

/** @} end of game */

#endif /* __GAME_H*/
