#ifndef _PAUSE_H
#define _PAUSE_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include "kbcKeys.h"
#include <xpms/resume.xpm>
#include <xpms/exitPause.xpm>
#include "devices/graphics.h"

/**
 * @brief calls function that draws highlighted option if there is any
 * 
 */
void PauseTimerHandler();
/**
 * @brief is responsible for keeping track of the selected options and changes  gamestate if it is chosen
 * 
 * @param k - key pressed by user
 */
void PauseHandlerKBC(enum KEY k);
/**
 * @brief draws highlighted option
 * 
 * @param opt - option selected in the menu
 */
void drawPause(int opt);


#endif
