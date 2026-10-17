#pragma once

/** @defgroup ModeState ModeState
 * @{
 * Functions for Mode State
 */

#include "bitmap.h"
#include "rectangle.h"

typedef enum {
	NORMAL_MOUSE, NORMAL_KEYBOARD, NIGHT_MOUSE, NIGHT_KEYBOARD, EXIT
} ModeAction;

typedef struct {
	int done; // specifies if the state is done
	Bitmap* background; // specifies the background image in Mode menu
	Bitmap* NormalMouse; // specifies the button image NormalMouse
	Bitmap* onNormalMouse;  // specifies the button image NormalMouse with mouse inside
	Bitmap* NormalKeyboard; // specifies the button image NormalKeyboard
	Bitmap* onNormalKeyboard;  // specifies the button image NormalKeyboard with mouse inside
	Bitmap* NightMouse; // specifies the button image NightMouse
	Bitmap* onNightMouse;  // specifies the button image NightMouse with mouse inside
	Bitmap* NightKeyboard; // specifies the button image NightKeyboard
	Bitmap* onNightKeyboard;  // specifies the button image NightKeyboard with mouse inside
	int mouseOnNormalMouse; // specifies if the mouse is inside of NormalMouse button
	Rectangle* NormalMouseButton; // specifies the NormalMouse rectangle
	int mouseOnNormalKeyboard; // specifies if the mouse is inside of NormalKeyboard button
	Rectangle* NormalKeyboardButton; // specifies the NormalKeyboard rectangle
	int mouseOnNightMouse; // specifies if the mouse is inside of NightMouse button
	Rectangle* NightMouseButton; // specifies the NightMouse rectangle
	int mouseOnNightKeyboard; // specifies if the mouse is inside of NightKeyboard button
	Rectangle* NightKeyboardButton; // specifies the NightKeyboard rectangle

	int action;  // specifies what button was pressed
} ModeState;

/**
 * @brief Creates a new Mode State
 *
 * @return Non NULL pointer to the state
 */
ModeState* newModeState();
/**
 * @brief Updates a Mode State
 *
 * @param state Pointer to MainMenuState
 * @param scancode Scancode read by the Keyboard
 */
void updateModeState(ModeState* state, unsigned long scancode);
/**
 * @brief Draws a Mode State
 *
 * @param state Pointer to ModeState
 */
void drawModeState(ModeState* state);
/**
 * @brief Deletes a Mode State
 *
 * @param state Pointer to ModeState
 */
void deleteModeState(ModeState* state);
