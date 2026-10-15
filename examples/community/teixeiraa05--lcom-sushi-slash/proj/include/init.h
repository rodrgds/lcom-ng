#ifndef __INIT_H
#define __INIT_H

#include "game/sprite.h"
#include "font.h"

/**
 * @file init.h
 * @brief Declares shared asset loading and cleanup helpers.
 */

/** @brief Number of frames in the idle mascot animation. */
#define MASCOT_IDLE_FRAME_COUNT 4

/** @brief Number of frames in the losing mascot animation. */
#define MASCOT_LOSE_FRAME_COUNT 15

/**
 * @brief Collection of sprites, fonts, and animations shared by game states.
 */
typedef struct {
  /** @brief Full-screen gameplay/menu background. */
  sprite_t background_sprite;

  /** @brief Title/name sprite shown on menus. */
  sprite_t name_sprite;

  /** @brief Bitmap font used by UI and HUD drawing. */
  Font game_font;

  /** @brief Mouse cursor sprite. */
  sprite_t mouse_cursor;

  /** @brief Life indicator sprite. */
  sprite_t heart;

  /** @brief Bomb hazard sprite. */
  sprite_t bomb;

  /** @brief Allocated frame sprites for the idle mascot animation. */
  sprite_t *mascote[MASCOT_IDLE_FRAME_COUNT];

  /** @brief Runtime controller for the idle mascot animation. */
  anim_sprite_t mascot_anim;

  /** @brief Allocated frame sprites for the losing mascot animation. */
  sprite_t *mascote_lose[MASCOT_LOSE_FRAME_COUNT];

  /** @brief Runtime controller for the losing mascot animation. */
  anim_sprite_t mascot_lose_anim;
} Assets;

/**
 * @brief Load all shared game assets.
 *
 * @param assets Asset collection to initialize.
 * @return 0 on success, non-zero on failure.
 */
int assets_init(Assets *assets);

/**
 * @brief Destroy all assets owned by an asset collection.
 *
 * @param assets Asset collection to destroy.
 */
void assets_destroy(Assets *assets);

#endif /* __INIT_H */
