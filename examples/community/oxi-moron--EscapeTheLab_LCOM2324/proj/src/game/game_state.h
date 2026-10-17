#ifndef G6_GAME_STATE_H
#define G6_GAME_STATE_H

#include <lcom/lcf.h>
#include "player.h"
#include "cursor.h"
#include "graphics.h"
#include "clock.h"
#include "intercom.h"
#include "game_macros.h"

enum game_state_t {MENU, PLAYER_SELECT, GAME, PAUSE, INTERCOM, DEFEAT, VICTORY};

static enum game_state_t game_state = MENU;
static uint8_t map_no = START_MAP;
static uint8_t player_no = 0;

/**
 * @brief Sets up game (loads initial map, initializes player and graphics display).
 * @return 0 if successful, non-zero otherwise.
 */
int game_setup();

/**
 * @brief Draws the current game frame.
 * @return 0 if successful, non-zero otherwise.
 */
int state_draw_frame();

/**
 * @brief Handles the in-game logic of pressing a key.
 * @param scancode Scancode of the pressed key.
 * @return 0 if successful, non-zero otherwise.
 */
int state_kbd_event(uint8_t scancode);

/**
 * @brief Handles the in-game logic of a mouse movement.
 * @param pp Packet received by the mouse.
 * @return 0 if successful, non-zero otherwise.
 */
int state_mouse_event(struct packet pp);

/**
 * @brief Handles the in-game logic of a RTC alarm.
 * @return 0 if successful, non-zero otherwise.
 */
int (state_rtc_event) ();

#endif //G6_GAME_STATE_H
