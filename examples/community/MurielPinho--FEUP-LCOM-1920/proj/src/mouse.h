#include <lcom/lcf.h>

#ifndef MOUSE_H
#define MOUSE_H

/** @defgroup Mouse Mouse
 * @{
 * Functions to control the mouse object
 */

/**
 * @brief Struct type for mouse object
 */
typedef struct
{
    int lb_status; /**< @brief int to represent the left button status */
    Sprite *sprite; /**< @brief Sprite to represent the mouse sprite */
} Mouse;

/**
 * @brief Function to create the mouse object
 * @param x uint16_t for the initial x coordinate of the mouse 
 * @param y uint16_t for the initial y coordinate of the mouse 
 * @param mouse_sprite Sprite* to the sprite of the mouse 
 * @return Returns the created mouse object
 */
Mouse *create_mouse(uint16_t x, uint16_t y, Sprite *mouse_sprite);
/**
 * @brief Function to delete a mouse object
 * @param mouse Mouse * for the mouse object 
 */
void delete_mouse(Mouse *mouse);
/**
 * @brief Function to draw a mouse object
 * @param mouse Mouse * for the mouse object 
 */
void draw_mouse(Mouse *mouse);
/**
 * @brief Function to set a mouse object's x coordinate
 * @param x uint16_t for the x coordinate to be set for the mouse 
 * @param mouse Mouse * for the mouse object 
 */
void setMouseX(Mouse *mouse, uint16_t x);
/**
 * @brief Function to set a mouse object's y coordinate
 * @param y uint16_t for the y coordinate to be set for the mouse 
 * @param mouse Mouse * for the mouse object 
 */
void setMouseY(Mouse *mouse, uint16_t y);
/**
 * @}
 */
#endif /*MOUSE_H*/
