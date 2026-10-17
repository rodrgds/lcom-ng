/**
 * @file virtual_keyboard_controller.h
 * @brief Controller for managing the state and performance of a virtual keyboard.
 *
 * Declares functions and types for handling key presses, releases, and tracking
 * key performance for a virtual keyboard interface.
 */

#ifndef _VIRTUAL_KEYBOARD_CONTROLLER_
#define _VIRTUAL_KEYBOARD_CONTROLLER_

#include <lcom/lcf.h>

#define NUM_KEYS 31

/**
 * @brief Array of characters representing the keys in the virtual keyboard.
 */
static const char key_map_chars[NUM_KEYS];

/**
 * @enum KeyPerformance
 * @brief Represents the performance rating for a key.
 */
typedef enum {
  KEY_EXCELLENT,    /**< Excellent performance */
  KEY_GOOD,         /**< Good performance */
  KEY_AVERAGE,      /**< Average performance */
  KEY_BAD,          /**< Bad performance */
  KEY_NO_DATA,      /**< No data available */
} KeyPerformance;

/**
 * @brief Array mapping each key to its performance rating.
 */
typedef KeyPerformance KeyPerformanceMap[NUM_KEYS];

/**
 * @brief Gets the array representing the pressed state of each key.
 * @return Pointer to a boolean array indicating pressed keys.
 */
bool* get_pressed_keys();

/**
 * @brief Gets the index of a key in the key map.
 * @param c The character of the key.
 * @return The index of the key, or -1 if not found.
 */
int get_key_index(char c);

#endif
