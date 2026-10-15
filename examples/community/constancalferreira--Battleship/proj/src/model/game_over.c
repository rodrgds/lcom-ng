#include "game_over.h"
#include "view/game_over_view.h"

/// Pointer to the current drawing buffer (from elsewhere in the program)
extern uint8_t *current_buffer;

/// Buffer used to draw the Game Over screen
uint8_t *game_over_buffer = NULL;

/// Size of each frame buffer
extern unsigned int frame_size;

/// Current game state
extern GameState state;

/// Current X position of the cursor
extern int cursor_x;

/// Current Y position of the cursor
extern int cursor_y;

/// Mouse packet containing the latest mouse input data
extern struct packet mouse_packet;

/// Sprite representing the "Menu" option
extern Sprite *menu_option;

/// Sprite representing the "Exit" option
extern Sprite *exit_option;

/// Currently selected game over option
static GameOverOption selected_option = GO_MENU;

/**
 * @brief Allocates the game over buffer if it hasn't been created yet.
 */
void set_game_over_buffer() {
  if (game_over_buffer == NULL) {
    game_over_buffer = (uint8_t *) malloc(frame_size);
  }
}

/**
 * @brief Initializes the Game Over screen.
 *
 * Allocates the buffer and draws the initial Game Over background.
 */
void init_game_over() {
  set_game_over_buffer();
  if (game_over_buffer != NULL) {
    draw_game_over_background(game_over_buffer);
  }
  selected_option = GO_MENU;
}

/**
 * @brief Main loop for the Game Over state.
 *
 * Draws the Game Over screen and cursor, then swaps buffers.
 */
void game_over_main_loop() {
  draw_game_over_screen();
  draw_cursor(current_buffer);
  swap_buffers();
}

/**
 * @brief Handles keyboard input in the Game Over screen.
 *
 * Supports navigation between options using arrow keys and selection with Enter.
 */
void game_over_keyboard_handler() {
  extern uint8_t scancode;

  if (scancode == UP_ARROW_KEY) {
      selected_option = GO_MENU;
  }
  else if (scancode == DOWN_ARROW_KEY) {
      selected_option = GO_EXIT;
  }
  else if (scancode == ENTER_KEY) {
      if (selected_option == GO_MENU) {
          set_state(START);
      }
      else if (selected_option == GO_EXIT) {
          set_state(EXIT);
      }
  }
}

/**
 * @brief Checks if the mouse is hovering over a given Game Over option.
 *
 * @param option The option to check (GO_MENU or GO_EXIT).
 * @return true if the cursor is over the given option, false otherwise.
 */
bool is_mouse_over_option(GameOverOption option) {
  if (option == GO_MENU) {
      return (cursor_x >= OPTIONS_X && 
              cursor_x <= OPTIONS_X + menu_option->width &&
              cursor_y >= OPTION_MENU_Y && 
              cursor_y <= OPTION_MENU_Y + menu_option->height);
  }
  else if (option == GO_EXIT) {
      return (cursor_x >= OPTIONS_X && 
              cursor_x <= OPTIONS_X + exit_option->width &&
              cursor_y >= OPTION_EXIT_Y && 
              cursor_y <= OPTION_EXIT_Y + exit_option->height);
  }
  return false;
}

/**
 * @brief Handles mouse input in the Game Over screen.
 *
 * Changes the selected option based on mouse hover and clicks.
 */
void game_over_mouse_handler() {
  if (is_mouse_over_option(GO_MENU)) {
      selected_option = GO_MENU;

      if (mouse_packet.lb) {
          set_state(MENU);
      }
  }
  else if (is_mouse_over_option(GO_EXIT)) {
      selected_option = GO_EXIT;

      if (mouse_packet.lb) {
          set_state(EXIT);
      }
  }
}

/**
 * @brief Gets the currently selected Game Over option.
 *
 * @return The currently selected option.
 */
GameOverOption get_game_over_option() {
  return selected_option;
}

/**
 * @brief Sets the currently selected Game Over option.
 *
 * @param option The option to set as selected.
 */
void set_game_over_option(GameOverOption option) {
  selected_option = option;
}
