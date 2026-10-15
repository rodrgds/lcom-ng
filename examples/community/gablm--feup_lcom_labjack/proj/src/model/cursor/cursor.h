#ifndef _LCOM_CURSOR_H_
#define _LCOM_CURSOR_H_

#include <lcom/lcf.h>

#include "../../drivers/drivers.h"
#include "../sprite/sprite.h"
#include "../../assets/cursor.xpm"
#include "../../assets/hand.xpm"

/**
 * @defgroup cursor Cursor
 * @ingroup models
 * @{
 */

/**
 * @brief Enum for the different cursor states.
 */
typedef enum {
	POINTER, /**< Pointer state */
	HAND     /**< Hand state */
} cursor_state_t;

/**
 * @brief Structure representing a cursor.
 */
typedef struct {
	uint32_t x;               /**< X coordinate of the cursor */
	uint32_t y;               /**< Y coordinate of the cursor */
	sprite_t *pointer;        /**< Pointer sprite */
	sprite_t *hand;           /**< Hand sprite */
	cursor_state_t state;     /**< Current state of the cursor */
} cursor_t;

/**
 * @brief Initializes the cursor.
 * 
 * @param cursor Pointer to the cursor to be initialized.
 * @return 0 on success, 1 on failure.
 */
int		cursor_init(cursor_t *cursor);

/**
 * @brief Moves the cursor to the specified coordinates.
 * 
 * @param cursor Pointer to the cursor to be moved.
 * @param x New X coordinate.
 * @param y New Y coordinate.
 * @return 0 on success, 1 on failure.
 */
int		cursor_move(cursor_t *cursor, uint32_t x, uint32_t y);

/**
 * @brief Draws the cursor on the screen.
 * 
 * @param cursor Pointer to the cursor to be drawn.
 * @return 0 on success, 1 on failure.
 */
int		cursor_draw(cursor_t *cursor);

/**
 * @brief Checks if the cursor collides with a given sprite.
 * 
 * @param cursor Pointer to the cursor.
 * @param sprite Pointer to the sprite to check collision with.
 * @return 1 if collision occurs, 0 otherwise.
 */
int		cursor_sprite_colides(cursor_t *cursor, sprite_t *sprite);

/**
 * @brief Checks if the cursor collides with a given rectangular box.
 * 
 * @param cursor Pointer to the cursor.
 * @param x Top-left X coordinate of the box.
 * @param y Top-left Y coordinate of the box.
 * @param x2 Bottom-right X coordinate of the box.
 * @param y2 Bottom-right Y coordinate of the box.
 * @return 1 if collision occurs, 0 otherwise.
 */
int		cursor_box_colides(cursor_t *cursor, uint32_t x, uint32_t y, uint32_t x2, uint32_t y2);

/**
 * @brief Destroys the cursor and frees its resources.
 * 
 * @param cursor Pointer to the cursor to be destroyed.
 */
void	cursor_destroy(cursor_t *cursor);

/** @} */

#endif /* _LCOM_EV_CURSOR_H_ */
