#ifndef __SUSHI_H
#define __SUSHI_H

#include <stdbool.h>
#include <stdlib.h>
#include "../macros/global.h"
#include "../macros/physics.h"
#include "sprite.h"

/**
 * @file sushi.h
 * @brief Declares gameplay items that use sushi-style physics and cutting.
 *
 * Regular sushi pieces award score when cut and cost a life when missed.
 * Bombs share the same motion and hit detection model, but are hazards handled
 * by gameplay logic.
 */

/** @brief Vertical spawn offset below the bottom edge of the screen, in pixels. */
#define SUSHI_SPAWN_Y_OFFSET 30

/** @brief Extra bottom margin after which a falling sushi piece is considered missed. */
#define SUSHI_MISS_Y_OFFSET 80

/** @brief Default score awarded for cutting a regular sushi piece. */
#define SUSHI_SCORE_VALUE 1

/**
 * @brief Runtime kind of a sushi-style gameplay item.
 */
typedef enum {
  /** @brief Regular sushi that should be cut for score. */
  SUSHI_REGULAR,

  /** @brief Bomb hazard that should be avoided. */
  SUSHI_BOMB,

  /** @brief Boss encounter. */
  SUSHI_BOSS
} sushi_kind_t;

/**
 * @brief Moving gameplay item launched across the screen.
 *
 * Positions and velocities are stored in the fixed-point representation defined
 * by macros/physics.h. The sprite pointer is owned by the sushi object and is
 * released by sushi_destroy().
 */
typedef struct {
  /** @brief Fixed-point horizontal position on the screen. */
  int x;

  /** @brief Fixed-point vertical position on the screen. */
  int y;

  /** @brief Fixed-point horizontal velocity. */
  int vx;

  /** @brief Fixed-point vertical velocity. */
  int vy;

  /** @brief Fixed-point gravity applied every tick. */
  int gravity;

  /** @brief Clockwise rotation in degrees, normalized to [0, 360). */
  int angle;

  /** @brief Clockwise degrees added on each physics tick. */
  int angular_velocity;

  /** @brief Score awarded when the item is successfully cut. */
  int score_value;

  /** @brief Index of the regular sushi XPM used for fragment selection. */
  uint8_t type;

  /** @brief Gameplay kind, such as regular sushi or bomb. */
  sushi_kind_t kind;

  /** @brief Owned sprite representing the item. */
  sprite_t* xpm;
} sushi_t;

/**
 * @brief Create a sushi item with explicit position, motion, sprite, and score.
 *
 * @param x Initial horizontal position in pixels.
 * @param y Initial vertical position in pixels.
 * @param vx Initial horizontal velocity in pixels per tick.
 * @param vy Initial vertical velocity in pixels per tick.
 * @param gravity Fixed-point gravity applied every tick.
 * @param xpm Owned sprite used to draw the sushi.
 * @param score_value Score awarded when the sushi is cut.
 *
 * @return Initialized regular sushi item.
 */
sushi_t create_sushi(int x, int y, int vx, int vy, int gravity, sprite_t* xpm, int score_value);

/**
 * @brief Create a bomb item with explicit position, motion, and sprite.
 *
 * @param x Initial horizontal position in pixels.
 * @param y Initial vertical position in pixels.
 * @param vx Initial horizontal velocity in pixels per tick.
 * @param vy Initial vertical velocity in pixels per tick.
 * @param gravity Fixed-point gravity applied every tick.
 * @param xpm Owned sprite used to draw the bomb.
 *
 * @return Initialized bomb item.
 */
sushi_t create_bomb(int x, int y, int vx, int vy, int gravity, sprite_t* xpm);

/**
 * @brief Create a boss sushi item with explicit position, motion, sprite, and score.
 *
 * @param x Initial horizontal position in pixels.
 * @param y Initial vertical position in pixels.
 * @param vx Initial horizontal velocity in pixels per tick.
 * @param vy Initial vertical velocity in pixels per tick.
 * @param gravity Fixed-point gravity applied every tick.
 * @param xpm Owned sprite used to draw the boss sushi.
 * @param score_value Score awarded when the boss sushi is defeated.
 *
 * @return Initialized boss sushi item.
 */
sushi_t create_sushi_boss(int x, int y, int vx, int vy, int gravity, sprite_t* xpm, int score_value);

/**
 * @brief Create a random regular sushi piece using the current difficulty motion.
 *
 * @param speed Launch speed in pixels per tick.
 * @param gravity Fixed-point gravity applied every tick.
 * @param prng_seed Pointer to the PRNG seed state.
 *
 * @return Initialized regular sushi item, or an item with a NULL sprite on failure.
 */
