#ifndef __GAME_H_
#define __GAME_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "../main.h"
#include "sprites/sprite.h"
#include "sprites/animSprite.h"
#include "../../keyboard_mouse/mouse.h"
#include "card.h"

/**
 * @defgroup game Game
 * @{
 *
 * Functions and data structures for managing the game state and user interface
 */

#define CARD_INITIAL_X 780
#define CARD_INITIAL_Y 160

#define CARD_FINAL_X 534
#define CARD_FINAL_Y -100

#define CARD_SPEED_X 8
#define CARD_SPEED_Y 16

#define INITIAL_COUNTDOWN_TIME_LEFT 2

#define DEALER_CARD_1_X 445
#define DEALER_CARD_1_Y 248
#define DEALER_CARD_2_X 534
#define DEALER_CARD_2_Y 248
#define DEALER_CARD_3_X 623
#define DEALER_CARD_3_Y 248

#define PLAYER_CARD_1_X 445
#define PLAYER_CARD_1_Y 468
#define PLAYER_CARD_2_X 534
#define PLAYER_CARD_2_Y 468
#define PLAYER_CARD_3_X 623
#define PLAYER_CARD_3_Y 468

#define SLIDER_X 570
#define SLIDER_Y 830
#define SLIDER_WIDTH (double) 500.0
#define SLIDER_HEIGHT 10
#define SLIDER_THUMB_SIZE 30

#define DECREASE_BET_BTN_X 500
#define DECREASE_BET_BTN_Y 815
#define DECREASE_BET_BTN_WIDTH 40
#define DECREASE_BET_BTN_HEIGHT 40

#define INCREASE_BET_BTN_X 1100
#define INCREASE_BET_BTN_Y 815
#define INCREASE_BET_BTN_WIDTH 40
#define INCREASE_BET_BTN_HEIGHT 40

#define FOLD_BTN_X 260
#define FOLD_BTN_Y 815
#define FOLD_BTN_WIDTH 100
#define FOLD_BTN_HEIGHT 40

#define PLAY_BTN_X 370
#define PLAY_BTN_Y 815
#define PLAY_BTN_WIDTH 100
#define PLAY_BTN_HEIGHT 40

#define PLAYER_NAME_X 10
#define PLAYER_NAME_Y 765
#define PLAYER_NAME_WIDTH 240
#define PLAYER_NAME_HEIGHT 40

#define NUMBER_OF_COINS_X 10
#define NUMBER_OF_COINS_Y 815
#define NUMBER_OF_COINS_WIDTH 240
#define NUMBER_OF_COINS_HEIGHT 40

#define MESSAGE_X 415
#define MESSAGE_Y 408
#define MESSAGE_WIDTH 322
#define MESSAGE_HEIGHT 40

#define NO_COINS_X 400
#define NO_COINS_Y 408
#define NO_COINS_WIDTH 353
#define NO_COINS_HEIGHT 40

#define PLAYER_HAND_TYPE_X 415
#define PLAYER_HAND_TYPE_Y 654
#define PLAYER_HAND_TYPE_WIDTH 322
#define PLAYER_HAND_TYPE_HEIGHT 40

#define DEALER_HAND_TYPE_X 415
#define DEALER_HAND_TYPE_Y 158
#define DEALER_HAND_TYPE_WIDTH 322
#define DEALER_HAND_TYPE_HEIGHT 40

#define BET_X 150
#define BET_Y 408
#define BET_WIDTH 240
#define BET_HEIGHT 40

/**
 * @brief Enumeration representing different states of the game
 */
typedef enum {
    GAME_INITIALIZE, /**< Initialization state */
    GAME_DEAL_CARDS, /**< Dealing cards state */
    GAME_BET, /**< Betting state */
    GAME_SHOW_PLAYER_HAND, /**< Showing player's hand state */
    GAME_DECIDE, /**< Deciding state */
    GAME_SHOW_DEALER_HAND, /**< Showing dealer's hand state */
    GAME_PLAY_HAND, /**< Playing hand state */
    GAME_FOLD_HAND, /**< Folding hand state */
    GAME_WIN, /**< Winning state */
    GAME_LOSE, /**< Losing state */
    GAME_DRAW, /**< Drawing state */
    GAME_REMOVE_CARDS, /**< Removing cards state */
    GAME_NO_COINS, /**< No coins state */
} game_state_enum;

