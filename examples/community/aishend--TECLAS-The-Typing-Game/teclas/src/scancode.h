/**
 * @file scancode.h
 * @brief Keyboard scancode to ASCII conversion.
 */
#ifndef _TECLAS_SCANCODE_H_
#define _TECLAS_SCANCODE_H_

#include <lcom/lcf.h>

/**
 * @brief Convert a keyboard scancode to its ASCII character.
 *
 * @param sc Raw keyboard scancode.
 * @return Corresponding ASCII character, or 0 if the scancode has no mapping.
 */
char scancode_to_char(uint8_t sc);

#endif
