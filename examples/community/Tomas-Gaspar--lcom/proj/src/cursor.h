#ifndef _CURSOR_H_
#define _CURSOR_H_

#include <stdint.h>

/**
 * @defgroup cursor Cursor
 * 
 * @brief Functions related to the cursor representation
 * 
 * @{
*/

/** @name Cursor struct */
/**@{
 *
 * @brief Struct representing a cursor object with its properties
 * 
 */
typedef struct {
    uint16_t x;             /**< @brief The X-coordinate of the cursor*/
    uint16_t y;             /**< @brief The Y-coordinate of the cursor*/
    uint16_t width;         /**< @brief The width of the screen*/
    uint16_t height;        /**< @brief The height of the screen*/

    uint32_t pencilColor;   /**< @brief The color of the pencil when drawing*/
    uint8_t pencilSize;     /**< @brief The size of the pencil when drawing*/
    char** cursorMap;       /**< @brief The array representing the XPM of the cursor*/

    uint8_t xpmCode;        /**< @brief The XPM code of the cursor*/
} Cursor;
/** @} end of Cursor*/

/**
 * @brief Create a cursor
 * 
 * Creates a cursor with the specified width, height, pencil color, cursor map, and XPM code.
 * 
 * @param width The width of the screen
 * @param height The height of the screen
 * @param pencilColor The color of the pencil when drawing
 * @param cursorMap The array representing the XPM of the cursor
 * @param xpmCode The XPM code of the cursor
 * @return The created cursor
*/
Cursor create_cursor(uint16_t width, uint16_t height, uint32_t pencilColor, char** cursorMap, uint8_t xpmCode);

/**
 * @brief Move the cursor
 * 
 * Moves the cursor by the specified deltas in the X and Y directions and it manages the limits of the screen.
 * 
 * @param cursor Pointer to the cursor
 * @param dx The delta in the X direction
 * @param dy The delta in the Y direction
*/
void move_cursor(Cursor* cursor, int16_t dx, int16_t dy);

/**
 * @brief Draw and move the cursor
 * 
 * Draws the cursor with the specified color and simultaneously moves it by the specified deltas in the X and Y directions.
 * 
 * @param cursor Pointer to the cursor
 * @param dx The delta in the X direction
 * @param dy The delta in the Y direction
 * @param color The color to draw the cursor
*/
void draw_and_move_cursor(Cursor* cursor, int16_t dx, int16_t dy, uint32_t color);

/**
 * @brief Check if the cursor is within limits
 * 
 * @param cursor Pointer to the cursor
 * @param xi The X-coordinate of the start limit
 * @param xf The X-coordinate of the end limit
 * @param yi The Y-coordinate of the start limit
 * @param yf The Y-coordinate of the end limit
 * 
 * @return True if the cursor is within the limits, false otherwise
*/
bool is_on_limits(Cursor* cursor, uint16_t xi, uint16_t xf, uint16_t yi, uint16_t yf);

/** }@ */

#endif
