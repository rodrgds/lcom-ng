#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "Controller/Timer/timer.h"
#include "Controller/Timer/i8254.h"
#include "Controller/Video/graphic.h"
#include "Controller/Keyboard/keyboard.h"
#include "Controller/Mouse/mouse.h"
#include "Controller/RTC//rtc.h"
#include "configs.h"
#include "Viewer/view.h"
#include "Model/game.h"
#ifndef SHARED_STATE_H
#define SHARED_STATE_H

/**
 * @brief Enumeration representing the program states.
 * 
 * This enum defines the different states the program can be in.
 */

typedef enum {
    RUNNING,  /**< Program is running */
    END       /**< Program has ended */
} ProgramState;

/**
 * @brief Enumeration representing the game states.
 * 
 * This enum defines the different states the game can be in.
 */

typedef enum {
    PLAY,  /**< Game is in play mode */
    STOP   /**< Game is stopped */
} GameState;

/**
 * @brief Enumeration representing the menu states.
 * 
 * This enum defines the different states the menu can be in.
 */

typedef enum {
    STARTMENU,       /**< Start menu state */
    CONTROLLERMENU,  /**< Controller menu state */
    GAMEOVER,        /**< Game over menu state */
    WINMENU,         /**< Win menu state */
    GAMEMENU         /**< In-game menu state */
} MenuState;

/**
 * @brief Handles timer events and updates the game state accordingly.
 */

void timer_state();

/**
 * @brief Handles keyboard input and updates the game state accordingly.
 */

void keyboard_state();

/**
 * @brief Handles mouse input and updates the game state accordingly.
 */

void mouse_state();

/**
 * @brief Handles the real-time clock (RTC) state.
 * 
 * Updates the game timer every `FREQUENCY` ticks when the game is in the PLAY state.
 * Also updates the displayed time in seconds, minutes, and hours.
 */

void rtc_state();

/**
 * @brief Handles keyboard input for cell selection and interaction.
 * 
 * Moves the selection cursor based on arrow key inputs.
 * Allows the player to reveal cells using the spacebar and flag cells using the 'F' key.
 */

void cell_state_keyboard();

/**
 * @brief Handles mouse input for cell selection and interaction.
 * 
 * Determines the selected cell based on mouse position and updates its state accordingly.
 * Handles left and right button clicks for revealing cells and flagging cells, respectively.
 */

void cell_state_mouse();

#endif //SHARED_STATE_H
