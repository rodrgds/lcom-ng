/**
 * This module contains all functions that handle the character in battle mode
 * Jorge Vale: 10%
 * Telmo Barros: 90%
 * Relative Weight: 9%
 */

#ifndef __CHARACTER_H
#define __CHARACTER_H

#include <stdio.h>

/** @defgroup Character Character
 * @{
 * Functions for manipulating character
 */

#define CHAR_INIT_X						500.0
#define CHAR_INIT_Y						400.0

#define CHARACTER_LENGTH				30

#define GRAVITY_SPEED					0.16
#define CHAR_X_SPEED					2
#define CHAR_JUMP_SPEED					7

#define CHAR_MOVE_FRAME_DELAY			10
#define NUMBER_OF_SPRITES				4

/** @name States of the character */
/** @{ */
typedef enum {
	CHAR_JUMP,
	CHAR_FALL,
	CHAR_MOVE_RIGHT,
	CHAR_MOVE_LEFT,
	STOPPED,
	GAMEOVER
} character_state;
/** @} end of States of the character */


/** @name Events of keyboard related to character movement */
/** @{ */
typedef enum kbd_character_event {
	A_DOWN, A_UP,
	W_DOWN, W_UP,
	S_DOWN, S_UP,
	D_DOWN, D_UP,
	NO_KEY
} kbd_character_event;
/** @} end of Events of keyboard related to character movement */


/** @name Events of timer related to character changing sprite */
/** @{ */
typedef enum timer_character_event {
	MOVE_FRAME_TICK,
	NOTICK
} timer_character_event;
/** @} end of Events of timer related to character changing sprite */

/** @name Character structure */
/** @{
 *
 * Character structure
 */
typedef struct {
	float x,y;	// character position
	float xspeed,yspeed;	// character speed
	unsigned int width, height;	//character dimensions
	uint16_t * current;	// pointer that stores current character sprite
	uint16_t * left;	// pointer to image that contains all sprites related to character left movement
	uint16_t * right;	// pointer to image that contains all sprites related to character right movement
	character_state state;	// current character state
	character_state last_state;	// last character state
	kbd_character_event kbd_event;	// current character keyboard event
	timer_character_event timer_event;	// current character timer event
	unsigned int falling, jumping;	// flags that stores state of character related to falling and jumping respectively
	unsigned int sprite_counting;	// sprite number to show and give the animated look to movement
} Character;
/** @} end of Character structure */

/**
 * @brief Character constructor
 *
 * @return A pointer to the character
 */
Character * new_character();

/**
 * @brief Updates character state given the current events
 *
 * @param character Character to change
 */
void update_character_state(Character * character);

/**
 * @brief Updates character position based on current state
 *
 * @param character Character to change
 */
void update_character(Character * character);

/**
 * @brief Draws the character in the video buffer
 *
 * @param character Character to draw
 */
void draw_character(Character * character);

/**
 * @brief Deletes the character by releasing the allocated memory
 *
 * @param character Character to destroy
 */
void delete_character(Character * character);

/**@}*/

#endif /* __GAME_H */
