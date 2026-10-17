#ifndef _CONTROLGAME_H_
#define _CONTROLGAME_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "control_base.h"
#include "view/layout.h"

extern GameState state;
extern bool isMulti;

#define DEF_CARD 40
#define NAME_X 70
#define NAME_Y 550

/**
 @brief Checks the game time and updates the game state if necessary.
This function is called to track the remaining time for the player. It decrements
the player's seconds when the counter is a multiple of 60 and the player has at least
one card. If the player's time runs out, the game state is updated to STAND.
Time Complexity: O(1) - Constant time complexity as the operations performed are independent of input size.
 @return int - Always returns 0, as there is no error handling in this function.
*/
int checkTime();

/**
 @brief Executes the game logic based on the current state.
This function handles the game logic for different states and performs
various operations such as drawing the mesa, cursor, text, animation card,
and checking time. It also handles player and computer steps in the stand state,
draws coin sprite in the chip state, draws rectangle in the edit state,
and draws the table and table digit in the table state.
 @return 0 on success, 1 on failure.
*/
int ti_game();

/**
 @brief Executes the game logic based on the current keyboard input and state.
This function handles various keyboard inputs and performs corresponding operations
based on the current state of the game. It handles sending characters, changing state,
updating player name in the edit state, handling table inputs, and returning to the menu state.
 @return 0 on success.
*/
int ki_game();

/**
 @brief Handles mouse actions based on the current state of the game.
This function takes the mouse coordinates and determines the current state of the game.
It then calls the corresponding function to handle mouse actions based on the state.
 @return 0 on success.
*/
int mouseaction_game();

/**
 @brief Handles the attribution of the card received from the serial communication.
This function takes a character pointer and extracts the 2 most significant bits
to determine the card attribution. It then updates the corresponding card variables
(multicard1, multicard2, multicard3) based on the extracted value.
 @param char_rec Pointer to the received character.
 @return 0 on success.
*/
int serp_card_attribution(uint8_t *char_rec);

/**
 @brief Handles the received quit signal.
This function checks the current state of the game and if it is in the OPRDY state,
it changes the state to MENU.
 @return 0 on success.
*/
int received_quit();

/**
 @brief Handles the received duel signal
This function checks if the game is not in multiplayer mode (!isMulti).
If the state of the game is WAIT, it calls the function accept_duel().
If the state is MENU, it changes the state to OPRDY (opponent ready).
 @return 0 on success.
*/
int received_duel();

/**
 @brief Creates an animated card for the game.
This function creates an animated card by calling the assignCard() function
for each available card in the game. It assigns the card's position and other
properties based on the player's cards (p1, pc, p2) and the anim_card structure.
The function also prints "hello world" as a debug message.
 @return 0 on success.
*/
int createAnimCard();

/**
 @brief Executes a step for the computer player by drawing a random card.
This function takes the card identifier as input and assigns a random card
to the corresponding card slot of the computer player. It then calls the
drawAnim() function to visualize the card being drawn.
Time Complexity: O(1) - Constant time complexity as the operations performed are independent of input size.
 @param card_id int - The identifier of the card slot to be updated (1 or 2).
 @return int - Always returns 0, as there is no error handling in this function.
*/
int pc_step(int card_id);


/**
 @brief Performs the next step for player 2 in the game.
This function handles the steps for player 2 in the game.
It checks if there is an ongoing animation (anim_card.step != 0) and returns if true.
It also checks if the selected card is available (multicard1, multicard2, multicard3)
and returns if the selected card is not available (equals 40).
If the selected card is available, it updates the corresponding card variable (p2.a1, p2.a2, p2.a3)
based on the card_id parameter.
Finally, it creates an animated card by calling the createAnimCard() function.
 @param card_id The ID of the selected card (1, 2, or 3).
 @return 0 on success.
*/
int p2_step(int card_id);

#endif
