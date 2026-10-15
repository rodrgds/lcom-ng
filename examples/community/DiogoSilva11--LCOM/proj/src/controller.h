#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <lcom/lcf.h>

#include "timer_info.h"
#include "kbd.h"
#include "vbe_func.h"
#include "mouse.h"

#include "info.h"
#include "sprites.h"

/** @defgroup controller controller
 * @{
 * 
 * Functions for initialization and termination of devices
 */

/**
 * @brief Execute the initialization actions for all devices
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (init)();

/**
 * @brief Execute the termination actions for all devices
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (leave)();

/**@}*/

#endif /* _CONTROLLER_H */
