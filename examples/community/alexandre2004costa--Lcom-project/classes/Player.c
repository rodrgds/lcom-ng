#include "Player.h"

/** @defgroup Player Player
 * @{
 */

/**
 * @brief Handles the player's movement.
 * @param player Player is nothing except a different name for a Object object. 
 * @details As the name shows, it represents a player.
 */
void (playerMovement)(Player *player){
    if (player->sprite->y + player->sprite->height/2 < 700) player->yspeed ++;
    if (player->sprite->y + player->sprite->height/2 >= 700 && player->yspeed >= 0) player->yspeed = 0;
    player->sprite->y += player->yspeed;
    if ((player->sprite->x < 0 && player->xspeed < 0) || (player->sprite->x + player->sprite->width > WindowWidth && player->xspeed > 0)) return;
    player->sprite->x += player->xspeed;   
}

/**@}*/
