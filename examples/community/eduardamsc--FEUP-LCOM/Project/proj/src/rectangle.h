#pragma once

typedef struct {
	int xi, yi, xf, yf;
} Rectangle;

/**
 * @brief Initializes struct with the coordinates for a rectangle (button)
 * @param xi x initial coordinate
 * @param yi y initial coordinate
 * @param xf x final coordinate
 * @param yf y final coordinate
 * @return The struct corresponding to the coordinates of a button's edges
 */
Rectangle* newRectangle(int xi, int yi, int xf, int yf);

/**
 * @brief Frees the space allocated for the rectangle
 * @param rect The struct with the button's coordinates to be released
 */
void deleteRectangle(Rectangle* rect);
