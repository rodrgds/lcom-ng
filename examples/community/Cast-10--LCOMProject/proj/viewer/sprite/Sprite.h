#pragma once

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdlib.h>

#include "video.h"

/** @brief Index value in the palette that represents a transparent pixel. */
#define TRANSPARENT_COLOR 255

/**
 * @brief Opaque sprite loaded from an XPM image.
 */
typedef struct Sprite Sprite;

/**
 * @brief Loads an XPM map into a Sprite.
 * @param xpm XPM map to load.
 * @return Pointer to the new Sprite, or NULL on failure.
 */
Sprite *spriteCreate(xpm_map_t xpm);

/**
 * @brief Returns a read-only pointer to the sprite's pixel data.
 * @param sprite Source sprite.
 * @return Pointer to the raw pixel buffer.
 */
const uint8_t *spriteGetMap(const Sprite *sprite);

/**
 * @brief Frees a Sprite previously created with spriteCreate().
 * @param sprite Sprite to destroy.
 */
void spriteDestroy(Sprite *sprite);

/**
 * @brief Draws a sprite at the given screen coordinates.
 * Pixels with index TRANSPARENT_COLOR are skipped.
 * @param sprite Sprite to draw.
 * @param x Screen X coordinate of the top-left corner.
 * @param y Screen Y coordinate of the top-left corner.
 * @return 0 on success, non-zero on error.
 */
int spriteDraw(Sprite *sprite, int x, int y);

/**
 * @brief Returns the width of a sprite in pixels.
 * @param sprite Source sprite.
 * @return Width in pixels.
 */
int getWidth(Sprite *sprite);

/**
 * @brief Returns the height of a sprite in pixels.
 * @param sprite Source sprite.
 * @return Height in pixels.
 */
int getHeight(Sprite *sprite);
