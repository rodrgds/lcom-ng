#ifndef _LCOM_SPRITES_H_
#define _LCOM_SPRITES_H_

#include <lcom/lcf.h>

typedef struct {
  uint16_t width, height;
  uint8_t *colors;
} sprite_t;

/**
 * @brief Create a sprite object
 * 
 * @param xpm xpm map of the sprite
 * @return sprite_t* pointer to the sprite object
 */
sprite_t *(create_sprite)(xpm_map_t xpm);

/**
 * @brief Destroy a sprite object
 * 
 * @param sp pointer to the sprite object
 */
void (destroy_sprite)(sprite_t *sp);

/**
 * @brief Draw a sprite object
 * 
 * @param sp pointer to the sprite object
 * @param x X coordinate of the sprite
 * @param y Y coordinate of the sprite
 * @param color_override override the color of the sprite (override[2] = {xpm color, new color}), NULL to use the sprite's color (for now it just changes 1 color, used for fonts)
 * @return int 0 upon success, non-zero otherwise
 */
int (draw_sprite_color_override)(sprite_t *sp, uint16_t x, uint16_t y, uint8_t *color_override);

/**
 * @brief Draw a sprite object
 * @details Calls draw_sprite_color_override with NULL as the color_override
 * 
 * @param sp pointer to the sprite object
 * @param x X coordinate of the sprite
 * @param y Y coordinate of the sprite
 * @return int 0 upon success, non-zero otherwise
 */
int (draw_sprite)(sprite_t *sp, uint16_t x, uint16_t y);

#endif // _LCOM_SPRITES_H_
