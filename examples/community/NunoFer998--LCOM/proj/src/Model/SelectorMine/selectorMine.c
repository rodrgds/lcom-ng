/**
 * @file selectorMine.c
 * @brief Selector mine model implementation
 * 
 * This file contains the implementation of selector mine functions
 * for creating, managing, and positioning the mine selector sprite
 * used in menu navigation.
 */

#include "selectorMine.h"

/**
 * @brief Creates a new selector mine object
 * 
 * This function initializes a new selector mine with its sprite
 * and sets it to the default initial position.
 * 
 * @return SMine object with initialized sprite and default position
 */
SMine createSMine() {
    SMine m;
    m.sprite = generate_Sprite((xpm_map_t) BigMine);  // Load mine sprite from XPM
    m.pos.x = INIT_POS_X;                             // Set initial X position
    m.pos.y = INIT_POS_Y;                             // Set initial Y position
    return m;
}

/**
 * @brief Sets the position of the selector mine
 * 
 * This function updates the position of the selector mine to
 * the specified coordinates, used for menu navigation.
 * 
 * @param mine Pointer to the selector mine
 * @param x New X coordinate
 * @param y New Y coordinate
 */
void setSMinePosition(SMine* mine, uint16_t x, uint16_t y) {
   mine->pos.x = x;  // Update X position
   mine->pos.y = y;  // Update Y position
}

/**
 * @brief Destroys a selector mine object and frees its memory
 * 
 * This function cleans up the selector mine by freeing
 * the memory allocated for its sprite.
 * 
 * @param mine Pointer to the selector mine to be destroyed
 */
void destroySMine( SMine* mine) {
    free(mine->sprite);  // Free the sprite memory
}
