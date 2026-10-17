#ifndef SPRITES_H
#define SPRITES_H

#include <lcom/lcf.h>

/**
 * @brief Struct representing a sprite in the game.
 */
typedef struct {
    xpm_image_t info; /**< Information about the XPM image of the sprite */
    uint16_t x; /**< X-coordinate of the sprite */
    uint16_t y; /**< Y-coordinate of the sprite */
    int16_t speed; /**< Speed of the sprite */
    int16_t angle; /**< Angle of movement of the sprite */
} sprite;

/**
 * @brief Struct representing the mouse sprite, which can change its appearance.
 */
typedef struct {
    xpm_image_t info[2]; /**< Array of XPM images for the mouse sprite */
    uint8_t index; /**< Current index of the displayed image */
    uint16_t x; /**< X-coordinate of the mouse sprite */
    uint16_t y; /**< Y-coordinate of the mouse sprite */
    int16_t speed; /**< Speed of the mouse sprite */
} mouse_sprite;

/**
 * @brief Loads a sprite with the specified properties and XPM image.
 *
 * @param x X-coordinate of the sprite.
 * @param y Y-coordinate of the sprite.
 * @param speed Speed of the sprite.
 * @param angle Angle of movement of the sprite.
 * @param sprite Pointer to the sprite to be initialized.
 * @param xpm The XPM image map for the sprite.
 */
void (load_sprite) (uint16_t x, uint16_t y, uint16_t speed, uint16_t angle, sprite* sprite, xpm_map_t xpm);

#endif
