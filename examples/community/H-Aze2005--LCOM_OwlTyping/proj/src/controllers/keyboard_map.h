/**
 * @file keyboard_map.h
 * @brief Defines the mapping between keyboard scan codes and characters.
 *
 * Provides the [`KeyMapEntry`](keyboard_map.h) structure and the [`key_map`](keyboard_map.h) array
 * for mapping keyboard make/break codes to their corresponding characters.
 */

#ifndef _KEYBOARD_MAP_
#define _KEYBOARD_MAP_

#include <lcom/lcf.h>
#include "devices/keyboard_layout.h"

/**
 * @struct KeyMapEntry
 * @brief Represents a mapping from keyboard make/break codes to a character.
 *
 * @var KeyMapEntry::make
 * Make code for the key.
 * @var KeyMapEntry::break_
 * Break code for the key.
 * @var KeyMapEntry::character
 * Character represented by the key.
 */
typedef struct {
    uint8_t make;      /**< Make code for the key. */
    uint8_t break_;    /**< Break code for the key. */
    char character;    /**< Character represented by the key. */
} KeyMapEntry;

/**
 * @brief Array mapping keyboard make/break codes to characters.
 *
 * Each entry in the array corresponds to a key and its associated codes and character.
 */
static const KeyMapEntry key_map[] = {
    {MKEY_A, BKEY_A, 'a'},
    {MKEY_B, BKEY_B, 'b'},
    {MKEY_C, BKEY_C, 'c'},
    {MKEY_CC, BKEY_CC, '*'}, // '*' -> 'ç'
    {MKEY_D, BKEY_D, 'd'},
    {MKEY_E, BKEY_E, 'e'},
    {MKEY_F, BKEY_F, 'f'},
    {MKEY_G, BKEY_G, 'g'},
    {MKEY_H, BKEY_H, 'h'},
    {MKEY_I, BKEY_I, 'i'},
    {MKEY_J, BKEY_J, 'j'},
    {MKEY_K, BKEY_K, 'k'},
    {MKEY_L, BKEY_L, 'l'},
    {MKEY_M, BKEY_M, 'm'},
    {MKEY_N, BKEY_N, 'n'},
    {MKEY_O, BKEY_O, 'o'},
    {MKEY_P, BKEY_P, 'p'},
    {MKEY_Q, BKEY_Q, 'q'},
    {MKEY_R, BKEY_R, 'r'},
    {MKEY_S, BKEY_S, 's'},
    {MKEY_T, BKEY_T, 't'},
    {MKEY_U, BKEY_U, 'u'},
    {MKEY_V, BKEY_V, 'v'},
    {MKEY_W, BKEY_W, 'w'},
    {MKEY_X, BKEY_X, 'x'},
    {MKEY_Y, BKEY_Y, 'y'},
    {MKEY_Z, BKEY_Z, 'z'},
    {MKEY_COMMA, BKEY_COMMA, '%'}, // '%' -> ','
    {MKEY_PERIOD, BKEY_PERIOD, '.'},
    {MKEY_SPACE, BKEY_SPACE, '_'},
    {MKEY_BACKSPACE, BKEY_BACKSPACE, '-'}
};

#endif
