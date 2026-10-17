#ifndef __MOUSE_H
#define __MOUSE_H

#include <lcom/lcf.h>

#include "graphics.h"
#include "macros.h"
#include "pen_pal.h"

#include "img/xpms/cursors/cursor.xpm"
#include "img/xpms/cursors/select_cursor.xpm"
#include "img/xpms/cursors_erasers/s_eraser_cursor.xpm"
#include "img/xpms/cursors_erasers/m_eraser_cursor.xpm"
#include "img/xpms/cursors_erasers/l_eraser_cursor.xpm"

/** @defgroup mouse
 * @{
 *
 *
 *
 * Functions, modules and data structures related to the mouse
 */

/* Data structure used to represent a cursor*/
typedef struct {
  uint16_t x, y;            /**< X and Y coordinates of the cursor */

  xpm_image_t img;          /**< normal cursor img */
  xpm_image_t eraser_img;   /**< eraser cursor img*/
  xpm_image_t select_img;   /**< hoover cursor img */

  bool isOverOption;        /**< Indicates whether or not the cursor is hoovering a clickable element */

} Cursor;

/** @defgroup Collision_type
 * @{
 *
 * Enumerable containing all possible collisions types
 */
typedef enum {
  NO_COLLISION,
  OVER_WORD_1,
  OVER_WORD_2,
  OVER_WORD_3,
  OVER_COLOR_1,
  OVER_COLOR_2,
  OVER_COLOR_3,
  OVER_COLOR_4,
  OVER_COLOR_5,
  OVER_COLOR_6,
  OVER_COLOR_7,
  OVER_ERASER,
  OVER_TEXTAREA,
  OVER_S,
  OVER_M,
  OVER_L,
  OVER_PLAY_AGAIN,
  OVER_PLAY_SWITCH,
  OVER_EXIT_GAME
} Collision;


/** @defgroup mouse_states
 * @{
 *
 * Enumerable containing all possible game states
 */
typedef enum {
  MOUSE_GETTING_NICKNAMES,
  MOUSE_CHOOSING_WORD,
  MOUSE_DRAWING,
  MOUSE_ANALYSING,
  MOUSE_GUESSING,
  MOUSE_PLAY_AGAIN
} MouseState;

/**
 * Interrupt handler for the mouse
 */
void (mouse_ih)();

/**
 * @brief Subscribes and enables mouses interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */

int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribe mouse interruptions subscriptions
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Read a byte from the kbc output buffer
 * @param byte Where to read the data from output buffer to
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_read_out_buf)(uint8_t *byte);

/**
 * @brief Disable mouse data reporting
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_disable_data_reporting)();

/**
 * @brief Parse a packet from the mouse
 * @param pp packet to parse
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_parse_packet)(struct packet *pp);

/**
 * @brief Used to draw in the screen, draws a pixel in the screen
 * @param selected_color color of the pixel to be drawn
 * @param width Size of the pixel to be drawn
 * @param addToBackgroundFrame If true, add the image to the frame permanently (mouse purposes)
 */
void(mouse_draw_pixel)(uint32_t selected_color, char width, bool addToBackgroundFrame);

/**
 * @brief Checks for cursor collision with words
 * @return Collision type
 */
Collision (cursor_collisions_in_word_selection)();

/**
 * @brief Checks for cursor collision with elements, while drawing
 * @return Collision type
 */
Collision (cursor_collisions_in_game)();

/**
 * @brief Checks for cursor collision with options in the last menu before exiting
 * @return Collision type
 */
Collision(cursor_collisions_option_selection)();

/**
 * @brief Creates a new cursor at the specified coordinates
 * @param X x coordinate of the cursor
 * @param Y y coordinate of the cursor
 */
void (new_cursor)(uint16_t x, uint16_t y);

/**
 * @brief Before drawing the cursor on the new position, need to revert clear the last cursor position
 * with the contents that were in the background before the cursor passed through
 *
 */
void (delete_prev_frame)();

/**
 * @brief Draw the cursor on the screen
 *
 */
void (draw_cursor)();

/**
 * @brief Clears the previous cursor, updates its positon, and draws its in the screen
 * @param pp mouse packet containing new mouse position
 */
void (update_cursor)(struct packet *pp);

/**
 * @brief Update the cursor position, after receiving a complete mouse packet
 * @param pp mouse packet containing new mouse position
 */
void (update_cursor_position)(struct packet *pp);

/**
 * @brief Given a collision type, checks which color was clicked
 * @param new_color Will contain the new color
 * @param Collision Collision type
 *
 */
void(handle_mouse_collision_with_colors)(uint32_t *new_color, Collision collision);



#endif /* __MOUSE_H */
