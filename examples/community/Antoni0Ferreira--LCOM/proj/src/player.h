#ifndef __PLAYER_H
#define __PLAYER_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "video.h"

#include "xpm/sship.xpm"
//sprite from https://opengameart.org/content/simple-spaceship

/**
 * @defgroup player player 
 * @{
 * 
 * Functions regarding the initialization and drawing of the player
 */

#define X_INITIAL_PLAYER 365 // Initial X coordinate of the player
#define Y_INITIAL_PLAYER 510 // Initial Y coordinate of the player

/**
 * @brief Struct type for storing and organizing information relative to the player
 * 
 */
struct player {
    uint16_t x, y;
    xpm_image_t img;
    uint8_t *sprite;
};

/**
 * @brief Struct type for storing the various states of the keys used for moving
 * ther player
 * 
 */
struct keyboard_state {
    bool A_DOWN; // The 'a' key is pressed or not
    bool S_DOWN; // The 's' key is pressed or not
    bool W_DOWN; // The 'w' key is pressed or not
    bool D_DOWN; // The 'd' key is pressed or not
};

/**
 * @brief Initializes the player's position, sprite and xpm image
 * 
 * @param player Player of the game
 * @return Return 0 upon success and non-zero otherwise 
 */
int (initialize_player)(struct player *player);

/**
 * @brief Function responsible for drawing the player according to its current position. 
 * It also checks if the user has reached the top of the screen, increasing the points value.
 * 
 * @param player Player of the game
 * @param state Struct state that indicates of keys of the kbd are pressed
 * @param boost_value The number of additional frames the player moves if a boost was captured
 * @param points The current points of the player
 */
void (draw_player)(struct player *player, struct keyboard_state *state, int boost_value, int *points);

/**
 * @}
 */

#endif /* __PLAYER_H */
