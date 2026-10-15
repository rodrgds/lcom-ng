#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include <stdint.h>
#include <stdbool.h>
#include "sprite.h"

typedef struct AnimatedSprite AnimatedSprite;

/**
 * @brief Creates an animated sprite.
 *
 * @param frames Array of sprite frame pointers.
 * @param frame_count Number of frames in the animation.
 * @param ticks_per_frame Animation speed in ticks per frame.
 * @param loop True if the animation loops, false if it stops at the end.
 */
AnimatedSprite* animated_sprite_create(Sprite *const *frames, uint8_t frame_count, uint8_t ticks_per_frame, bool loop);

/**
 * @brief Frees an animated sprite.
 */
void animated_sprite_destroy(AnimatedSprite *as);

/**
 * @brief Advances the animation by one tick.
 */
void animated_sprite_tick(AnimatedSprite *as);

/**
 * @brief Resets the animation to frame 0.
 */
void animated_sprite_reset(AnimatedSprite *as);

/**
 * @brief Gets the sprite for the current animation frame.
 */
const Sprite* animated_sprite_get_current_frame(const AnimatedSprite *as);

/**
 * @brief Checks whether the animation reached its final frame.
 */
bool animated_sprite_is_done(const AnimatedSprite *as);

#endif // ANIMATED_SPRITE_H
