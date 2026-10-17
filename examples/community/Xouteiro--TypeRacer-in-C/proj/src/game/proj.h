#ifndef PROJ_H
#define PROJ_H





#include <lcom/lcf.h>
#include "controller/timer/timer.h"
#include "controller/video/graphics.h"
#include "controller/keyboard_mouse/keyboard.h"
#include "controller/keyboard_mouse/mouse.h"
#include "controller/rtc/rtc.h"
#include "states_design/states.h"
#include "states_design/design.h"
#include "game/game.h"

/**
 * @brief Initializes everything needed for the program.
 * 
 * @return int 0 upon success, non-zero otherwise.
 */
int setup(void);

/**
 * @brief Cleans up the program.
 * This function **must** be called before exiting the program.
 * 
 * @return int 0 upon success, non-zero otherwise.
 */
int proj_cleanup(void);


#endif
