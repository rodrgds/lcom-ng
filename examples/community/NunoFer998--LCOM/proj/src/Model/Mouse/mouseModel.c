/**
 * @file mouseModel.c
 * @brief Mouse model implementation
 * 
 * This file contains the implementation of mouse-related functions
 * for creating, managing, and destroying mouse objects.
 */

#include "mouseModel.h"

/**
 * @brief Creates a new mouse object at specified coordinates
 * 
 * This function initializes a new mouse with a sprite and sets
 * its initial position to the provided coordinates.
 * 
 * @param x Initial X coordinate for the mouse
 * @param y Initial Y coordinate for the mouse
 * @return Mouse object with initialized position and sprite
 */
Mouse createMouse(uint16_t x, uint16_t y) {
    Mouse m;
    m.sprite = generate_Sprite((xpm_map_t) MouseXPM);  // Load mouse sprite from XPM
    m.pos.x = x;                                        // Set initial X position
    m.pos.y = y;                                        // Set initial Y position
    return m;
}

/**
 * @brief Destroys a mouse object and frees its memory
 * 
 * This function cleans up the mouse object by freeing
 * the memory allocated for its sprite.
 * 
 * @param m Mouse object to be destroyed
 */
void destroyMouse(Mouse m) {
    freeSprite(m.sprite);  // Free the sprite memory
}