/**
 * @brief Handles keyboard input during the game
 *
 * This function handles keyboard input during the game.
 */
void handle_kbd_game();

/**
 * @brief Handles mouse input during the game
 *
 * This function handles mouse input during the game.
 */
void handle_mouse_game();

/**
 * @brief Handles timer interrupts during the game
 *
 * This function handles timer interrupts during the game.
 */
void handle_timer_game();

/**
 * @brief Handles going back to the main menu
 *
 * This function handles going back to the main menu during the game.
 */
void handle_game_go_back();

/**
 * @brief Handles folding the hand
 *
 * This function handles folding the hand during the game.
 */
void handle_game_fold_hand();

/**
 * @brief Handles playing the hand
 *
 * This function handles playing the hand during the game.
 */
void handle_game_play_hand();

/**
 * @brief Handles mouse hover actions during the game
 *
 * This function handles mouse hover actions during the game.
 */
void handle_game_mouse_hover();

/**
 * @brief Handles mouse click actions during the game
 *
 * This function handles mouse click actions during the game.
 */
void handle_game_mouse_click();

/**
 * @brief Handles mouse input on the bet slider
 *
 * This function handles mouse input on the bet slider during the game.
 */
void handle_game_mouse_slider();

/**
 * @brief Updates the bet value while sliding the slider
 *
 * This function updates the bet value while sliding the slider during the game.
 */
void update_bet_if_sliding();

/**
 * @brief Checks if the mouse is on the bet slider thumb
 *
 * This function checks if the mouse is on the bet slider thumb during the game.
 */
bool is_on_bet_slider_thumb();

/**
 * @brief Checks if the mouse is on the bet slider area
 *
 * This function checks if the mouse is on the bet slider area during the game.
 */
bool is_on_bet_slider_area();

/**
 * @brief Checks if the mouse is on the fold button
 *
 * This function checks if the mouse is on the fold button during the game.
 */
bool is_on_fold_button();

/**
 * @brief Checks if the mouse is on the play button
 *
 * This function checks if the mouse is on the play button during the game.
 */
bool is_on_play_button();

/**
 * @brief Checks if the mouse is on the decrease bet button
 *
 * This function checks if the mouse is on the decrease bet button during the game.
 */
bool is_on_decrease_bet_button();

/**
 * @brief Checks if the mouse is on the increase bet button
 *
 * This function checks if the mouse is on the increase bet button during the game.
 */
bool is_on_increase_bet_button();

/**
 * @brief Validates the number of coins
 *
 * This function validates the number of coins during the game.
 */
void validate_number_of_coins();

/**
 * @brief Validates the bet value
 *
 * This function validates the bet value during the game.
 */
void validate_bet();

/**
 * @brief Decreases the bet value
 *
 * This function decreases the bet value during the game.
 * @param value Value to decrease the bet by
 */
void decrease_bet(int value);

/**
 * @brief Increases the bet value
 *
 * This function increases the bet value during the game.
 * @param value Value to increase the bet by
 */
void increase_bet(int value);

/**
 * @brief Handles the countdown timer
 * 
 * This function handles the countdown timer during the game.
 */
void countdown_timer();

/**
 * @brief Increases the number of coins
 *
 * This function increases the number of coins during the game.
 */
void increase_number_of_coins();

/**
 * @brief Decreases the number of coins
 *
 * This function decreases the number of coins during the game.
 */
void decrease_number_of_coins();

/**
 * @brief Decreases the number of coins when betting
 *
 * This function decreases the number of coins when betting during the game.
 */
void decrease_number_of_coins_bet();

/**
 * @brief Draws the dealer's hand
 *
 * This function draws the dealer's hand on the screen during the game.
 */
void draw_dealer_hand();

/**
 * @brief Draws the player's hand
 *
 * This function draws the player's hand on the screen during the game.
 */
void draw_player_hand();

/**
 * @brief Flips the dealer's cards
 *
 * This function flips the dealer's cards during the game.
 */
void flip_dealer_cards();

/**
 * @brief Flips the player's cards
 *
 * This function flips the player's cards during the game.
 */
