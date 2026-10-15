#ifndef __PROJ_INPUT_H
#define __PROJ_INPUT_H

/** @defgroup input input
 * @{
 *
 * Functions for handling user input
 */

/**
 * @brief Key enumeration for supported input keys
 */
typedef enum {
  KEY_NONE,
  KEY_UP,
  KEY_DOWN,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_ENTER,
  KEY_SPACE,
  KEY_ESCAPE,
} Key;

/* button hit rectangles for menu navigation */
#define START_BX 243 /**< @brief Left edge of "Start" button */
#define START_BY 541 /**< @brief Top of both buttons */
#define BUTTON_W 230 /**< @brief Width of "Start" button */
#define BUTTON_H 35  /**< @brief Height of both buttons */

#define EXIT_BX 617 /**< @brief Left edge of "Exit" button */
#define EXIT_BY 541 /**< @brief Same top Y as "Start" */

/**
 * @brief Translates a scancode to a Key enum value
 *
 * @param scancode The scancode to translate
 *
 * @return The corresponding Key enum value, or KEY_NONE if not recognized
 */
Key(translate_scancode)(uint8_t scancode);

/**@}*/

#endif /* __PROJ_INPUT_H */
