
#ifndef _CURSOR_H_
#define _CURSOR_H_

/** @defgroup cursor Cursor
 *  @{
 *
 *  Cursor related functions
 */

/** A Cursor is an "object" that contains all needed information to create,
 *  draw, erase, update and destroy a cursor on the screen.
 */
typedef struct {

	unsigned int x;		/**< X position of cursor */
	unsigned int y;		/**< Y position of cursor */

} Cursor;

/**
 *  @brief Creates a cursor.
 *
 *  These cursor start at the position (500, 500)
 *
 *  @return Returns a pointer to a cursor.
 */
Cursor * create_cursor();

/**
 *  @brief Draws a cursor in the screen.
 *  @param c Cursor to draw.
 */
void draw_cursor(Cursor * c);

/**
 *  @brief Erases a cursor from the screen.
 *  @param c Cursor to erase.
 */
void erase_cursor(Cursor * c);

/**
 *  @brief Updates the position of the cursor.
 *
 *  Erases the cursor in the last position and draws it in the new position.
 *  @param c Cursor to update.
 *  @param dx Axis X variation.
 *  @param dy Axis Y variation.
 */
void update_cursor(Cursor *c , int dx, int dy);

/**
 *  @brief Destroy and free the memory allocated to the cursor.
 *  @param c Cursor to destroy.
 */
void destroy_cursor(Cursor * c);

/** @} end of cursor */

#endif
