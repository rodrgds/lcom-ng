/**
 * @file menu.h
 * @brief Main menu system -- screens, options, rendering, and input handling.
 * @ingroup group_ui
 *
 * The menu module manages the main menu, instructions screen, and credits
 * screen. It handles mouse movement / clicks, keyboard navigation, RTC
 * date-time display, and returns menu_result_t actions that drive state
 * transitions in the app layer.
 */

#ifndef PROJ_MENU_H
#define PROJ_MENU_H

#include <stdbool.h>
#include <stdint.h>

#include "multiplayer.h"
#include "overlay_menu.h"
#include "tower.h"

struct resources;

/**
 * Menu option IDs for main menu
 */
typedef enum {
  MENU_OPTION_PLAY_SINGLE = 0,
  MENU_OPTION_PLAY_MULTI = 1,
  MENU_OPTION_INSTRUCTIONS = 2,
  MENU_OPTION_CREDITS = 3,
  MENU_OPTION_EXIT = 4,
  MENU_OPTION_COUNT = 5
} menu_option_t;

/**
 * Menu screen types
 */
typedef enum {
  MENU_SCREEN_MAIN = 0,
  MENU_SCREEN_INSTRUCTIONS = 1,
  MENU_SCREEN_CREDITS = 2,
  MENU_SCREEN_COUNT = 3
} menu_screen_t;

/**
 * Menu result indicating which action to take
 */
typedef enum {
  MENU_RESULT_NONE = 0,
  MENU_RESULT_PLAY_SINGLE,
  MENU_RESULT_PLAY_MULTI,
  MENU_RESULT_EXIT,
  MENU_RESULT_BACK,
  MENU_RESULT_INSTRUCTIONS,
  MENU_RESULT_CREDITS
} menu_result_t;

/**
 * RTC time structure for display
 */
typedef struct {
  uint8_t day;
  uint8_t month;
  uint8_t year;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
} menu_datetime_t;

/**
 * Main menu state structure
 */
typedef struct {
  menu_screen_t current_screen;
  int16_t selected_option;
  int16_t hovered_option;
  int16_t cursor_x;
  int16_t cursor_y;
  menu_datetime_t datetime;
  uint32_t last_rtc_update;
  const struct resources *resources;
  uint32_t frame_count;
  uint16_t render_hz;
  uint16_t timer_hz;
  vg_sprite_t background_sprite;
  vg_sprite_t panel_logo_sprite;
  vg_sprite_t instructions_sprite;
  vg_sprite_t credits_sprite;
  bool background_cached;
  bool panel_logo_cached;
  bool instructions_cached;
  bool credits_cached;
} menu_t;

/**
 * Initialize the main menu.
 * Returns 0 on success, non-zero on failure.
 */
int menu_init(menu_t *menu, const struct resources *resources, uint16_t timer_hz, uint16_t render_hz);

/**
 * Shutdown and cleanup the menu.
 */
void menu_shutdown(menu_t *menu);

/**
 * Update menu state based on elapsed time.
 * Returns a menu_result_t indicating what action to take.
 */
menu_result_t menu_update(menu_t *menu, uint32_t current_tick);

/**
 * Render the menu to the back buffer.
 * Returns 0 on success, non-zero on failure.
 */
int menu_render(menu_t *menu);
/**
 * @brief Render the multiplayer connection waiting screen.
 *
 * Displays a status panel while waiting for a peer to connect.
 * If no status text is provided, a default waiting message is shown.
 *
 * @param menu        Menu instance.
 * @param status_text Status message to display, or NULL for the default text.
 *
 * @return 0 on success, non-zero on failure.
 */
int menu_render_multiplayer_wait(menu_t *menu, const char *status_text);
/**
 * @brief Render the multiplayer role selection screen.
 *
 * Displays the attacker/defender role selection interface,
 * current local and peer roles, and an optional status message.
 *
 * @param menu         Menu instance.
 * @param role_menu    Overlay menu used for role selection.
 * @param local_role   Currently selected local role.
 * @param peer_role    Role selected by the connected peer.
 * @param status_text  Status message to display, or NULL for the default text.
 *
 * @return 0 on success, non-zero on failure.
 */
int menu_render_multiplayer_roles(menu_t *menu, const overlay_menu_t *role_menu,
                                  multiplayer_role_t local_role,
                                  multiplayer_role_t peer_role,
                                  const char *status_text);

/**
 * Handle keyboard input (scancode).
 * Returns a menu_result_t indicating what action to take.
 */
menu_result_t menu_handle_keyboard(menu_t *menu, bool make, uint8_t scancode);

/**
 * Handle mouse movement.
 * dx: horizontal movement (positive = right)
 * dy: vertical movement (positive = down)
 */
void menu_handle_mouse_movement(menu_t *menu, int16_t dx, int16_t dy);

/**
 * Handle mouse click (left button).
 * Returns a menu_result_t indicating what action to take.
 */
menu_result_t menu_handle_mouse_click(menu_t *menu);

#endif
