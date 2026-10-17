#ifndef __GAME_H
#define __GAME_H

/** @file game.h
 * @brief file containing the declarations of the non static functions and the sctructures in game.c
 */

/** @defgroup game game
 * @{
 *
 * @brief functions for running the game
 */

#include "keyboard.h"
#include "mouse.h"
#include "timer.h"
#include "rtc.h"
#include "video_gr.h"
#include "highscore.h"
#include "menu.h"

/**@{
 * @brief enumeration of game states
 */
typedef enum {
	GAME_OVER,		/**< @brief when a game is over and should be scored */
	GAME_SCORE,		/**< @brief when a game has already been scored */
	GAME_EXIT,		/**< @brief to exit a game */
	GAME_PAUSE,		/**< @brief when a game is paused */
	GAME_PLAY,		/**< @brief when a game is being played */
	GAME_MENU		/**< @brief when a game is in one of the menus */
} State;
/** @}*/

/**@{
 * @brief Game State info
 */
typedef struct {
	unsigned short dealt;				/**< @brief number of cards that have been dealt */
	unsigned short display_cards[21];	/**< @brief cards that are currently displayed */
	unsigned short display;				/**< @brief number of cards that are currently displayed */
	unsigned short remove_cards[3];		/**< @brief cards to be removed from the game if a set is formed */
	unsigned short remove_card;			/**< @brief number of cards that have been flagged for removal */
	char remove;						/**< @brief indicator to remove cards if a set is formed */
	char frame;							/**< @brief indicator to add frame arround card to be removed */
	unsigned short add_cards[3];		/**< @brief cards to be added to the game if a set is formed or there are no sets */
	unsigned short extra_card;			/**< @brief extra card to handle flagging of cards for removal (two clicks in the same card unflag it) */
	char add;							/**< @brief indicator that 3 cards should be added to the display */
	char hint;							/**< @brief indicator that a hint of a set should be given, or 3 cards should be added if there are no sets */
	State state;						/**< @brief game state (play, pause, menu, etc) */
} GameState;
/** @}*/

/**@{
 * @brief Game State info
 */
typedef struct {
	Menu *menu;
	Mode mode;
	unsigned short number_players;
	unsigned short players[9][5];
	unsigned int scores[9];
	unsigned short array[81];
	GameState *gamestate;
	Graphics *graphics;
	Keyboard *keyboard;
	Mouse *mouse;
	Timer *timer;
	RTC *rtc;
	HighScore *highscores[10];
} Game;
/** @}*/

/**@{
 * @brief enumeration of game buttons
 */
typedef enum {
	SET_BUTTON,			/**< @brief button to indicate a new set */
	NOSETS_BUTTON,		/**< @brief button to indicate that there are no sets */
	HINT_BUTTON,		/**< @brief button to request a set hint */
	PAUSE_BUTTON,		/**< @brief button to pause the game (or play if the game is paused) */
	SAVE_BUTTON,		/**< @brief button to save the game */
	EXIT_BUTTON,		/**< @brief button to leave the game */
	NO_BUTTON			/**< @brief button to use as default when no button was pressed */
} Button;
/** @}*/

/**
 * @brief function that initializes and runs the game
 *
 * @return 0 upon success, 1 upon failure
 */
int run_game();

/** @} end of game */
#endif /* __GAME_H */
