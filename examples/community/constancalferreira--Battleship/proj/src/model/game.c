#include "game.h"

/// Current game state
GameState state = MENU;

/// Previous game state
GameState previous_state = MENU;

/**
 * @brief Sets the current game state.
 *
 * If the new state is different from the current one, updates both
 * the current and previous states.
 *
 * @param new_state The new game state to set.
 */
void set_state(GameState new_state) {
  if (new_state == state) return;
  previous_state = state;
  state = new_state;
}

/**
 * @brief Gets the current game state.
 *
 * @return The current GameState.
 */
GameState get_state() {
  return state;
}

/**
 * @brief Gets the previous game state.
 *
 * @return The GameState before the current one.
 */
GameState get_previous_state() {
  return previous_state;
}

/**
 * @brief Handles keyboard input to change game states.
 *
 * Allows switching to MENU, RULES, HELP, GAME_OVER, or EXIT states
 * based on specific key presses. Useful for testing screen transitions.
 */
void game_keyboard_handler() {
  switch (scancode) {
    case M_KEY:
      set_state(MENU);
      scancode = 0; 
      break;
    case SPACE_KEY:
       if (state != RULES && state != HELP) set_state(RULES); 
       scancode = 0;
       break;
    case H_KEY:
       if (state != HELP && state != RULES) set_state(HELP);
       scancode = 0;
       break;   
    case ESC_KEY:
      set_state(EXIT);
      break;    

    default:
      break;
  }
}

/**
 * @brief Handles mouse movement and updates cursor position.
 *
 * Uses the latest mouse packet deltas to move the cursor,
 * clamping its position to the screen bounds.
 */
void game_mouse_handler() {
  cursor_x += mouse_packet.delta_x * MOUSE_SPEED;
  cursor_y -= mouse_packet.delta_y * MOUSE_SPEED;

  if (cursor_x < 0) cursor_x = 0;
  if (cursor_x > mode_info.XResolution - cursor->width)
    cursor_x = mode_info.XResolution - cursor->width;

  if (cursor_y < 0) cursor_y = 0;
  if (cursor_y > mode_info.YResolution - cursor->height)
    cursor_y = mode_info.YResolution - cursor->height;  
}

/**
 * @brief Frees all graphical buffers used throughout the game.
 *
 * This includes the current screen buffer and buffers for each
 * game screen (menu, arena, rules, help, game over, etc.).
 */
void free_buffers() {
  free_buffer(current_buffer);
  free_buffer(arena_buffer);
  free_buffer(menu_buffer);
  free_buffer(start_buffer);
  free_buffer(mode_buffer);
  free_buffer(rules_buffer);
  free_buffer(help_buffer);
  free_buffer(game_over_buffer);
}
