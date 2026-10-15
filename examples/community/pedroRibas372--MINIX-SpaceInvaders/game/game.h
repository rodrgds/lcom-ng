#ifndef _GAME_H_
#define _GAME_H_



/**
 * @brief Represents the different states the game can be in.
 *
 * This enumeration is used to track the current state of the game,
 * such as whether the player is in the menu, playing, has won, lost, or exited.
 */
typedef enum {
  /**
   * @brief The game is in the menus.
   */
  MENU_MODE,

  /**
   * @brief The player is inside a level
   */
  PLAY_MODE,

  /**
   * @brief Exit Game.
   */
  EXIT_MODE,

  /**
   * @brief The player has lost the game, this is just to write the "You Lost" message, when you leave the screen it switches to MENU_MODE
   */
  LOST_MODE,

  /**
   * @brief The player has won the game, this is just to write the "You Win" message, when you leave the screen it switches to MENU_MODE
   */
  WON_MODE
} GameState;

/**
 * @brief Loads all necessary data to start a level.
 *
 * This function initializes the hero and the enemies into their respective places so the game is able to run
 */
void loadGame();

/**
 * @brief Starts the main game logic.
 *
 * This function begins the core execution of the game. Ends when state reaches EXIT_MODE.
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int gameStart();

/**
 * @brief Handles game-related interrupts
 * 
 * This is the main PLAY_MODE loop.
 *
 * This function manages interrupts during the game execution (TIMER and Keyboard)
 * 
 * The game is updated at each timer int and timer freq is 60.
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int controllGameInterrupts();

/**
 * @brief Free mallocs and resets game variables so that the player can restart the level without leaving.
 */
void destroygame();


#endif
