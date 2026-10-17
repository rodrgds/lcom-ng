/**
 * @file sprites.h
 * @brief Header file for sprite related functions and structures.
 */

#ifndef _LCOM_GAME_SPRITES_H_
#define _LCOM_GAME_SPRITES_H_

#include <lcom/lcf.h>
#include "../game/game_utils.h"

/**
 * Enumeration defining different sprite identifiers.
 */
typedef enum {
  TEXT,
  PLAYER,
  TILE,
  TARGET_LOW,
  TARGET_MID,
  TARGET_HIGH,
  // add more identifiers as needed
} sprite_id_e;

/**
 * Structure representing a pixmap (a rectangular image composed of pixels).
 */
typedef struct {
  uint16_t width, height;         // Width and height of the pixmap
  uint32_t *color_array;          // Array containing pixel colors
} pixmap_t;

/**
 * Structure representing a sprite.
 */
typedef struct {
  sprite_id_e identifier;         // Identifier of the sprite
  position_t position;            // Position of the sprite
  uint8_t n_frames;               // Number of frames of the sprite
  uint8_t i_frame;                // Index of the current active frame
  pixmap_t* frames;               // Array of frames of the sprite
} sprite_t;

/**
 * Structure representing a target
 */
typedef struct {
  sprite_t sprite;                // Sprite structure
  unsigned int remaining_time;    // Remaining time
  bool active;                    // Active status
  bool popped;                    // Popped status
} target_t;

/**
 * Structure representing a not-pseudo sprite.
 */
typedef struct {
    uint16_t height, width;       // Height and width of the sprite
    uint32_t *colors;             // Array of colors
} not_pseudo_sprite; 

/**
 * Creates a sprite from an array of XPM maps.
 * @param xpms Array of XPM maps representing the frames of the sprite.
 * @param pos The initial position of the sprite.
 * @param n The number of frames in the sprite.
 * @param id The identifier of the sprite.
 * @return A pointer to the created sprite or NULL if memory allocation fails.
 */
sprite_t *create_sprite(xpm_map_t xpms[], position_t pos, int n, sprite_id_e id);

/**
 * Creates a not-pseudo sprite from an XPM map.
 * @param sprite The XPM map representing the sprite.
 * @return A pointer to the created not-pseudo sprite or NULL if memory allocation fails.
 */
not_pseudo_sprite *(create_not_pseudo_sprite)(xpm_map_t sprite);

/**
 * Deletes a not-pseudo sprite.
 * @param sprite Pointer to the not-pseudo sprite to be deleted.
 */
void delete_not_pseudo_sprite(not_pseudo_sprite *sprite);

/**
 * Deletes a sprite.
 * @param spr Pointer to the sprite to be deleted.
 */
void delete_sprite(sprite_t *spr);

/**
 * Retrieves the width of an XPM character.
 * @param letter The character for which to retrieve the width.
 * @return The width of the XPM character, or -1 if the character is not found.
 */
int get_xpm_width_from_char(char letter);

/**
 * Retrieves the XPM map corresponding to a character.
 * @param letter The character for which to retrieve the XPM map.
 * @return The XPM map corresponding to the character, or NULL if the character is not found.
 */
xpm_map_t get_xpm_from_char(char letter);

#endif
