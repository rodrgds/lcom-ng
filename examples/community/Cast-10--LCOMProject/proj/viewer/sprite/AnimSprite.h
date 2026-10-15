#pragma once

#include <stdlib.h>

#include "Sprite.h"

/**
 * @brief Opaque animated sprite that cycles through a sequence of frames.
 */
typedef struct AnimSprite AnimSprite;

/**
 * @brief Allocates and initialises an AnimSprite.
 * @param animationTime Number of ticks each frame is displayed.
 * @param numSprites Total number of frames in the animation.
 * @param x Initial screen X coordinate.
 * @param y Initial screen Y coordinate.
 * @param sprites Array of numSprites Sprite pointers (the AnimSprite does not take ownership).
 * @return Pointer to the new AnimSprite, or NULL on failure.
 */
AnimSprite *createAnimSprite(int animationTime, int numSprites, int x, int y, Sprite **sprites);

/**
 * @brief Frees an AnimSprite previously created with createAnimSprite().
 * @param animSprite AnimSprite to destroy.
 */
void destroyAnimSprite(AnimSprite *animSprite);

/**
 * @brief Advances the animation by one tick, switching frames when animationTime is reached.
 * @param animSprite AnimSprite to update.
 */
void updateAnimSprite(AnimSprite *animSprite);

/**
 * @brief Draws the current frame of the animation at its stored position.
 * @param animSprite AnimSprite to draw.
 * @return 0 on success, non-zero on error.
 */
int drawAnimSprite(AnimSprite *animSprite);

/**
 * @brief Translates the animation position by a relative offset.
 * @param animSprite AnimSprite to move.
 * @param dx Horizontal offset in pixels.
 * @param dy Vertical offset in pixels.
 */
void moveAnimSprite(AnimSprite *animSprite, int dx, int dy);

/**
 * @brief Sets the absolute screen position of the animation.
 * @param animSprite AnimSprite to reposition.
 * @param x New screen X coordinate.
 * @param y New screen Y coordinate.
 */
void setAnimSpritePosition(AnimSprite *animSprite, int x, int y);

/**
 * @brief Resets the animation back to frame 0.
 * @param animSprite AnimSprite to reset.
 */
void animSpriteReset(AnimSprite *animSprite);

/**
 * @brief Returns 1 the first time this is called after the animation completes a full loop.
 * Subsequent calls return 0 until the next loop completes.
 * @param animSprite AnimSprite to query.
 * @return 1 if the animation just looped, 0 otherwise.
 */
int animSpriteHasLooped(AnimSprite *animSprite);

/**
 * @brief Returns the Sprite for the currently displayed frame.
 * @param animSprite Source AnimSprite.
 * @return Pointer to the current Sprite frame.
 */
Sprite *getCurrentSprite(AnimSprite *animSprite);
