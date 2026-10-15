#ifndef _MENU_STATE_H_
#define _MENU_STATE_H_

#include <stdbool.h>
#include <stdint.h>

/**
 * @file menu_state.h
 * @brief Declares main menu state transitions.
 */

/**
 * @brief Selectable entries in the main menu.
 */
typedef enum {
  /** @brief No menu entry is selected. */
  MENU_OPTION_NONE,

  /** @brief Start singleplayer gameplay. */
  MENU_OPTION_SINGLEPLAYER,

  /** @brief Set gameplay seed. */
  MENU_OPTION_SEED,

  /** @brief Exit the application. */
  MENU_OPTION_EXIT
} MenuOption;

/**
 * @brief Actions emitted by the main menu state machine.
 */
typedef enum {
  /** @brief No action should be taken. */
  MENU_ACTION_NONE,

  /** @brief Transition to singleplayer gameplay. */
  MENU_ACTION_START_SINGLEPLAYER,

  /** @brief Transition to enter seed. */
  MENU_ACTION_ENTER_SEED,

  /** @brief Exit the application. */
  MENU_ACTION_EXIT
} MenuAction;

/** @brief Maximum number of decimal digits accepted for a uint32 seed. */
#define MENU_SEED_MAX_DIGITS 10

/**
 * @brief Mutable state for the main menu.
 */
typedef struct {
  /** @brief Option currently hovered by the input cursor. */
  MenuOption hovered_option;

  /** @brief Whether the user is currently typing a manual seed. */
  bool entering_seed;

  /** @brief Text buffer storing the typed uint32 seed digits. */
  char seed_str[MENU_SEED_MAX_DIGITS + 1];

  /** @brief Parsed numeric seed value, or 0 when no seed is set. */
  uint32_t seed_val;
} menu_t;

/**
 * @brief Initialize main menu state.
 *
 * @param menu Menu state to initialize.
 */
void menu_init(menu_t *menu);

/**
 * @brief Update main menu state from hover and click input.
 *
 * @param menu Menu state to mutate.
 * @param hovered_option Option currently under the cursor.
 * @param clicked Whether the user clicked this frame.
 * @return Action requested by the menu.
 */
MenuAction menu_update(menu_t *menu, MenuOption hovered_option, bool clicked);

/**
 * @brief Handle a character keyboard input for numeric seed typing.
 *
 * @param menu Menu state to mutate.
 * @param c Key character received.
 */
void menu_handle_key(menu_t *menu, char c);

#endif /* _MENU_STATE_H_ */
