#ifndef __MOUSE_H
#define __MOUSE_H

#include <stdint.h>
#include <stdbool.h>


//subscribe to mouse interrupts and enable them
int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)(void);

int (mouse_read_byte)(uint8_t *byte); //read a byte from the mouse, with error checking
int (mouse_read_ack) (uint8_t *ack); //read an ACK byte from the mouse, with error checking
int (mouse_write_aux_command) (uint8_t command); //write a command to the mouse

int (mouse_enable_data_report)(void);
int (mouse_disable_data_report)(void);

int (mouse_packet_is_ready)();
struct packet* (handle_packet) ();

void (mouse_ih)(void);

//Getters for the data read from the mouse
bool (mouse_has_error)();
uint8_t (mouse_get_data)();

// =========== PROJ ============

/**
 * @brief Velocity threshold in pixels per tick above which movement is a slash.
 */
#define SLASH_THRESHOLD 12

/**
 * @brief Represents the processed high-level mouse state.
 */
typedef struct MouseGameState {
  int x, y;          /**< Absolute cursor coordinates on the screen. */
  int prev_x, prev_y;/**< Previous absolute cursor coordinates on the screen. */
  int dx, dy;        /**< Relative movement deltas from the last packet. */
  bool lb_pressed;   /**< True if Left Button is pressed. */
  bool rb_pressed;   /**< True if Right Button is pressed. */
  bool mb_pressed;   /**< True if Middle Button is pressed. */
  bool is_slashing;  /**< True if moving faster than SLASH_THRESHOLD with Left Button held. */
} MouseGameState;

/**
 * @brief Initializes the mouse and subscribes to its interrupts.
 *
 * @param state Pointer to the MouseGameState structure to be initialized.
 * @param screen_width Width of the screen in pixels for boundary clamping.
 * @param screen_height Height of the screen in pixels for boundary clamping.
 * @param irq_set Pointer to store the subscribed IRQ hook bitmask.
 * @return 0 on success, non-zero on failure.
 */
int mouse_init(MouseGameState *state, int screen_width, int screen_height, uint8_t *irq_set);

/**
 * @brief Unsubscribes from mouse interrupts and disables data reporting.
 *
 * @return 0 on success, non-zero on failure.
 */
int mouse_cleanup(void);

/**
 * @brief Handles mouse interrupts and updates the game state.
 *
 * Calls the low-level interrupt handler to read a byte, compiles bytes into
 * a packet, and updates the MouseGameState absolute position and buttons.
 *
 * @param state Pointer to the MouseGameState structure to be updated.
 * @param screen_width Width of the screen in pixels for boundary clamping.
 * @param screen_height Height of the screen in pixels for boundary clamping.
 * @return true if a complete 3-byte packet was parsed and state updated, false otherwise.
 */
bool mouse_proj_ih(MouseGameState *state, int screen_width, int screen_height);

/**
 * @brief Updates the MouseGameState based on a newly parsed packet.
 *
 * Takes the raw mouse packet, negates the inverted Y-axis delta, accumulates
 * absolute screen coordinates, clamps the cursor to the screen dimensions, and
 * detects slash gestures using squared velocity.
 *
 * @param state Pointer to the MouseGameState structure to be updated.
 * @param pp Pointer to the parsed raw mouse packet struct.
 * @param screen_width Width of the screen in pixels for clamping boundaries.
 * @param screen_height Height of the screen in pixels for clamping boundaries.
 */
void mouse_update_state(MouseGameState *state, struct packet *pp, int screen_width, int screen_height);

/**
 * @brief Adds the current cursor position to the trail history.
 *
 * Keeps the newest point at index 0 and discards the oldest point once the
 * fixed-size trail buffer is full.
 *
 * @param state Pointer to the MouseGameState structure to be updated.
 */
/**
 * @brief Retrieves the latest accumulated X and Y movement deltas from the mouse.
 *
 * @param dx Pointer to store the accumulated horizontal displacement.
 * @param dy Pointer to store the accumulated vertical displacement.
 * @return true if movement has occurred, false otherwise.
 */
bool mouse_get_movement(MouseGameState *state, int *dx, int *dy);

/**
 * @brief Retrieves the current state of the mouse buttons as a bitmask.
 *
 * @return A bitmask representing the current state of the mouse buttons.
 */
uint8_t mouse_get_buttons(MouseGameState *state);

/**
 * @brief Retrieves the current absolute position of the mouse cursor.
 *
 * @param x Pointer to store the absolute cursor X coordinate.
 * @param y Pointer to store the absolute cursor Y coordinate.
 */
void mouse_get_position(MouseGameState *state, int *x, int *y);

#endif /* __MOUSE_H */
