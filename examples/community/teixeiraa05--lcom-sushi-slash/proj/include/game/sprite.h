#ifndef __SPRITE_H
#define __SPRITE_H

/**
 * @file sprite.h
 * @brief Declares sprite loading and frame animation helpers.
 */

/** @brief Slow animation delay, in timer ticks between frames. */
#define ANIM_SLOW_DELAY 20

/** @brief Normal animation delay, in timer ticks between frames. */
#define ANIM_NORMAL_DELAY 10

/** @brief Fast animation delay, in timer ticks between frames. */
#define ANIM_FAST_DELAY 5

#include <lcom/lcf.h>

/**
 * @brief Loaded XPM sprite.
 */
typedef struct {
  /** @brief Pixel buffer loaded from the XPM map. */
  uint8_t *pixmap;

  /** @brief Sprite width in pixels. */
  uint16_t width;

  /** @brief Sprite height in pixels. */
  uint16_t height;
} sprite_t;

/**
 * @brief Animation made from an ordered array of sprite frames.
 */
typedef struct {
  /** @brief Array of frame sprites. */
  sprite_t **sprites;

  /** @brief Number of frame sprites in sprites. */
  size_t num_sprites;

  /** @brief Index of the frame currently displayed. */
  uint8_t cur_frame;

  /** @brief Tick at which the animation last advanced. */
  int last_tick;

  /** @brief Number of ticks to wait before advancing frames. */
  int frame_delay;
} anim_sprite_t;

/**
 * @brief Load an XPM map into a sprite.
 *
 * @param sprite Sprite object to fill.
 * @param xpm XPM map to load.
 * @return 0 on success, non-zero on failure.
 */
int sprite_load(sprite_t *sprite, xpm_map_t xpm);

/**
 * @brief Release the pixel buffer owned by a sprite.
 *
 * @param sprite Sprite to destroy.
 */
void sprite_destroy(sprite_t *sprite);

/**
 * @brief Load multiple XPM maps into sprites and initialize an animation.
 *
 * @param anim Animation object to initialize.
 * @param sprites Array of allocated sprite pointers, one for each frame.
 * @param count Number of frames to load.
 * @param xpms XPM maps used as animation frames.
 * @param frame_delay Ticks between frame changes.
 * @return 0 on success, non-zero on failure.
 */
int anim_sprite_load(anim_sprite_t *anim, sprite_t *sprites[], size_t count,
                     xpm_map_t xpms[], int frame_delay);

#endif /* __SPRITE_H */
