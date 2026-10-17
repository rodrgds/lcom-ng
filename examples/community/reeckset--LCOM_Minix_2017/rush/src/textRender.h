#ifndef _TEXTRENDER_H_
#define _TEXTRENDER_H_
/** @defgroup textRender textRender
 * @{
 *
 * Functions to specify the game logic (what do kbd/mouse events do, what happens every frame and what happends on startup)
 */
#include "gameObjects.h"

/**
 * @brief Draws a number on the screen
 * @param objs - objLayer array
 * @param nbr - number to draw
 * @param x - xPosition
 * @param y - yPosition
 * @param align - alignment
 *
 * @return none
 */
void renderNumber(ObjLayer* objs, unsigned int nbr, int x, int y, int align);

/**
 * @brief Return a number's sprite path
 * @param src - number
 *
 * @return path of the given number's sprite (bmp)
 */
char* getBmpPathFromNbr(int src);

/**
 * @brief Clears the numbers from the screen
 * @param objs - objLayer array
 *
 * @return none
 */
void clearText(ObjLayer* objs);
#endif

/** @} end of textRender*/
