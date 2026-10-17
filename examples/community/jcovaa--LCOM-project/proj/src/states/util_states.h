/**
 * @file util_states.h
 * @brief Utility functions for handling text and drawing paragraphs in the game states
 */

#ifndef _UTIL_STATES_H_
#define _UTIL_STATES_H_

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Calculate the width of a paragraph
 * 
 * @param paragraph Array of strings representing words in the paragraph
 * @param size Number of words in the paragraph
 * @param word_spacing Spacing between words in pixels
 * @return uint16_t Total width of the paragraph in pixels
 */
uint16_t calc_paragraph_width(const char** paragraph, size_t size, uint16_t word_spacing);

/**
 * @brief Draw a paragraph centered horizontally
 * 
 * @param paragraph Array of strings representing words in the paragraph
 * @param size Number of words in the paragraph
 * @param y Position Y to draw the paragraph
 * @param word_spacing Spacing between words in pixels
 * @param text_scale Scale of the text
 */
void draw_centered_paragraph(const char** paragraph, size_t size, uint16_t y, uint16_t word_spacing, uint8_t text_scale);

#endif // _UTIL_STATES_H_
