#ifndef _INSTRUCTIONS_VIEWER_H_
#define _INSTRUCTIONS_VIEWER_H_

/**
 * @file instructions_viewer.h
 * @brief Header file containing function declarations for instructions viewer functions.
 */

#include "viewer.h"

/** @defgroup instructions_viewer instructions_viewer
 * @{
  */
extern Sprite* instructions_text_1;
extern Sprite* instructions_text_2;
extern Sprite* instructions_text_3;
extern Sprite* instructions_text_4;
extern Sprite* instructions_text_5;
extern Sprite* instructions_text_6;

/**
 * @brief Draws the instructions menu with the instruction texts.
 * @return 1 if the drawing is successful, 0 otherwise.
 */
int (draw_instructions_menu)();

#endif
