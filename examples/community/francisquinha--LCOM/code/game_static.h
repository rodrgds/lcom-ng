#ifndef __GAME_STATIC_H
#define __GAME_STATIC_H

/** @file game_static.h
 * @brief file containing the declarations of the static functions in game.c
 * the functions that are not declared in game.h are here
 */

/** @defgroup game_static game_static
 * @{
 *
 * @brief static functions for running the game
 */
/**
 * @brief reset the gamestate so that a new game can be played
 * @param gamestate the gamestate to be reset
 */
static void reset_gamestate(GameState *gamestate);

/**
 * @brief create a new gamestate to used in a new game
 * @return pointer to the created GameState
 */
static GameState *create_gamestate();

/**
 * @brief create a new game to be played
 * @return pointer to the created Game
 */
static Game *create_game();

/**
 * @brief destroy a game that was created when it is no longer needed
 * @param game to be destroyed
 */
static void destroy_game(Game *game) ;

/**
 * @brief given a displayed card index, compute the position of its top left corner
 * @param game pointer to the game whose card position we want to determine
 * @param index index of the card in the display
 * @param x	horizontal coordinate of the top left corner
 * @param y vertical coordinate of the top left corner
 */
static void get_position(Game *game, unsigned short index, unsigned short *x, unsigned short *y);

/**
 * @brief given a keyboard scan code, determine the corresponding button during game play (or pause)
 * @param button the button that corresponds to that scan code
 * @param scan_code the scan code whose corresponding button we want to find
 */
static void get_keyboard_button(Button *button, long scan_code);

/**
 * @brief given a mouse click determine the button it corresponds to during game play (or pause)
 * @param game the game of the mouse click
 * @param button the button that was clicked
 * @param x the horizontal coordinate of the mouse click
 * @param y the vertical coordinate of the mouse click
 */
static void get_mouse_button(Game *game, Button *button, unsigned short x, unsigned short y);

/**
 * @brief given a mouse click determine the card it corresponds to during game play
 * @param game the game of the mouse click
 * @param index the index of the card clicked, 81 if no card was clicked
 * @param x the horizontal coordinate of the mouse click
 * @param y the vertical coordinate of the mouse click
 */
static void get_mouse_card_index(Game *game, unsigned short *index, unsigned short x, unsigned short y);

/**
 * @brief remove from the display the card with a given index
 * @param game the game whose displayed card we wish to remove
 * @param base the buffer where the card is displayed
 * @param origin the original background buffer
 * @param index the index of the card to be removed
 */
static void remove_card(Game *game, char *base, char *origin, unsigned short index);

/**
 * @brief add a card to the display in the position of a given card index
 * @param game the game where we want to display the new card
 * @param base the buffer where the card is displayed
 * @param index the index of the position the card should be added to
 */
static void add_card(Game *game, char *base, unsigned short index);

/**
 * @brief add 3 new cards to the display when there are no sets within the displayed cards
 * @param game the game whose display will get 3 new cards
 */
static void add_cards(Game *game);

/**
 * @brief add 3 new cards to the display when 3 other cards (forming a set) have been removed
 * @param game the game whose display will get 3 new cards
 */
static void add_cards_remove(Game * game);

/**
 * @brief add a frame around a newly selected card
 * @param game the game where the frame will be drawed
 * @param base the buffer where the frame will be drawed
 * @param index the index of the card around which to draw the frame
 */
static void add_frame(Game *game, char *base, unsigned short index);

/**
 * @brief remove a frame from around a previosly selected card
 * @param game the game where the frame will be drawed
 * @param base the buffer where the frame will be drawed
 * @param origin the original buffer containing the background
 * @param index the index of the card around which to draw the frame
 */
static void remove_frame(Game *game, char *base, char *origin, unsigned short index);

/**
 * @brief remove all frames from the game display
 * @param game the game from where the frames will be removed
 */
static void remove_frames(Game *game);

