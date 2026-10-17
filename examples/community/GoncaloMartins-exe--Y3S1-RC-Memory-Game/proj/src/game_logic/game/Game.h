/**
 * @file Game.h
 * @brief Contains functions and structures for managing the game logic.
 */
#ifndef GAME_H
#define GAME_H

#include <stdlib.h> 
#include <time.h>    
#include <string.h>

#define BOARD_WIDTH 4  /**< Width of the game board (number of columns) */
#define BOARD_HEIGHT 4  /**< Height of the game board (number of rows) */

#include "../board/Board.h"
#include "../player/Player.h"
#include "../../menu/menu.h"


/**
 * @struct Game
 * @brief Represents the game state, including the board and the player.
 */
typedef struct {
    Board board;    /**< The game board */
    Player player;  /**< The player */
} Game;


/**
 * @brief Starts and runs the main game loop.
 * Initializes game components, handles user input and hardware interrupts,
 * updates animations, renders the game screen, and determines game completion.
 * @param game Pointer to the Game structure.
 * @param kbd_irq Pointer to the bit mask for keyboard interrupt subscription.
 */
void game_run(Game* game, uint8_t* kbd_irq);

/**
 * @brief Initializes the game state, including player, board, and shuffled cards.
 * Randomly assigns fruit pairs to cards, positions them on the board,
 * and prepares the player for interaction.
 * @param game Pointer to the Game structure to initialize.
 */
void game_init(Game* game);


/**
 * @brief Updates all card animations on the board.
 * Iterates through the board and advances any ongoing animations
 * (e.g., card flip transitions).
 * @param board Pointer to the Board structure.
 */
void board_update_animations(Board* board);

/**
 * @brief Renders the current visual state of the game.
 * Draws the board, cards, player position, and possibly other UI elements.
 * @param game Pointer to the Game structure.
 */
void game_draw(const Game* game);

/**
 * @brief Draws the fruit on the front side of a card.
 * Used to visually represent the card's assigned fruit when it's face-up.
 * @param card Pointer to the card whose fruit is to be drawn.
 * @param game Pointer to the current Game structure.
 */
void desenhar_fruta(Card* card, Game* game);


/**
 * @brief Draws the back (hidden) side of a card.
 * Optionally used when the card is not face-up to conceal its content.
 * @param card Pointer to the card to be drawn.
 * @param faceUp Indicates whether the card should be shown face-up or face-down.
 */
void desenhar_carta_traseira(Card* card, bool faceUp);

/**
 * @brief Handles player keyboard input to control movement and selection.
 * Responds to arrow keys to move the player and to action keys (e.g., Enter)
 * to flip cards or navigate menus.
 * @param game Pointer to the Game structure.
 */
void input_handler(Game* game);


/**
 * @brief Checks the current game state for matches or mismatches.
 * If two unmatched face-up cards are detected, determines whether they form a match.
 * If matched, marks them as matched; otherwise, triggers a flip-back animation.
 * @param game Pointer to the Game structure.
 * @return int Status code (0 = no update needed, 1 = animation or state change occurred).
 */
int check_game_status(Game* game);

/**
 * @brief Determines whether all cards on the board have been matched.
 * Used to identify the game's completion state and trigger a win condition.
 * @param game Pointer to the Game structure.
 * @return true if all cards are matched; false otherwise.
 */
bool game_is_finished(Game* game);


#endif
