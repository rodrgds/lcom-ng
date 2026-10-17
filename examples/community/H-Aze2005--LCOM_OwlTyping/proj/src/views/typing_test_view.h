/**
 * @file typing_test_view.h
 * @brief Typing test view rendering functions.
 *
 * Declares functions for drawing the typing test text and typed characters on the screen.
 */

#ifndef TYPING_TEST_VIEW_H
#define TYPING_TEST_VIEW_H

#include <lcom/lcf.h>
#include "utils/utils.h"
#include "devices/video.h"
#include "images/text_cursor.xpm"

/**
 * @brief Draws the phrase text for the typing test.
 * @param shuffled_indices Array of shuffled indices for the phrase.
 * @param phrase_index Index of the phrase to display.
 */
void draw_text(const int* shuffled_indices, int phrase_index);

/**
 * @brief Draws the characters typed by the user during the typing test.
 * @param typed_chars Array of typed characters.
 * @param typed_count Number of characters typed.
 * @param shuffled_indices Array of shuffled indices for the phrase.
 * @param phrase_index Index of the phrase to display.
 */
void draw_typed_chars(const char* typed_chars, int typed_count, const int* shuffled_indices, int phrase_index);

#endif
