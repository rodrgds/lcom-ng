#ifndef __DUELNIX_H
#define __DUELNIX_H

#include "bitmap.h"
#include "timer.h"

/** @defgroup duelnix duelnix
 * @{
 * 	Duelnix functions
 */

/* Represents the states */
typedef enum {
	MAIN_MENU_STATE, PRACTICE_STATE, GAME_STATE, MULTIPLAYER
} State;

/*! Represents the program */
typedef struct {
	int IRQ_SET_KBD;
	int IRQ_SET_TIMER;
	int IRQ_SET_MOUSE;
	int IRQ_SET_RTC;

	int done, draw;
	unsigned long scancode;
	Timer* timer;

	State currentState;
	void* state;

} Duelnix;

/**
 * @brief Creates a new program. Initializing devices and variables ; allocates it
 * @return pointer to the game
 */
Duelnix* initDuelnix();

/**
 * @brief Hardware cycle which updates all devices. Variables and state updated
 * @param duelnix pointer to the game
 */
void updateDuelnix(Duelnix* duelnix);

/**
 * @brief Draws the program in wichever state it is in
 * @param duelnix pointer to the game
 */
void drawDuelnix(Duelnix* duelnix);

/**
 * @brief frees memory and ends the program
 * @param duelnix pointer to the game
 */
void endDuelnix(Duelnix* duelnix);

/**
 * @brief Changes the state pointer of Duelnix to the newState enum type
 * @param game pointer to the game
 * @param newState new state to be atributed to the game
 */
void changeState(Duelnix* game, State newState);

/**
 * @brief Checks if the current state is over and changes state accordingly(deleting the current one)
 * @param game pointer to the game
 */
void checkIfStateIsDone(Duelnix* game);

/**
 * @brief frees resources form memory deleting the current state when it is over
 * @param game pointer to the game
 */
void deleteCurrentState(Duelnix* game);

/**@}*/

#endif /* _DUELNIX_H */
