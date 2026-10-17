#ifndef __MARIOEVENT_H
#define __MARIOEVENT_H

/** @defgroup MarioEvent MarioEvent
 * @{
 * Functions related to the MarioEvent methods
 */


#include "Bitmap.h"
#include "Mario.h"
#include "kbd.h"
#include "timer.h"
#include "vbe.h"
#include "video_gr.h"
#include "i8254.h"
#include "mouse.h"
#include "Background.h"
#include "Pipe.h"
#include "IRQ.h"


typedef struct
{
	int irq_set_timer, irq_set_mouse, irq_set_kbd;
	unsigned long BreakCode;

}MarioGame;

/**
 * Initializes the MarioGame struct
 * @return pointer to the MarioGame struct initialized
 */
MarioGame* StartMarioGame();

/**
 * Runs the main Game while there is no colision, when mario colides with a pipe the game ends
 * @param map pointer to the video_map
 * @param irqs pointer to the IRQ given
 * @return the number of pipes passed
 */
int UpdateMarioGame(char *map,IRQ *irqs);
/**
 * Free the allocated memory of the MarioGame
 * @param newMarioG pointer to the MarioGame struct
 */
void DestroyGame(MarioGame *newMarioG);

/** @} */

#endif
