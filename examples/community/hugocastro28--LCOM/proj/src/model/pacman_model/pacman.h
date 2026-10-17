#ifndef PACMAN_MODEL_H
#define PACMAN_MODEL_H

#include "../position.h"

/**
 * @brief Game element whose movement is manipulated by the player.
 * 
 */
typedef struct {
    Position pos;
} Pacman;

/**
 * @brief Set the pacman initial position.
 * 
 */
void (set_pacman_position)();

#endif
