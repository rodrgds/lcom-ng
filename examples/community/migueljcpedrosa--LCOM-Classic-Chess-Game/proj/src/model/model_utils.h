/**
 * @file model_utils
 * @brief This file has the implementation of the functions related to the utils of the model.
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include "view/sprite.h"

/**
 * @brief Constant for the screen width.
 * 
 */
#define SCREEN_WIDTH 1152

/**
 * @brief Constant for the screen height.
 * 
 */
#define SCREEN_HEIGHT 864

/**
 * @brief Constant for the square size.
 * 
 */
#define SQUARE_SIZE 100

/**
 * @brief Struct representing a Title.
 *
 * This struct includes the x and y coordinates for positioning the title, a pointer to an array of words, 
 * the number of words in the title, and a sprite representing the font.
 */
typedef struct {

    int x;
    int y;
    char** words;
    int num_words;
    Sprite* font;

} Title;

/**
 * @brief Struct representing a Button.
 *
 * This struct includes the x and y coordinates for positioning the button, the width and height of the button,
 * and a sprite representing the button.
 */
typedef struct {

  int x;
  int y;
  int width;
  int height;

  Sprite* sprite;

} Button;

#endif
