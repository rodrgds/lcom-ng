#ifndef FONT_H
#define FONT_H

#include <stdint.h>

/**
 * @brief Retrieves the bitmap data for a given character.
 * 
 * @param c The character to retrieve the bitmap for.
 * @return A pointer to an 8-byte array representing the character's bitmap.
 */
const uint8_t* get_font_bitmap(char c);

/**
 * @brief Prints the bitmap of a character to the console (for debugging).
 * 
 * @param c The character to print.
 */
void print_font_bitmap(char c);


// type int to indicate success (0) or failure (1)

int draw_letter(char c, int x, int y, uint32_t color, int size);
int draw_string(const char* str, int x, int y, uint32_t color, int size);

#endif // FONT_H


