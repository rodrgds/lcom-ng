#ifndef TEXT_H
#define TEXT_H

#include <lcom/lcf.h>
#include <lcom/video_gr.h>

/** @defgroup text text
 *  @{
 *
 * Constants, structures and functions used for programming the text used by our program
 */

#define CHAR_WIDTH 6   /**< @brief the width of a character loaded from the font XPM*/
#define CHAR_HEIGHT 11 /**< @brief the height of a character loaded from the font XPM*/
#define FONT_WIDTH 16  /**< @brief how many characters exist per column */
#define FONT_HEIGHT 16 /**< @brief how many characters exist per row */

/**
 * @brief A structure to represent the possible alignments of a piece of text
 *
 * @param LEFT align the text to the left
 * @param CENTER align the text to the center
 * @param RIGHT align the text to the right
 */
typedef enum {
  LEFT,
  CENTER,
  RIGHT
} text_alignment_t;

/**
 * @brief Loads the text font xpm
 *
 */
void(text_load)();

/**
 * @brief Draws a piece of text to the screen
 *
 * @param text the text to be drawn
 * @param x upper-left x coordinate of the first character of the text
 * @param y upper-left y coordinate of the first character of the text
 * @param scale scale used to draw the text
 * @param color color of the text
 * @param alignment alignment of the text
 */
void(text_draw)(char *text, int16_t x, int16_t y, int16_t scale, int color, text_alignment_t alignment);

/**
 * @brief Returns the xpm image of the font
 *
 * @return font_xpm
 */
xpm_image_t(get_font_xpm)();

/**@}*/

#endif
