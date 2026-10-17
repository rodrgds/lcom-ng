#ifndef _LCOM_MODEL_H_
#define _LCOM_MODEL_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "controller/KBC/KBC.h"
#include "controller/mouse/mouse.h"
#include "controller/video/graphics.h"
#include "sprite.h"

/** @defgroup model model
 * @{
 *
 * Functions for using the model
 */

/**
 * @brief Enumeration representing the system state.
 *
 * The SystemState enumeration represents the possible states of a system. It contains the following values:
 * - RUNNING: Represents the running state of the system.
 * - EXIT: Represents the exit state of the system.
 */

typedef enum {
    RUNNING,
    EXIT,
} SystemState;

/**
 * @brief Enumeration representing the menu state.
 *
 * The MenuState enumeration represents the possible states of a menu. It contains the following values:
 * - START: Represents the start of the menu.
 * - GAME: Represents the game state.
 * - INSTRUCTIONS: Represents the instructions state.
 * - END: Represents the end of the menu.
 */

typedef enum {
    START,
    GAME,
    INSTRUCTIONS,
    END
} MenuState;

/**
 * @brief Enumeration representing the tool state.
 *
 * The ToolState enumeration represents the possible states of a drawing tool. It contains the following values:
 * - PENCIL: Represents the pencil tool state.
 * - BUCKET: Represents the bucket tool state.
 * - RECTANGLE: Represents the rectangle tool state.
 */

typedef enum {
    PENCIL,
    BUCKET,
    RECTANGLE,
} ToolState;


SystemState is_playing;
MenuState game_state;
ToolState tool;

/*

typedef enum {
    MENU,
    PAINTING,
    EXITED,
} GameState

*/

void setupsprites();
void destroy_sprites();

#endif
