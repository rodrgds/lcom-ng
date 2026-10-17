#ifndef _GAME_H_
#define _GAME_H_

#include "../cursor.h"

/**
 * @defgroup Game Game 
 * 
 * @brief Module responsible for game logic
 * 
 * @{
 * 
 * @defgroup game game
 * 
 * @brief Game State Machine
 * 
 * @{
*/

/**
 * @brief Game States
 * 
 * This enum represents all the possible game states
*/
typedef enum {
    /** @brief Main Menu*/
    MAIN_MENU,
    /** @brief Multiplayer Menu*/
    MULTIPLAYER_MENU,
    /**
     * @brief State in which the player has chosen to guess and is waiting for the game to start
     * 
     * This is the state where the player arrives if he clicks GUESS in the multiplayer menu.
     * The player will remain in this state until they choose to exit or the other player chooses the DRAW option in the multiplayer menu.
    */
    WAITING_GUESSING,
    /**
     * @brief Guessing mode
     * 
     * In this state, the player receives, via serial port, what is drawn by the other player, and attempts to guess what they are drawing.
    */
    GUESSING,
    /**
     * @brief State in which the player has chosen to draw and is waiting for the game to start
     * 
     * This is the state where the player arrives if he clicks START in the multiplayer menu.
     * The player will remain in this state until they choose to exit or the other player chooses the DRAW option in the multiplayer menu.
    */
    WAITING_DRAWING,
    /**
     * @brief Drawing mode
     * 
     * In this state, the player is given a word and attempts to draw it.
    */
    DRAWING,
    /**
     * @brief Singleplayer mode
     * 
     * In this state, the player can freely draw without needing a friend to join the game.
    */
    DRAWING_SINGLE_PLAYER,
    /**
     * @brief Game over
     * 
     * The timer has either run out, in which case both players lose, or the player guessing got the word right, resulting in a win for both.
    */
    FINISH
} state;

/**
 * @brief Device Interrupts
 * 
 * This enum represents all the possible device interrupts that can be received. 
 * They are received by the proj_main_loop and sent to the game_state_handler if needed.
*/
typedef enum {
    TIMER,
    KEYBOARD,
    MOUSE,
    RTC,
    SERIAL_PORT
} interrupt;

/**
 * @brief Data Receival Statess
 * 
 * These help in identifying the type of data that is being received through the serial port and processing it.
*/
typedef enum {
    UNKNOWN_DATA,
    PENCIL_SIZE_DATA,
    PENCIL_COLOR_DATA,
    MOUSE_DATA,
    CORRECT_GUESS_DATA,
    WRONG_GUESS_DATA
} sp_received_data;

/**
 * @brief Game State Machine
 * 
 * Function responsible for the game state machine. Processes the interrupts received to change state and what is displayed on screen. 
 * 
 * @param interrupt The origin of a received interrupt
 * @param cursor Pointer to the cursor
 * 
 * @return 0 on success, 'e' to exit the game, any other value on error.
*/
int game_state_handler(interrupt interrupt, Cursor* cursor);

/**
 *  }@
 * }@
*/

#endif
