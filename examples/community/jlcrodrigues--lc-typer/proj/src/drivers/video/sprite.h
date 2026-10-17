#ifndef SPRITE_H
#define SPRITE_H

#include <lcom/lcf.h>
#include "alphabet.h"
#include "digits.h"

/** @defgroup sprite
 * @{
 * 
 *  Drawable screen image
 */

#define DIGIT_HEIGHT 57
#define DIGIT_WIDTH 30
#define FONT_HEIGHT 57
#define FONT_WIDTH 30
#define PADDING 5
#define LINE_HEIGHT (FONT_HEIGHT + PADDING)

/**
 * @brief Structure that holds drawable images.
 * Use "::"<create_sprite> to load an image.
 * 
 */
typedef struct Sprite
{
  /** @brief xpm_image_t img Loaded from the xpm. */
  xpm_image_t img;
  /** @brief uint8_t* pix_map Map of the image colors. */
  uint8_t* pix_map;
} Sprite;

#include "driver.h" //Including video after Sprite is defined

/**
 * @brief Create a sprite from from a xpm.
 * @param xpm XPM map.
 * @return sprite Sprite struct with sprite info.
 */
Sprite (create_sprite)(xpm_map_t xpm);

/**
 * @brief Loads the font xpm and stores it in a local variable.
 * This is executed once per run.
 * 
 */
void (load_font)(void);

/**
 * @brief Draw a character to the screen.
 * The char is only drawn if its image is available.
 * 
 * @param letter Character to be drawn.
 * @param pos_x X position on the screen.
 * @param pos_y Y position on the screen.
 * @param color Color in RRGGBB mode(hex).
 * @return int 0 if the char is drawn, false otherwise.
 */
int draw_char(char letter, int pos_x, int pos_y, uint32_t color);

/**
 * @brief Draw a whole sentence to the screen.
 * The function separates the letters and marks spaces.
 * 
 * @param sentence Char* Sentence to be drawn.
 * @param pos_x X position on the screen.
 * @param pos_y Y position on the screen.
 * @param color Color in RRGGBB mode(hex).
 * @return int 0 if the sentence is drawn, false otherwise.
 */
int draw_sentence(char *sentence, int pos_x, int pos_y, uint32_t color);

/**
 * @brief Get the width of a sentence on the screen.
 * 
 * @param sentence Char* Sentence to be drawn.
 * @return int Size (pixels) of the sentence.
 */
int get_sentence_width(char *sentence);

/**@}*/

#endif