/**
 * @brief move a card from one position to another
 * @param game the game where the card will be moved
 * @param base the buffer where the cards are drawed
 * @param origin the original buffer containing the background
 * @param destination_index the index of the new position intended for the card
 * @param origin_index the index of the current card position
 */
static void displace_card(Game *game, char *base, char *origin,
	unsigned short destination_index, unsigned short origin_index);

/**
 * @brief make the necessary arrangements so that 3 new cards can be added to the display
 * @param game the game where the cards will be added
 */
static void set_add_cards(Game *game);

/**
 * @brief pause an ongoing game
 * @param game the game to be paused
 */
static void pause_game(Game *game);

/**
 * @brief unpause a paused game
 * @param game the game to be unpaused
 */
static void play_game(Game *game);

/**
 * @brief save a game to be continued later
 * @param game to be saved
 */
static void save_game(Game *game);

/**
 * @brief load a previously saved game
 * @param game the game to be loaded
 */
static void load_game(Game *game);

/**
 * @brief go back to the game menu
 * @param game the game to go to menu
 */
static void menu_game(Game *game);

/**
 * @brief select a card using the mouse
 * @param game the game where the card will be selected
 * @param index the index of the card to be selected
 */
static void select_card(Game *game, unsigned short index);

/**
 * @brief select and take action on a button using the mouse or the keyboard
 * @param game the game where the button will be selected
 * @param button the button to be selected
 */
static void select_button(Game *game, Button button);

/**
 * @brief check the state of a game for playing inconsistencies
 * @param game the game whose state is to be checked
 */
static void state_check(Game *game);

/**
 * @brief to handle the unselection of cards
 * @param game whose selection and unselection will be handled
 */
static void state_undo(Game *game);

/**
 * @brief act accordingly to the game state (remove/add cards, get hints, etc)
 * @param game whose action should be taken
 */
static void state_action(Game *game);

/**
 * @brief interruption handler for the mouse, keyboard and timer
 *
 * This does not handle the application side of the interruption,
 * only gets the info into the corresponding structures
 *
 * @param int_irq interruption irq
 * @param arg arguments for the interruption handler
 * arg[0] is the the game pointer
 * arg[1] is the mouse irq line
 * arg[2] is the keyboard irq line
 * arg[3] is the timer irq line
 * @return always returns 0, use to be 1 to stop interrupt loop, but that is for the app handler
 */
static unsigned short mouse_keyboard_timer_int_handler(int int_irq, long *arg);

/**
 * @brief compute the scores at the end of a game
 * @param game the game whose scores will be computed
 */
static void compute_scores(Game *game);

/**
 * @brief show the current highscores at the end of a game
 * @param game the game whose highscores should be shown
 */
static void show_highscores(Game *game);

/**
 * @brief handle all game scoring
 *
 * computes the scores, computes the new highscore table, saves the new highscores and displays them
 *
 * @param game the game whose scoring will be handled
 */
static void score_game(Game *game);

/**
 * @brief randomly shuffle and array of unsigned shorts
 * @param array the array to be shuffled
 * @param n the size of the array
 */
static void shuffle(unsigned short *array, size_t n);

/**
 * @brief start a new game according to its settings
 * @param game the game to be started
 */
static void start_game(Game *game);

/**
 * @brief interruption application handler for the mouse, keyboard and timer
 *
 * This does not handle the interruption itself, but its consequences
 * according to the needs of the application
 *
 * @param int_irq interruption irq
 * @param arg arguments for the interruption handler
 * arg[0] is the the game pointer
 * arg[1] is the mouse irq line
 * arg[2] is the keyboard irq line
 * arg[3] is the timer irq line
 * @return 0 if the loop should continue, 1 if it should stop
 */
static unsigned short mouse_keyboard_timer_app_handler(int int_irq, long *arg);

/** @} end of game_static */
#endif /* __GAME_STATIC_H */
