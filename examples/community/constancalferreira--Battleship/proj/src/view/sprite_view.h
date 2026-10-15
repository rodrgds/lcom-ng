/**
 * @file sprite_view.h
 * @brief Functions for rendering sprites to the screen
 * 
 * This file contains declarations for sprite drawing functions that
 * transform sprite data into visible graphics on the screen. Includes
 * standard drawing as well as visual effects like highlighting and recoloring.
 */

#ifndef SPRITE_VIEW_H
#define SPRITE_VIEW_H


#include <lcom/lcf.h>
#include <lcom/xpm.h>
#include <minix/sysutil.h>

#include "controller/graphics.h"
#include "model/sprite.h"
#include "model/game_macro.h"




/**
 * @brief Draws a sprite at the specified position
 * 
 * Renders a sprite to the buffer at the given coordinates. Transparent pixels
 * in the sprite (defined by XPM transparency color) are not drawn.
 *
 * @param sprite Pointer to the sprite to be drawn
 * @param x X-coordinate where the sprite will be positioned
 * @param y Y-coordinate where the sprite will be positioned
 * @param buffer Pointer to the graphics buffer where the sprite will be drawn
 * @return 0 on success, 1 on failure
 */
int(draw_sprite)(Sprite *sprite, uint16_t x, uint16_t y, uint8_t *buffer);

/**
 * @brief Draws a sprite with a brightness boost effect
 * 
 * Similar to draw_sprite, but increases the RGB values of each pixel
 * to create a highlighted appearance. Used for hover/selection effects.
 *
 * @param sprite Pointer to the sprite to be drawn
 * @param x X-coordinate where the sprite will be positioned
 * @param y Y-coordinate where the sprite will be positioned
 * @param buffer Pointer to the graphics buffer where the sprite will be drawn
 * @return 0 on success, 1 on failure
 */
int(draw_sprite_highlighted)(Sprite *sprite, uint16_t x, uint16_t y, uint8_t *buffer);

/**
 * @brief Draws a sprite with a custom color
 * 
 * Renders a sprite's shape but replaces all non-transparent pixels
 * with the specified color. Used for creating color variations of
 * the same sprite.
 *
 * @param sprite Pointer to the sprite to be drawn
 * @param x X-coordinate where the sprite will be positioned
 * @param y Y-coordinate where the sprite will be positioned
 * @param recolor The color to use for all non-transparent pixels
 * @param buffer Pointer to the graphics buffer where the sprite will be drawn
 * @return 0 on success, 1 on failure
 */
int draw_sprite_recolor(Sprite *sprite, uint16_t x, uint16_t y, uint32_t recolor, uint8_t *buffer);





#endif
