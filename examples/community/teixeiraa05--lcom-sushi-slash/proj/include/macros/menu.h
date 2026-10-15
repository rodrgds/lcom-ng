#ifndef _MACROS_MENU_H_
#define _MACROS_MENU_H_

#include "global.h"

/**
 * @file menu.h
 * @brief Defines shared menu layout and color constants.
 */

/** @brief Main menu button width in pixels. */
#define MENU_BUTTON_W 300

/** @brief Main menu button height in pixels. */
#define MENU_BUTTON_H 56

/** @brief Vertical gap between main menu buttons. */
#define MENU_BUTTON_GAP 14

/** @brief Main menu button left coordinate. */
#define MENU_BUTTON_X CENTER_X(MENU_BUTTON_W)

/** @brief Top coordinate of the first main menu button. */
#define MENU_BUTTON_Y (SCREEN_HEIGHT / 2)

/** @brief Main menu button default fill color. */
#define MENU_BUTTON_COLOR 0x303030

/** @brief Main menu button hover fill color. */
#define MENU_BUTTON_HOVER_COLOR 0x606060

/** @brief Main menu mascot left coordinate. */
#define MENU_MASCOT_X 560

/** @brief Main menu mascot top coordinate. */
#define MENU_MASCOT_Y 180

/** @brief Status bar left coordinate. */
#define MENU_STATUS_X 10

/** @brief Status bar right margin in pixels. */
#define MENU_STATUS_RIGHT_MARGIN 10

/** @brief Status bar baseline/top coordinate. */
#define MENU_STATUS_Y (SCREEN_HEIGHT - 18)

/** @brief Status bar text color. */
#define MENU_STATUS_COLOR 0xFFFFFF

/** @brief Pause panel width in pixels. */
#define PAUSE_PANEL_W 400

/** @brief Pause panel height in pixels. */
#define PAUSE_PANEL_H 300

/** @brief Pause panel left coordinate. */
#define PAUSE_PANEL_X CENTER_X(PAUSE_PANEL_W)

/** @brief Pause panel top coordinate. */
#define PAUSE_PANEL_Y CENTER_Y(PAUSE_PANEL_H)

/** @brief Pause menu button width in pixels. */
#define PAUSE_BUTTON_W 220

/** @brief Pause menu button height in pixels. */
#define PAUSE_BUTTON_H 42

/** @brief Vertical gap between pause menu buttons. */
#define PAUSE_BUTTON_GAP 12

/** @brief Number of pause menu buttons. */
#define PAUSE_BUTTON_COUNT 3

/** @brief Total vertical space occupied by pause menu buttons. */
#define PAUSE_BUTTONS_H                                                        \
  (PAUSE_BUTTON_COUNT * PAUSE_BUTTON_H +                                       \
   (PAUSE_BUTTON_COUNT - 1) * PAUSE_BUTTON_GAP)

/** @brief Pause menu button left coordinate. */
#define PAUSE_BUTTON_X CENTER_X(PAUSE_BUTTON_W)

/** @brief Top coordinate of the first pause menu button. */
#define PAUSE_BUTTON_Y (PAUSE_PANEL_Y + 120)

/** @brief Integer scale used for pause title text. */
#define PAUSE_TITLE_SCALE 3

/** @brief Pause title top coordinate. */
#define PAUSE_TITLE_Y (PAUSE_PANEL_Y + 34)

/** @brief Pause screen mascot left coordinate. */
#define PAUSE_MASCOT_X 530

/** @brief Pause screen mascot top coordinate. */
#define PAUSE_MASCOT_Y 200

/** @brief Integer scale used for game-over title text. */
#define GAME_OVER_TITLE_SCALE 4

/** @brief Game-over title top coordinate. */
#define GAME_OVER_TITLE_Y 70

/** @brief Integer scale used for game-over score text. */
#define GAME_OVER_SCORE_SCALE 2

/** @brief Game-over score top coordinate. */
#define GAME_OVER_SCORE_Y 165

/** @brief Game-over stats list top coordinate. */
#define GAME_OVER_STATS_Y 220

/** @brief Vertical gap between game-over stat lines. */
#define GAME_OVER_STATS_GAP 22

/** @brief Game-over stats text color. */
#define GAME_OVER_STATS_COLOR 0xFFFFFF

/** @brief Game-over mascot left coordinate. */
#define GAME_OVER_MASCOT_X CENTER_X(128)

/** @brief Game-over mascot top coordinate. */
#define GAME_OVER_MASCOT_Y 350

/** @brief Game-over button width in pixels. */
#define GAME_OVER_BUTTON_W 300

/** @brief Game-over button height in pixels. */
#define GAME_OVER_BUTTON_H 52

/** @brief Horizontal gap between game-over buttons. */
#define GAME_OVER_BUTTON_GAP 24

/** @brief Game-over left button left coordinate. */
#define GAME_OVER_BUTTON_LEFT_X CENTER_X(GAME_OVER_BUTTON_W * 2 + GAME_OVER_BUTTON_GAP)

/** @brief Game-over right button left coordinate. */
#define GAME_OVER_BUTTON_RIGHT_X (GAME_OVER_BUTTON_LEFT_X + GAME_OVER_BUTTON_W + GAME_OVER_BUTTON_GAP)

/** @brief Game-over button top coordinate. */
#define GAME_OVER_BUTTON_Y 520

#endif /* _MACROS_MENU_H_ */
