#ifndef _PAUSE_MENU_STATE_H_
#define _PAUSE_MENU_STATE_H_

#include <stdbool.h>

/**
 * @file pause_menu_state.h
 * @brief Declares pause menu state transitions.
 */

/**
 * @brief Selectable entries in the pause menu.
 */
typedef enum {
  /** @brief No pause menu entry is selected. */
  PAUSE_OPTION_NONE,

  /** @brief Resume the current game. */
  PAUSE_OPTION_RESUME,

  /** @brief Return to the main menu. */
  PAUSE_OPTION_MAIN_MENU,

  /** @brief Exit the application. */
  PAUSE_OPTION_EXIT
} PauseOption;

/**
 * @brief Actions emitted by the pause menu state machine.
 */
typedef enum {
  /** @brief No action should be taken. */
  PAUSE_ACTION_NONE,

  /** @brief Resume gameplay. */
  PAUSE_ACTION_RESUME,

  /** @brief Return to the main menu. */
  PAUSE_ACTION_MAIN_MENU,

  /** @brief Exit the application. */
  PAUSE_ACTION_EXIT
} PauseAction;

/**
 * @brief Mutable state for the pause menu.
 */
typedef struct {
  /** @brief Option currently hovered by the input cursor. */
  PauseOption hovered_option;
} pause_menu_t;

/**
 * @brief Initialize pause menu state.
 *
 * @param menu Pause menu state to initialize.
 */
void pause_menu_init(pause_menu_t *menu);

/**
 * @brief Update pause menu state from hover and click input.
 *
 * @param menu Pause menu state to mutate.
 * @param hovered_option Option currently under the cursor.
 * @param clicked Whether the user clicked this frame.
 * @return Action requested by the pause menu.
 */
PauseAction pause_menu_update(pause_menu_t *menu, PauseOption hovered_option, bool clicked);

#endif /* _PAUSE_MENU_STATE_H_ */
