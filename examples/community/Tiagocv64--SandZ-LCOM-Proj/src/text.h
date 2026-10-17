#pragma once
#include <stdint.h>

/** @defgroup text text
 * @{
 * Functions for manipulating text drawing
 */

/**
 * @brief      Initialize text structure
 */
void init_text();

/**
 * @brief      Destroy text structure and free memory used by it
 */
void destroy_text();

/**
 * @brief      Print a character to screen
 *
 * @param[in]  c          { character to print }
 * @param[in]  y          { y coordinate }
 * @param[in]  x          { x coordinate }
 * @param[in]  color      { color of character }
 * @param[in]  size_text  { size of character: SMALL, MEDIUM, LARGE }
 */
void print_char(char c, uint32_t y, uint32_t x, uint32_t color, int size_text);

/**
 * @brief      Print a string to screen
 *
 * @param      string     { string to print }
 * @param[in]  size       { size of string }
 * @param[in]  y          { y coordenate }
 * @param[in]  x          { x coordenate }
 * @param[in]  color      { color of string }
 * @param[in]  size_text  { size of string's characters: SMALL, MEDIUM, LARGE }
 * @param[in]  align      { way of text alignment relative to coordinates: ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT }
 */
void print_string(char* string, uint32_t size, uint32_t y, uint32_t x, uint32_t color, int size_text, int align);
