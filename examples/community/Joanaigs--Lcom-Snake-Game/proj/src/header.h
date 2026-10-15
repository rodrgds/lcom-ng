#include <lcom/lcf.h>
#include "objects.h"


/**
 * game time counter
 */
int gameTime;

/**
 * @brief image of the clock
 *
 */
object timeClock;
/**
 * @brief image of left half of the heart
 */
object leftHeart;
/**
 * @brief image of right half of the heart
 *
 */
object rightHeart;
/**
 * @brief image of number 1
 *
 */
object one;
/**
 * @brief image of number 2
 *
 */
object two;
/**
 * @brief image of number 3
 *
 */
object three;
/**
 * @brief image of number 4
 *
 */
object four;
/**
 * @brief image of number 5
 *
 */
object five;
/**
 * @brief image of number 6
 *
 */
object six;
/**
 * @brief image of number 7
 *
 */
object seven;
/**
 * @brief image of number 8
 *
 */
object eight;
/**
 * @brief image of number 9
 *
 */
object nine;
/**
 * @brief image of number 0
 *
 */
object zero;

/**
 * Initiates the header xpm elements
 */
void initHeaderElements();

/**
 * @brief Draws on the screen the number passed by parameter
 * 
 * @param number to be drawn
 * @param x position (column) where to start drawing from
 * @param y position (line) where to start drawing from
 */
void draw_number(int number, int x, int y);

/**
 * @brief erases the number drawn on the screen
 *
 * @param number to be erased
 * @param x position (column) where to start erasing from
 * @param y position (line) where to start erasing from
 *
 * Temp variable indicates if the heart to be erased is from the left or right
 * even temp value -> left heart
 * odd temp value -> right heart
 */
void erase_number(int number, int x, int y);

/**
 * @brief draws the lives on the screen
 * 
 * @param number of hearts (lives) to be drawn
 * @param x position (column) to start drawing from
 * @param y position (line) to start drawing from
 */
void drawHearts(double number, int x, int y);

/**
 * @brief erases the hearts on the screen
 *
 * @param number of hearts to be erased
 * @param x column where to start erasing from
 * @param y line where to start erasing from
 */
void eraseHearts(double number, int x, int y);

/**
 * @brief erases the time drawn on the screen
 * 
 */
void eraseTime();

/**
 * @brief erases the number of apples drawn on screen
 * 
 */
void eraseAppleNumb();

/**
 * @brief Draws the header with the hearts, the time and apples information
 *
 */
void drawHeader();
