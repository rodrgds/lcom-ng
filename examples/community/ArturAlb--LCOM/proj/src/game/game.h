/**
 * @file game.h
 *
 */

#ifndef _LCOM_GAME_H_
#define _LCOM_GAME_H_

/* DEVICES */
#include "../devices/KBC/keyboard.h"
#include "../devices/video_card/video.h"
#include "../devices/KBC/mouse.h"
#include "../devices/timer/timer.h"

/* MODELS */
#include "models/object.h"

/* VIEWS */
#include "views/canvas.h"

/* STATES */
#include "states/gamestate.h"

typedef enum {MENU, GAME, QUIT} STATE;

#define WAIT 5

/**
 * @brief initializes all aspects of the game, including allocating memory for its objects
 * @return zero on success, non-zero otherwise
 */
int (game_start)();

/**
 * @brief executes the gameplay loop
 * @return zero on success, non-zero otherwise 
 */
int (game_loop)();

/**
 * @brief terminates the game, ensuring all allocated memory is freed
 * @return zero on success, non-zero otherwise 
 */
int (game_stop)();

#endif // _LCOM_GAME_H_
