#ifndef __TEST5_H
#define __TEST5_H

/** @defgroup mouse-maze mouse-maze
 * @{
 * Functions for manipulate the program
 */

#include "timer.h"
#include "rtc.h"

typedef enum{
	MAIN_MENU_STATE, GAME_STATE, MODE_STATE, WIN_STATE, LOSE_STATE
} State;

typedef struct {
	int irq_set_kb; // specifies the keyboard IRQ
	int irq_set_mouse; // specifies the mouse IRQ
	int irq_set_timer; // specifies the timer IRQ
	int irq_set_rtc; // specifies the rtc IRQ
	Timer* timer; // specifies a timer pointer
	int validation; // specifies if the program is done
	unsigned long clean; // specifies a variable to clean a mouse buffer
	unsigned long scan_code; // specifies a keyboard scancode
	State currentState; // specifies the current state
	void* state; // specifies a pointer to the current state
} MouseMaze;


/**
 * @brief Initialize the program
 *
 * @return Non NULL pointer to program
 */
MouseMaze* startMouseMaze();
/**
 * @brief Update the program
 *
 * @param maze Pointer to program
 */
void updateMouseMaze(MouseMaze* maze);
/**
 * @brief Finish the program
 *
 * @param maze Pointer to program
 */
void stopMouseMaze(MouseMaze* maze);

/**
 * @brief Change a State
 *
 * @param maze Pointer to program
 * @param newState New program state
 */
void changeState(MouseMaze* maze, State newState);

/**
 * @brief Check if a state is done
 *
 * @param maze Pointer to program
 */
void checkIfStateIsDone(MouseMaze* maze);
/**
 * @brief Delete current state
 *
 * @param maze Pointer to program
 */
void deleteCurrentState(MouseMaze* game);


/**
 * @brief Handler to rtc chronometer
 *
 * @param maze Pointer to program
 */
void handler_rtc(MouseMaze* maze);


#endif /* __TEST5_H */
