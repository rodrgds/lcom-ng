#ifndef __ANIM_SPRITE_H
#define __ANIM_SPRITE_H

#include "model/sprite/sprite.h"

/** @defgroup anim_sprite anim_sprite
 * @{
 *
 * Functions for creating and managing animated sprites
 */

/**
 * @brief Struct representing an animated sprite
 *
 * Contains frame data, timing information, and animation state
 * for sprites that cycle through multiple frames.
 */
typedef struct {
  Sprite *sp;          /**< @brief Pointer to the current frame sprite */
  Sprite *const *map;  /**< @brief Array of sprite frames for animation */
  uint8_t num_fig;     /**< @brief Total number of frames in the animation */
  uint8_t cur_fig;     /**< @brief Index of the current frame being displayed */
  uint16_t aspeed;     /**< @brief Number of ticks per frame (animation speed) */
  uint16_t cur_aspeed; /**< @brief Current tick counter for frame timing */
  bool loop;           /**< @brief Whether the animation should loop continuously */
} AnimSprite;

/**
 * @brief Creates a new animated sprite
 *
 * Allocates memory for an animated sprite and initializes it with the provided
 * frame array, timing, and loop settings.
 *
 * @param frames Array of sprite pointers representing animation frames
 * @param nframes Number of frames in the animation
 * @param ticks_per_frame Number of game ticks each frame should be displayed
 * @param loop Whether the animation should loop when it reaches the end
 *
 * @return Pointer to the created animated sprite, or NULL if allocation fails
 */
AnimSprite *(create_animSprite) (Sprite *const *frames, uint8_t nframes, uint16_t ticks_per_frame, bool loop);

/**
 * @brief Destroys an animated sprite and frees its resources
 *
 * Frees the memory allocated for the animated sprite structure.
 * Note: Does not free the individual sprite frames, as they may be shared.
 *
 * @param asp Pointer to the animated sprite to be destroyed
 */
void(destroy_animSprite)(AnimSprite *asp);

/**
 * @brief Updates the animated sprite's current frame
 *
 * Advances the animation by one tick. When enough ticks have passed,
 * moves to the next frame. Handles looping and non-looping animations.
 *
 * @param asp Pointer to the animated sprite to update
 */
void(update_animSprite)(AnimSprite *asp);

/**@}*/

#endif /* __ANIM_SPRITE_H */
