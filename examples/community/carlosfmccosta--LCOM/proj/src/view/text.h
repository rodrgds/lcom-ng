/**
 * @file text.h
 * @brief Declares the text rendering interface using large letter sprites.
 *
 * Provides functions to create, destroy, and draw text objects using large font sprites.
 */

#ifndef __TEXT_H
#define __TEXT_H

#include "model/sprite.h"
#include "assets/letters_big.h"
#include <ctype.h>

/**
 * @brief Struct representing a text object.
 */
struct text;
typedef struct text text_t;

/**
 * @brief Creates a text object at the given position.
 * @param x X coordinate for the text.
 * @param y Y coordinate for the text.
 * @return Pointer to the created text object, or NULL on failure.
 */
text_t *create_text(uint16_t x, uint16_t y);

/**
 * @brief Destroys a text object and frees its resources.
 * @param text Pointer to the text object to destroy.
 */
void destroy_text(text_t* text);

/**
 * @brief Draws a string using the text object.
 * @param text Pointer to the text object.
 * @param str String to draw (A-Z and space only).
 * @return 0 on success, non-zero on error.
 */
int draw_text(text_t* text, const char* str);

/**
 * @brief Gets the underlying sprite of the text object.
 * @param text Pointer to the text object.
 * @return Pointer to the sprite.
 */
sprite_t* get_text_sprite(text_t *text);

#endif //__TEXT_H
