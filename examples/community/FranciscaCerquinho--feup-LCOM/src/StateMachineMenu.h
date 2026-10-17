#ifndef __STATEMACHINEMENU_H
#define __STATEMACHINEMENU_H


#include "timer.h"
#include "mouse.h"
#include "Button.h"
#include "Bitmap.h"
#include "IRQ.h"

/**
 * Struct from start and exit the game
 */
typedef enum
{
StartGame, ExitGame
} ActionMenu;


/**
 * Struct from the buttons of the menu
 */
typedef struct
{
	Bitmap *menu;

	int Start;
	int ExitGame;
	Button *Play, *ClickedStart;
	Button *Exit, *ClickedExit;


} StateMenu;

/**
 * Loads the menu and the buttons
 * @return the created struct
 */
StateMenu *newSateMenu();

/**
 * Handles the interrupts during the menu
 * @param *map buffer
 * @param *irqs irq's
 * @return value of the next state
 */
int Menu(char *map, IRQ *irqs);

#endif
