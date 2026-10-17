/**
 * @file utils.h
 * @brief Utility functions for character-to-XPM image mapping.
 *
 * Provides functions to retrieve XPM images for characters, including
 * normal, bright, and error-highlighted variants.
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <lcom/lcf.h>
#include "allChars.h"

/**
 * @brief Gets the XPM image corresponding to the given character.
 * @param c The character to get the XPM for.
 * @return The XPM image for the character.
 */
xpm_map_t get_xpm_from_char(char c);

/**
 * @brief Gets the bright XPM image corresponding to the given character.
 * @param c The character to get the bright XPM for.
 * @return The bright XPM image for the character.
 */
xpm_map_t get_bright_xpm_from_char(char c);

/**
 * @brief Gets the error XPM image corresponding to the given character.
 * @param c The character to get the error XPM for.
 * @return The error XPM image for the character.
 */
xpm_map_t get_error_xpm_from_char(char c);

#endif
