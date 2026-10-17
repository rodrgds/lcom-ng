#pragma once

#include "bitmap.h"
#include "timer.h"
#include "mouse.h"
#include "game.h"
#include "gameState.h"

/**
 * @struct PrisonBreaker
 * @brief This structure holds the information about the devices and the Game
 */
typedef struct {
	int IRQ_SET_KBD;
	int IRQ_SET_TIMER;
	int IRQ_SET_MOUSE;

	int draw;

	unsigned long scancode;
	Timer* timer;
	Mouse* mouse;

	GameState* gameState;

} PrisonBreaker;

/**
 * @brief Initializes the PrisonBreaker
 *
 * @return The New PrisonBreaker
 */
PrisonBreaker* initPrisonBreaker();

/**
 * @brief Updates the Game in function of an interrupt
 *
 * @param prisonBreaker The PrisonBreaker to be updated
 */
void updatePrisonBreaker(PrisonBreaker* prisonBreaker);

/**
 * @brief Displays the PrisonBreaker, either menu or game
 *
 * @param prisonBreaker The PrisonBreaker to be displayed
 */
void drawPrisonBreaker(PrisonBreaker* prisonBreaker);

/**
 * @brief Free the PrisonBreaker and all the resources used by it
 *
 * @param prisonBreaker The PrisonBreaker to be freed
 */
void stopPrisonBreaker(PrisonBreaker* prisonBreaker);

