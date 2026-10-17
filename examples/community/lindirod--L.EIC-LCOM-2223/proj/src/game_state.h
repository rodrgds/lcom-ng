/**
 * @file game_state
 * @author Diogo Pintado, Gabriel Machado Jr., José Costa, Linda Rodrigues
 * 
 *
 * @brief Defines the GameState enum.
 * 
 */
#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_


/**
 * @brief Enumeration of the different states the game can be in.
 * 
 */
typedef enum {
    GAME,            ///< The main game state.
    INSTRUCTIONS,    ///< The state for displaying instructions.
    LEADERBOARD,     ///< The state for displaying the leaderboard.
    MENU,           ///< The state for displaying the main menu.
    GAME_OVER       ///< The state for displaying the game_over.      
} GameState;

#endif // _GAME_STATE_H_
