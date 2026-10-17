#ifndef _CURSORCONTROLLER_H_
#define _CURSORCONTROLLER_H_

#include "src/include.h"
#include "src/Controller/Game/GameState.h"
#include "src/Model/Elements/Cursor.h"
#include "src/Controller/Game/MapController.h"
#include "src/Controller/Game/MenusController.h"
#include "src/Model/Elements/Backpack.h"

/**
 * @file CursorController.h
 * @brief Declares functions and data structures for handling cursor movement and interaction.
 *
 * Handles the player's mouse cursor, including movement logic based on mouse input,
 * collision checks with elements, and button press effects depending on the current game state.
 */

typedef struct{
   size_t idx;
   bool collision;
} CursorController;

extern GameState gamestate;
void Update_Mouse(Cursor *c, GameState gamestate);
void MoveCursor(Cursor *c, mouse_reading_t mouse_reading);
void ButtonPressed(Cursor *c, mouse_reading_t mouse_reading);


#endif
