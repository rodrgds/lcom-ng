#ifndef __MOUSE_H
#define __MOUSE_H

#include <lcom/lcf.h>

/**
 * @defgroup mouse Mouse
 * @{
 *
 * Functions and structures for interacting with the mouse
 */

/**
 * @brief Subscribes to mouse interrupts
 *
 * @param bit_no Pointer to a variable to store the bit mask for the interrupt
 * @return Return 0 upon success, non-zero otherwise
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from mouse interrupts
 *
 * @return Return 0 upon success, non-zero otherwise
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Mouse interrupt handler
 */
void (mouse_ih)();

/**
 * @brief Processes the received mouse packet byte
 */
void (process_packet_byte)();

/**
 * @brief Writes a command to the mouse
 *
 * @param command Command to write
 * @return Return 0 upon success, non-zero otherwise
 */
int (mouse_write)(uint8_t command);

/**
 * @brief Performs a mouse gesture
 *
 * @param x_len Length of the gesture in the x direction
 * @param tolerance Tolerance for detecting the gesture
 * @return Return 0 upon success, non-zero otherwise
 */
int (mouse_gesture)(uint8_t x_len, uint8_t tolerance);

/**
 * @brief Enumeration of possible mouse states
 */
enum mouse_states {
  INITIAL,       /*!< Initial state */
  FIRST_LINE,    /*!< First line drawn */
  VERTEX,        /*!< Vertex reached */
  SECOND_LINE,   /*!< Second line drawn */
  COMPLETE       /*!< Gesture complete */
};

/**
 * @brief Updates the mouse position
 */
void (update_mouse_position)();

/**
 * @brief Structure representing the mouse position
 */
struct mouse_position {
  int x, y;      /*!< Coordinates */
};


#endif /* __MOUSE_H */
