#ifndef ARROW_H
#define ARROW_H

/** @defgroup Arrow Arrow
 * @{
 * Functions for manipulating arrows
 */

struct Arrow;
typedef struct Arrow arrow_t;

/**
 * @brief Creates a new instance of arrow
 *
 * @param xIN Integer with the 'x' position of the new arrow
 * @param yIN Integer with the 'y' position of the new arrow
 * @return Returns the pointer to the new created arrow
 */
arrow_t* createArrow(int xIN, int yIN);

/**
 * @brief Gets the 'being_used' variable value
 * @param a The arrow we are referring to
 *
 * @return 1 on true and 0 on false
 */
int getBeingUsed(arrow_t *a);

/**
 * @brief This functions tells us if the arrow is inside the screen or not
 * @param a The arrow we are referring to
 *
 * @return 1 on true and 0 on false
 */
int isArrowInsideScreen(arrow_t *a);

/**
 * @brief Return the 'x' position of an arrow
 *
 * @param a The arrow we wish to know the 'x' position
 * @return Returns the int 'x' position of an arrow
 */
int getArrowPos_X(arrow_t *a);

/**
 * @brief Return the 'y' position of an arrow
 *
 * @param a The arrow we wish to know the 'y' position
 * @return Returns the int 'y' position of an arrow
 */
int getArrowPos_Y(arrow_t *a);

/**
 * @brief This function displays the bitmap image associated with an arrow
 *
 * @param b The arrow we wish to display
 */
void drawArrow(arrow_t *a);

/**
 * @brief Changes the current position of the balloon
 *
 * @param a The arrow we wish to change position
 * @param newX The new 'x' position of the arrow
 * @param newY The new 'y' position of the arrow
 */
void updateArrowPosition(arrow_t *a);

#endif /**@}*/
