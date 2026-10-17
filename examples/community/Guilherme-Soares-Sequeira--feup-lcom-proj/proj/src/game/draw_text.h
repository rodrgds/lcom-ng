#ifndef DRAW_CHAT_H
#define DRAW_CHAT_H

#include "../utils/position.h"

#include "element.h"
#include "pixel_buffer.h"

/** @defgroup draw_chat draw_chat
 *  @{
 *
 * Constants, structures and functions used for programming the drawing of the chat
 */

#define CHAR_SCALE 2 /**< @brief the size scale of a character relative to the font used */

#define INPUT_CHAR_SIZE 80 /**< @brief size of the text buffer when writing strings to the canvas */

/**
 * @brief Adds a character to the current text
 *
 * @param c character to be added
 */
void(add_char_to_current_text)(char c);

/**
 * @brief Removes characters from the current text until it's empty
 */
void(clear_current_text)();

/**
 * @brief Returns the current text
 *
 * @return text
 */
char *(get_current_text) ();

/**
 * @brief Removes the last character to be added to the current text
 */
void(remove_char_from_current_text)();

/**@}*/

#endif
