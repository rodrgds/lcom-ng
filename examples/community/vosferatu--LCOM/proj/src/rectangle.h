#ifndef __RECTANGLE_H
#define __RECTANGLE_H

/** @defgroup Rectangle Rectangle
 * @{
 * Rectangle functions
 */

/*! Represents the Rectangle */
typedef struct{
	int xi,yi,xf,yf;
} Rectangle;


/**
 * @brief Creates a new rectangle, for the specified coordinates; allocates it
 * @param xi start point X
 * @param yi start point Y
 * @param xf end point X
 * @param yf end point Y
 * @return A pointer to a rectangle type
 */
Rectangle* newRectangle(int xi,int yi, int xf,int yf);

/**
 * @brief frees a rectangle from memory
 * @param rect pointer to a rectangle type
 */
void deleteRectangle(Rectangle* rect);

/**
 * @brief draws a border for the rectangle (used for mouse clicks)
 * @param rect pointer to a rectangle type
 */
void drawRectangle(Rectangle* rect);


/**@}*/


#endif /* _RECTANGLE_H */
