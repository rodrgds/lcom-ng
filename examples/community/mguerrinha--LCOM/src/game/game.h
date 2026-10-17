#ifndef _LCOM_GAME_H_
#define _LCOM_GAME_H_

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include "controller/timer/timer.h"
#include "model/model.h"
#include "model/player.h"

/**
 * @brief Function that moves the player in the direction of the aux_dir.
 * @param player_info Struct with the player information.
 * @param aux_dir Direction to move the player.
*/
void (move_player)(PlayerInfo *player_info, enum direction *aux_dir);

/**
 * @brief Function that checks if the player is colliding with himself or with the other player, at each time that the player moves. If a player collides with his own trail, he is eliminated; if he collides with an enemy's trail, the enemy is eliminated.
 * @param player_info Struct with the player information.
*/
void (player_collisions)(PlayerInfo *player_info);

/**
 * @brief Function that fills the territory of the player everytime he gets back to his territory (considering what he achieved when he was away).
 * @param player_info Struct with the player information.
*/
void (fill_territory)(PlayerInfo *player_info);

/**
 * @brief Function that counts the territory of each player.
*/
void (count_territory)();

#endif
