#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include "model/utils/sprite.h"
#include <stdarg.h>

extern uint16_t x_res; /**< X resolution of the screen */
extern uint16_t y_res; /**< Y resolution of the screen */

/**
 * @brief Represents an Animated Sprite for rendering animated objects on screen.
 */
typedef struct {
    Sprite *sprite;             /**< The base sprite for the animated sprite. */
    uint32_t frames_per_sprite; /**< The number of frames per sprite. */
    uint32_t current_frame;     /**< The current frame being displayed. */
    uint32_t number_sprites;    /**< The total number of sprites in the animation. */
    uint32_t current_sprite;    /**< The current sprite being displayed. */
    uint8_t **maps;             /**< The sprite maps for the animated sprite. */
} AnimatedSprite;

/**
 * @brief Creates a new sprite with the given Sprite and xpm maps.
 *
 * @param sprite Base Sprite.
 * @param frames_per_sprite The number of frames per sprite.
 * @param number_sprites The total number of sprites in the animation.
 * @param ... Variable arguments corresponding to the remaining sprite's xpm maps.
 *
 * @return Pointer to the created AnimatedSprite, or NULL on failure.
 */
AnimatedSprite *create_animated_sprite(Sprite *sprite, uint32_t frames_per_sprite, uint32_t number_sprites, ...);

/**
 * @brief Frees the memory associated with the animated sprite.
 *
 * @param anim_sprite Pointer to the AnimatedSprite structure to destroy.
 * @return 0 for success, 1 for failure.
 */
int destroy_animated_sprite(AnimatedSprite *anim_sprite);

/**
 * @brief Draws the AnimatedSprite on the given frame buffer.
 *
 * @param anim_sprite Pointer to the AnimatedSprite structure to draw.
 * @param frame_buffer Pointer to the frame buffer to draw the sprite on.
 *
 * @return 0 on success, 1 on failure.
 */
int draw_animated_sprite(AnimatedSprite *anim_sprite, uint8_t *frame_buffer);

#endif
