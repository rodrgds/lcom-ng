#pragma once
#include <lcom/lcf.h>

/**
 * @brief Structure representing a sprite.
 */
typedef struct {
    uint16_t height; /**< The height of the sprite. */
    uint16_t width; /**< The width of the sprite. */
    uint32_t *colors; /**< Pointer to the array of colors representing the sprite. */
    uint32_t color; /**< The color of the sprite. */
} Sprite;

/**
 * @brief Creates a sprite from an XPM map.
 *
 * @param sprite The XPM map representing the sprite.
 * @return A pointer to the created sprite, or NULL on failure.
 */
Sprite *create_sprite_xpm(xpm_map_t sprite);

/**
 * @brief Destroys a sprite, freeing its memory.
 *
 * @param sprite The sprite to destroy.
 */
void destroy_sprite(Sprite *sprite);

