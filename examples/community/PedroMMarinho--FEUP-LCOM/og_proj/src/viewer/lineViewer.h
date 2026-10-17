#pragma once

#include <lcom/lcf.h>
#include "../model/vector.h"


#define LINE_OVERLAP_NONE 0
#define LINE_OVERLAP_MAJOR 0x01
#define LINE_OVERLAP_MINOR 0x02
#define LINE_OVERLAP_BOTH 0x03

/**
 * @brief Draws a line between two points with optional overlapping pixels.
 * 
 * This function draws a line between the specified starting and ending points on the screen.
 * It provides the option to draw overlapping pixels at the endpoints to improve visual continuity when drawing thick lines.
 * 
 * @param aXStart The x-coordinate of the starting point.
 * @param aYStart The y-coordinate of the starting point.
 * @param aXEnd The x-coordinate of the ending point.
 * @param aYEnd The y-coordinate of the ending point.
 * @param aOverlap A bitmask indicating which pixels to draw as overlapping. 
 *                 It can be one of the following values:
 *                   - LINE_OVERLAP_NONE: Do not draw any overlapping pixels.
 *                   - LINE_OVERLAP_MAJOR: Draw overlapping pixels in the main direction of the line.
 *                   - LINE_OVERLAP_MINOR: Draw overlapping pixels in the minor direction of the line.
 * @param aColor The color of the line.
 */

void drawLineOverlap(unsigned int aXStart, unsigned int aYStart, unsigned int aXEnd, unsigned int aYEnd, uint8_t aOverlap,
                     uint32_t aColor);

/**
 * @brief Draws a line with thickness using Bresenham's algorithm.
 *
 * This function draws a line with the specified thickness using Bresenham's algorithm.
 *
 * @param aXStart The x-coordinate of the starting point.
 * @param aYStart The y-coordinate of the starting point.
 * @param aXEnd The x-coordinate of the ending point.
 * @param aYEnd The y-coordinate of the ending point.
 * @param aThickness The thickness of the line.
 * @param aColor The color of the line.
 */                
void drawThickLine(vector_t start, vector_t end, unsigned int aThickness, uint32_t aColor);
