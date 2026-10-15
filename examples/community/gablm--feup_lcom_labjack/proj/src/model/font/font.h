#ifndef _LCOM_FONT_H_
#define _LCOM_FONT_H_

#include "../../data_structures/queue/queue.h"
#include "../../assets/font/font.h"
#include "../sprite/sprite.h"

/**
 * @defgroup font Font
 * @ingroup models
 * @{
 * 
 */

/**
 * @brief Structure representing a font with sprites for letters, numbers, and special characters.
 */
typedef struct {
  queue_t *xpms_numbers;  /**< Queue of sprites for numbers */
  queue_t *xpms_letters;  /**< Queue of sprites for letters */
  sprite_t *two_dots;     /**< Sprite for the colon character ':' */
  sprite_t *euro;         /**< Sprite for the euro symbol '~' */
} font_t;

/**
 * @brief Initializes the font by loading sprites for letters, numbers, and special characters.
 * 
 * @param font Pointer to the font to be initialized.
 * @return 0 on success, 1 on failure.
 */
int font_init(font_t *font);

/**
 * @brief Prints a single character at the specified coordinates with a specified color.
 * 
 * @param font Pointer to the font.
 * @param c Character to print.
 * @param x X coordinate for the character.
 * @param y Y coordinate for the character.
 * @param color Color to use for the character.
 */
void font_print_char(font_t *font, char c, uint32_t x, uint32_t y, uint32_t color);

/**
 * @brief Prints a string at the specified coordinates with a specified color.
 * 
 * @param font Pointer to the font.
 * @param str String to print.
 * @param x X coordinate for the string.
 * @param y Y coordinate for the string.
 * @param color Color to use for the string.
 */
void font_print_str(font_t *font, char *str, uint32_t x, uint32_t y, uint32_t color);

/**
 * @brief Prints a number at the specified coordinates with a specified color.
 * 
 * @param font Pointer to the font.
 * @param num Number to print.
 * @param x X coordinate for the number.
 * @param y Y coordinate for the number.
 * @param color Color to use for the number.
 */
void font_print_number(font_t *font, uint32_t num, uint32_t x, uint32_t y, uint32_t color);

/**
 * @brief Destroys the font and frees its resources.
 * 
 * @param font Pointer to the font to be destroyed.
 */
void font_destroy(font_t *font);

/** @} */

#endif /* _LCOM_FONT_H_ */
