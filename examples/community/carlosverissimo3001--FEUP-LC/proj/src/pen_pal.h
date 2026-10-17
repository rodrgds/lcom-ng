#ifndef _PEN_PAL_H
#define _PEN_PAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CodeToCharacter.h"
#include "Color.h"
#include "graphics.h"
#include "keyboard.h"
#include "macros.h"
#include "mouse.h"
#include "number.h"
#include "Sprite.h"
#include "words.h"
#include "rtc.h"

#include "img/xpms/prompts/player1_nickname.xpm"
#include "img/xpms/prompts/player2_nickname.xpm"
#include "img/xpms/prompts/word_selection.xpm"
#include "img/xpms/prompts/play_again_exit_game.xpm"
#include "img/xpms/prompts/guess.xpm"
#include "img/xpms/screens/light_screen.xpm"
#include "img/xpms/screens/dark_screen.xpm"
#include "img/xpms/cross.xpm"


/** @defgroup pen_pal
 * @{
 *
 *
 *
 * Top level functions developped for the game, includes main loop
 */

/** @defgroup game_states
 * @{
 *
 * Enumerable containing all possible game states
 */
typedef enum {
    GETTING_NICKNAME_1,
    GETTING_NICKNAME_2,
    CHOOSING_WORD,
    PLAYER1_DRAWING,
    ANALYSE_DRAWING,
    PLAYER2_GUESSING,
    CORRECT_GUESS,          // User will then be prompted with the option to play again or exit
    WRONG_GUESS,
    OUT_OF_GUESSES,         // User will then be prompted with the option to play again or exit
    PLAY_AGAIN,
    EXIT_GAME
} GameState;

/**
 * @brief Game main loop
 * @return Return 0 upon success, 1 otherwise
 */
int (play_game)();

/**
 * @brief Creates all the elements in the game
 *
 */
void (start_xpms)();

/**
 * @brief Deletes all the elements in the game
 *
 */
void (delete_elements)();

/**
 * @brief Draws the initial elements onto the screen
 *
 */
void (draw_initial_elements)();

/**
 * @brief Handles the timer interruption, according to the state of the game
 *
 */
void (timer_event_handler)();

/**
 * @brief Handles the timer interruption, according to the state of the keyboard
 *
 */
void (keyboard_event_handler)();

/**
 * @brief Handles the timer interruption, according to the state of the mouse
 *
 */
void (mouse_event_handler)();

/**
 * @brief Displays the state of the game into the sidebar
 * @param state Current state of the game
 */
void (display_state)(GameState state);

/**
 * @brief Generates 3 randoms words from a dictionary
 *
 */
void(generate_words)();

/**
 * @brief If the user wants to play again, some variables need to be resetted
 *
 */
void (prepare_restart)();

/**
 * @brief Changes the selected pixel width
 * @param index Index of the ticked checkbox
 */
void (update_width)(int index);

/**
 * @brief Helper function to swap contents of two strings
 *
 */
void (switch_nicknames)();

/**
 * @brief Display the current game number
 *
 */
void (draw_game_number)();



/** @}*/

#endif /* _PEN_PAL_H_ */

