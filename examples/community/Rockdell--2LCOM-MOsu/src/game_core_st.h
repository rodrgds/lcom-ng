
#ifndef __GAME_CORE_ST_H
#define __GAME_CORE_ST_H

#include <stdio.h>

#include "circle.h"
#include "pixmap.h"
#include "i8042.h"

#include "cursor.h"
#include "video_gr.h"
#include "keyboard.h"
#include "rtc.h"

#include <minix/drivers.h>

/** @defgroup st State machine
 *  @{
 *
 *  State machine related functions
 */

/** Possible states of the state machine. */
typedef enum {
	MENU1,			/**< First menu state. */
	MENU2,			/**< Second menu state. */
	IN_GAME_EASY,	/**< In-game (easy mode) state. */
	IN_GAME_HARD,	/**< In-game (hard mode) state. */
	END_GAME		/**< End of the game state. */
} state_t;

/** Possible type of events. */
typedef enum {
	ACTIVATE,		/**< Mouse is over option. */
	LEFT_B,			/**< Left button (left click). */
	START_EASY,		/**< Start game (easy mode). */
	START_HARD,		/**< Start game (hard mode). */
	KEY_PRESS,		/**< Key pressed. */
	MISSED,			/**< Circle missed. */
	SAVE			/**< Save score. */
} ev_type_t;

/** Event is an "object" that contains all the information to
 *  characterize for the state machine to process.  */
typedef struct {
	ev_type_t type;			/**< Type of event. */
	unsigned long key_code;	/**< Key code. */
	Circle* circ;			/**< Pointer to circle. */
	Cursor* mouse;			/**< Pointer to cursor. */
} event_t;

/** Player is an "object" that contains all the information about the player,
 * 	including a score, multiplier, max multiplier and life.
 */
typedef struct {
	unsigned int score;		/**< Score. */
	unsigned int multi;		/**< Multiplier. */
	unsigned int max_multi;	/**< Max multiplier. */
	int life;				/**< Life. */
} player_t;

/**
 *  @brief Start state machine.
 *  @param file Path to scores' file.
 */
void linkStart(char *file);

/**
 *  @brief Checks how long has the game has been running.
 *  @return Returns the time, in minutes, the game has been running.
 */
unsigned int timePassed();

/**
 *  @brief Gets the current state of the state machine.
 *  @return Returns the current state of the state machine.
 */
state_t getCurrentState();

/**
 *  @brief Sets the current state of the state machine.
 *  @param s State to change to.
 */
void setCurrentState(state_t s);

/**
 *  @brief Gets the player.
 *  @return Returns the player.
 */
player_t getPlayer();

/**
 *  @brief State machine handler.
 *
 *  Handles the events and updates the current state.
 *  @param evt Event to handle.
 */
void mosu_handler(event_t *evt);

/** @} end of state machine */

#endif