sushi_t create_random_sushi(int speed, int gravity, uint32_t *prng_seed);

/**
 * @brief Create a random bomb using the current difficulty motion.
 *
 * @param speed Launch speed in pixels per tick.
 * @param gravity Fixed-point gravity applied every tick.
 * @param prng_seed Pointer to the PRNG seed state.
 *
 * @return Initialized bomb item, or an item with a NULL sprite on failure.
 */
sushi_t create_random_bomb(int speed, int gravity, uint32_t *prng_seed);

/**
 * @brief Advance a sushi-style item by one physics tick.
 *
 * @param sushi Item to update.
 */
void update_sushi(sushi_t* sushi);

/**
 * @brief Get the item's horizontal screen position in pixels.
 *
 * @param sushi Item to inspect.
 *
 * @return Horizontal position in pixels, or 0 if sushi is NULL.
 */
int sushi_screen_x(const sushi_t* sushi);

/**
 * @brief Get the item's vertical screen position in pixels.
 *
 * @param sushi Item to inspect.
 *
 * @return Vertical position in pixels, or 0 if sushi is NULL.
 */
int sushi_screen_y(const sushi_t* sushi);

/**
 * @brief Destroy the sprite owned by a sushi-style item.
 *
 * @param sushi Item whose sprite should be released.
 */
void sushi_destroy(sushi_t* sushi);

/**
 * @brief Moving fragment created after a regular sushi piece is cut.
 */
typedef struct {
  /** @brief Fixed-point horizontal position on the screen. */
  int x;

  /** @brief Fixed-point vertical position on the screen. */
  int y;

  /** @brief Fixed-point horizontal velocity. */
  int vx;

  /** @brief Fixed-point vertical velocity. */
  int vy;

  /** @brief Fixed-point gravity applied every tick. */
  int gravity;

  /** @brief Clockwise rotation in degrees, normalized to [0, 360). */
  int angle;

  /** @brief Clockwise degrees added on each physics tick. */
  int angular_velocity;

  /** @brief Owned sprite representing the fragment. */
  sprite_t* xpm;
} sushi_fragment_t;

/**
 * @brief Test visible rotated pixels for sushi, and padded bounds for hazards.
 *
 * @param sushi Item to test.
 * @param cut_x0 Slash start horizontal position in pixels.
 * @param cut_y0 Slash start vertical position in pixels.
 * @param cut_x1 Slash end horizontal position in pixels.
 * @param cut_y1 Slash end vertical position in pixels.
 * @param radius Padding around hazard bounds; ignored for regular sushi.
 *
 * @return true if the segment hits the item, false otherwise.
 */
bool sushi_segment_intersects(const sushi_t* sushi, int cut_x0, int cut_y0, int cut_x1, int cut_y1, int radius);

/**
 * @brief Select a generated profile from the slash and split a regular sushi.
 *
 * @param sushi Item to split and destroy.
 * @param cut_x0 Slash start horizontal position in pixels.
 * @param cut_y0 Slash start vertical position in pixels.
 * @param cut_x1 Slash end horizontal position in pixels.
 * @param cut_y1 Slash end vertical position in pixels.
 * @param left_fragment Output fragment on the negative side of the profile normal.
 * @param right_fragment Output fragment on the positive side of the profile normal.
 *
 * @return 0 on success; non-zero leaves the sushi and outputs unchanged.
 */
int cut_sushi(sushi_t* sushi, int cut_x0, int cut_y0, int cut_x1, int cut_y1,
               sushi_fragment_t* left_fragment, sushi_fragment_t* right_fragment);

/**
 * @brief Advance a sushi fragment by one physics tick.
 *
 * @param fragment Fragment to update.
 */
void update_sushi_fragment(sushi_fragment_t* fragment);

/**
 * @brief Get the fragment's horizontal screen position in pixels.
 *
 * @param fragment Fragment to inspect.
 *
 * @return Horizontal position in pixels, or 0 if fragment is NULL.
 */
int sushi_fragment_screen_x(const sushi_fragment_t* fragment);

/**
 * @brief Get the fragment's vertical screen position in pixels.
 *
 * @param fragment Fragment to inspect.
 *
 * @return Vertical position in pixels, or 0 if fragment is NULL.
 */
int sushi_fragment_screen_y(const sushi_fragment_t* fragment);

/**
 * @brief Destroy the sprite owned by a sushi fragment.
 *
 * @param fragment Fragment whose sprite should be released.
 */
void sushi_fragment_destroy(sushi_fragment_t* fragment);

#endif /* __SUSHI_H */
