#ifndef __ANIM_SPRITE_H_
#define __ANIM_SPRITE_H_

#include <lcom/lcf.h>
#include "video/video.h"
#include "sprite.h"

/**
 * @brief Structure representing an animated sprite
 */
typedef struct AnimSprite {
  Sprite *map[5]; /**< Map of sprites */
  uint32_t num_sprites; /**< Number of sprites */
  uint32_t curr_sprite; /**< Index of the current sprite */
  int x, y; /**< Current position */
  int xspeed, yspeed; /**< Current speed */
  float xScale, yScale; /**< Current scale */
} AnimSprite;

/**
 * @brief Global variables representing different animated sprites
 */
AnimSprite *coin_1;
AnimSprite *coin_2;
AnimSprite *coin_shadow_1;
AnimSprite *coin_shadow_2;
AnimSprite *dealer_card_1;
AnimSprite *dealer_card_2;
AnimSprite *dealer_card_3;
AnimSprite *player_card_1;
AnimSprite *player_card_2;
AnimSprite *player_card_3;

/**
 * @brief Creates an animated sprite
 * @param sprites Array of sprite pointers
 * @param x Initial x coordinate
 * @param y Initial y coordinate
 * @param xspeed Initial horizontal speed
 * @param yspeed Initial vertical speed
 * @return Pointer to the created animated sprite
 */
AnimSprite *(create_animSprite)(Sprite *sprites[], int x, int y, int xspeed, int yspeed);

/**
 * @brief Destroys an animated sprite
 * @param animsp Pointer to the animated sprite to destroy
 */
void (destroy_animSprite)(AnimSprite *animsp);

/**
 * @brief Flips an animated sprite horizontally
 * @param animsp Pointer to the animated sprite to flip
 * @param first Index of the first sprite to flip
 * @param last Index of the last sprite to flip
 * @param speed Speed of the flip animation
 */
void (animSprite_horizontal_flip)(AnimSprite *animsp, uint32_t first, uint32_t last, double speed);

/**
 * @brief Sets the position of an animated sprite
 * @param animsp Pointer to the animated sprite
 * @param x New x coordinate
 * @param y New y coordinate
 */
void (set_animSprite_position)(AnimSprite *animsp, int x, int y);

/**
 * @brief Sets the speed of an animated sprite
 * @param animsp Pointer to the animated sprite
 * @param xspeed New horizontal speed
 * @param yspeed New vertical speed
 */
void (set_animSprite_speed)(AnimSprite *animsp, int xspeed, int yspeed);

/**
 * @brief Draws an animated sprite
 * @param animsp Pointer to the animated sprite
 * @param i Index of the sprite to draw
 */
void (draw_animSprite)(AnimSprite *animsp, uint32_t i);

/**
 * @brief Moves an animated sprite to a new position
 * @param animSp Pointer to the animated sprite
 * @param destx Destination x coordinate
 * @param desty Destination y coordinate
 */
void (move_animSprite)(AnimSprite *animSp, int destx, int desty);

/**
 * @brief Initializes global animated sprite variables
 */
void (create_animSprites)();

/**
 * @brief Destroys global animated sprite variables
 */
void (destroy_animSprites)();

#endif /* __ANIM_SPRITE_H_ */
