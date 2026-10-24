/**
 * @file input.h
 * @brief Input aggregation and state tracking.
 */
#ifndef _INPUT_H_
#define _INPUT_H_

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Structure holding the current state of game inputs.
 */
typedef struct {
  bool up;
  bool down;
  bool left;
  bool right;
  bool shoot;
  bool shoot_clicked;
  bool pause;
  bool pause_clicked;
  bool escape;
  bool escape_clicked;
  int mouse_x;
  int mouse_y;
  bool mouse_left_pressed;
  bool mouse_left_clicked;
} InputState;

/**
 * @brief Initializes the InputState structure.
 * @param input Pointer to the InputState.
 */
void input_init(InputState *input);

/**
 * @brief Processes keyboard scancodes and updates state.
 * @param input Pointer to the InputState.
 * @param scancode The latest scancode read.
 */
void input_handle_keyboard(InputState *input, uint8_t scancode);

/**
 * @brief Processes mouse packets and updates state.
 * @param input Pointer to the InputState.
 * @param packet Array containing the 3-byte mouse packet.
 */
void input_handle_mouse(InputState *input, uint8_t *packet);

/**
 * @brief Resets single-frame input flags at the end of a frame.
 * @param input Pointer to the InputState.
 */
void input_end_frame(InputState *input);

#endif
