#ifndef _WORD_H_
#define _WORD_H_

#include <lcom/lcf.h>
#include "graphics.h"
#include "Sprite.h"
#include "words.h"

#include "img/xpms/letters/A.xpm"
#include "img/xpms/letters/B.xpm"
#include "img/xpms/letters/C.xpm"
#include "img/xpms/letters/D.xpm"
#include "img/xpms/letters/E.xpm"
#include "img/xpms/letters/F.xpm"
#include "img/xpms/letters/G.xpm"
#include "img/xpms/letters/H.xpm"
#include "img/xpms/letters/I.xpm"
#include "img/xpms/letters/J.xpm"
#include "img/xpms/letters/K.xpm"
#include "img/xpms/letters/L.xpm"
#include "img/xpms/letters/M.xpm"
#include "img/xpms/letters/N.xpm"
#include "img/xpms/letters/O.xpm"
#include "img/xpms/letters/P.xpm"
#include "img/xpms/letters/Q.xpm"
#include "img/xpms/letters/R.xpm"
#include "img/xpms/letters/S.xpm"
#include "img/xpms/letters/T.xpm"
#include "img/xpms/letters/U.xpm"
#include "img/xpms/letters/V.xpm"
#include "img/xpms/letters/W.xpm"
#include "img/xpms/letters/X.xpm"
#include "img/xpms/letters/Y.xpm"
#include "img/xpms/letters/Z.xpm"
#include "img/xpms/letters/Space.xpm"

#define DICTIONARY_SIZE 40


/** @defgroup words
 * @{
 *
 *
 *
 * Functions developped for all the alphabetical elements displayed on the screen
 */
/**
 * @brief Load the xpm of letter into the alphabet
 * @param letter The letter to load the xpm of
*/
void (load_letter)(char letter);

/**
 * @brief Loads all the letters in the alphabet xpms, by calling
 * load_letter() on each letter
 */
void (load_alphabet)();

/**
 * @brief Draws a word on the screen
 * @param word The word to be drawn
 * @param x The x position of the word
 * @param y The y position of the word
 */
void (draw_word)(char *word, int x, int y);

/**
 * @brief Draws a word on the screen, with a desired color
 * @param word The word to be drawn
 * @param x The x position of the word
 * @param y The y position of the word
 * @param color The color of the word
 */
void (draw_word_with_color)(char *word, int x, int y, uint32_t color);

/**
 * @brief Draw a bigger word on the screen (Could probalby be draw_phrase but wtv)
 * @param word The word to be drawn
 * @param x The x position of the word
 * @param y The y position of the word
 */
void (draw_big_word)(char *word, int x, int y);

/**
 * @brief Draw a letter on the screen
 * @param letter The letter to be drawn
 * @param x The x coordinate of the word
 * @param y The y coordinate of the word
 */
void (draw_letter)(char c, int x, int y);

/**
 * @brief Get the width of the xpm image of the character
 * @param c The character whose xpm width will be returned
 * @return width of the xpm image linked with the character c
 */
uint16_t (get_width)(char c);

/**
 * @brief Draw a letter on the screen, with a desired color
 *
 */
void(draw_letter_with_color)(char c, int x, int y, uint32_t color);

/**
 * @brief Retrive random word from the dictionary
 * @return random word
 */
char *(get_random_word)();

/**
 * @brief Append a character to the end of a string
 * @param str The string to append the character to
 * @param c The character to append
 */
void(add_character)(char *str, char c);

/**
 * @brief Delete the last character from the string
 * @param str The string to remove the character from
 * @return Returns deleted character
 */
char (remove_character)(char *str);

/**
 * @brief Given a position defined by x and y draws the character c
 * with the background color (essentially erasing it)
 * @param c The character to errase
 * @param x X coordinate of the character
 * @param y Y coordinate of the character
 */
void (erase_letter)(char c , int x , int y);

#endif
