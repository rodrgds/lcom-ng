#pragma once

/** @defgroup rectangle rectangle
 * @{
 * Functions for rectangle manipulation
 */

typedef struct {
	int x; // specifies the x left upper corner position
	int y; // specifies the y left upper corner position
	int w; // specifies the width
	int h; // specifies the height
} Rectangle;


/**
 * @brief Creates a new Rectangle
 *
 * @param x X position
 * @param y Y position
 * @param w Width value
 * @param h Height value
 * @return Non NULL pointer to the rectangle
 */
Rectangle* newRectangle(int x, int y, int w, int h);

/**
 * @brief Deletes a Rectangle
 *
 * @param rectangle Pointer to Rectangle
 */
void deleteRectangle(Rectangle * rectangle);

/**
 * @brief Set a rectangle location
 *
 * @param rectangle Pointer to Rectangle
 * @param x X new position
 * @param y Y new position
 */
void setRecLocation(Rectangle * rectangle, int x, int y);

/**
 * @brief Move a rectangle
 *
 * @param rectangle Pointer to Rectangle
 * @param x X variation
 * @param y Y variation
 */
void moveRectangle(Rectangle* rectangle, int x, int y);

/**
 * @brief Draw a rectangle
 *
 * @param rectangle Pointer to Rectangle
 * @param color Rectangle color
 * @param buf Buffer to draw a rectangle
 */
void drawRectangle(Rectangle* rectangle, int color, char* buf);

/**
 * @brief Verify rectangle collision
 *
 * @param r1 Pointer to Rectangle
 * @param r2 Pointer to Rectangle
 * @return validation
 */
int rectanglesColliding(Rectangle* r1, Rectangle* r2);

/**
 * @brief Verify rectangle inside
 *
 * @param r1 Pointer to Rectangle
 * @param r2 Pointer to Rectangle
 * @return validation
 */
int rectangleInside(Rectangle* r1, Rectangle* r2);