void flip_player_cards();

/**
 * @brief Draws the revealed dealer's cards
 *
 * This function draws the revealed dealer's cards on the screen during the game.
 */
void draw_show_dealer_cards();

/**
 * @brief Draws the revealed player's cards
 *
 * This function draws the revealed player's cards on the screen during the game.
 */
void draw_show_player_cards();

/**
 * @brief Draws the player's hand type
 *
 * This function draws the player's hand type on the screen during the game.
 */
void draw_player_hand_type();

/**
 * @brief Draws the dealer's hand type
 *
 * This function draws the dealer's hand type on the screen during the game.
 */
void draw_dealer_hand_type();

/**
 * @brief Draws the player's name
 *
 * This function draws the player's name on the screen during the game.
 */
void draw_player_name();

/**
 * @brief Draws the number of coins
 *
 * This function draws the number of coins on the screen during the game.
 */
void draw_number_of_coins();

/**
 * @brief Draws the bet value
 *
 * This function draws the bet value on the screen during the game.
 */
void draw_bet();

/**
 * @brief Draws a message on the screen
 *
 * This function draws a message on the screen during the game.
 * @param message Message to display
 */
void draw_message(char message[]);

/**
 * @brief Draws the bet slider
 *
 * This function draws the bet slider on the screen during the game.
 */
void draw_slider();

/**
 * @brief Draws the decrease bet button
 *
 * This function draws the decrease bet button on the screen during the game.
 */
void draw_decrease_bet_button();

/**
 * @brief Draws the increase bet button
 *
 * This function draws the increase bet button on the screen during the game.
 */
void draw_increase_bet_button();

/**
 * @brief Draws the fold button
 *
 * This function draws the fold button on the screen during the game.
 */
void draw_fold_button();

/**
 * @brief Draws the bet button
 *
 * This function draws the bet button on the screen during the game.
 */
void draw_bet_button();

/**
 * @brief Draws the play button
 *
 * This function draws the play button on the screen during the game.
 */
void draw_play_button();

/**
 * @brief Draws the game initialization screen
 *
 * This function draws the game initialization screen on the screen during the game.
 */
void draw_game_initialize();

/**
 * @brief Draws the screen during the dealing cards phase
 *
 * This function draws the screen during the dealing cards phase on the screen during the game.
 */
void draw_game_deal_cards();

/**
 * @brief Draws the screen during the betting phase
 *
 * This function draws the screen during the betting phase on the screen during the game.
 */
void draw_game_bet();

/**
 * @brief Draws the screen showing the player's hand
 *
 * This function draws the screen showing the player's hand on the screen during the game.
 */
void draw_game_show_player_hand();

/**
 * @brief Draws the screen during the decision phase
 *
 * This function draws the screen during the decision phase on the screen during the game.
 */
void draw_game_decide();

/**
 * @brief Draws the screen showing the dealer's hand
 *
 * This function draws the screen showing the dealer's hand on the screen during the game.
 */
void draw_game_show_dealer_hand();

/**
 * @brief Draws the screen during the playing hand phase
 *
 * This function draws the screen during the playing hand phase on the screen during the game.
 */
void draw_game_play_hand();

/**
 * @brief Draws the screen during the folding hand phase
 *
 * This function draws the screen during the folding hand phase on the screen during the game.
 */
void draw_game_fold_hand();

/**
 * @brief Draws the screen indicating a win
 *
 * This function draws the screen indicating a win on the screen during the game.
 */
void draw_game_win();

/**
 * @brief Draws the screen indicating a loss
 *
 * This function draws the screen indicating a loss on the screen during the game.
 */
void draw_game_lose();

/**
 * @brief Draws the screen indicating a draw
 *
 * This function draws the screen indicating a draw on the screen during the game.
 */
void draw_game_draw();

/**
 * @brief Draws the screen during the removing cards phase
 *
 * This function draws the screen during the removing cards phase on the screen during the game.
 */
void draw_game_remove_cards();

/**
 * @brief Draws the screen indicating no coins left
 *
 * This function draws the screen indicating no coins left on the screen during the game.
 */
void draw_game_no_coins();

/** @} */

#endif /* __GAME_H */
