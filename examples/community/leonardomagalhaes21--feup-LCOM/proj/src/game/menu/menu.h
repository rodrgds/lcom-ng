


#ifndef __MENU_H
#define __MENU_H
#include <lcom/lcf.h>
#include "../../devices/mouse/mouse.h"
#include "../../devices/keyboard/keyboard.h"
#include "../../devices/graphics/graphics.h"
#include "game/logic/save_score.h"
#include "../sprite/sprite.h"
#include "../classes/player.h"


#define SCAN_CODE_A 0x1E
#define SCAN_CODE_B 0x30
#define SCAN_CODE_C 0x2E
#define SCAN_CODE_D 0x20
#define SCAN_CODE_E 0x12
#define SCAN_CODE_F 0x21
#define SCAN_CODE_G 0x22
#define SCAN_CODE_H 0x23
#define SCAN_CODE_I 0x17
#define SCAN_CODE_J 0x24
#define SCAN_CODE_K 0x25
#define SCAN_CODE_L 0x26
#define SCAN_CODE_M 0x32
#define SCAN_CODE_N 0x31
#define SCAN_CODE_O 0x18
#define SCAN_CODE_P 0x19
#define SCAN_CODE_Q 0x10
#define SCAN_CODE_R 0x13
#define SCAN_CODE_S 0x1F
#define SCAN_CODE_T 0x14
#define SCAN_CODE_U 0x16
#define SCAN_CODE_V 0x2F
#define SCAN_CODE_W 0x11
#define SCAN_CODE_X 0x2D
#define SCAN_CODE_Y 0x15
#define SCAN_CODE_Z 0x2C
#define SCAN_CODE_COLON 0x27
#define SCAN_CODE_BACKSPACE 0x0E
#define SCAN_CODE_ENTER 0x1C
#define DEFAULT_X_RESOLUTION_14C 1152
#define DEFAULT_Y_RESOLUTION_14C 864

/**
 * @brief Enumerates the possible states of the game.
 * 
 * Represents the different states the game can be in, such as the main menu, the game itself, the scoreboard, the leaderboard, or the exit screen.
 */

typedef enum {
  MENU,
  GAME,
  SCOREBOARD,
  LEADERBOARD,
  EXIT,
  RESOLUTION,
} GameState;
extern GameState currentState;


/**
 * @brief Sets the game state to the main menu.
 */
void setMenuState();

/**
 * @brief Handles button click events for the "Play" button in the menu.
 * 
 * @param x The x-coordinate of the mouse click.
 * @param y The y-coordinate of the mouse click.
 * @return int Returns 0 on success.
 */
int playButton(int x, int y);


/**
 * @brief Handles button click events for the "Leaderboard" button in the menu.
 * 
 * @param x The x-coordinate of the mouse click.
 * @param y The y-coordinate of the mouse click.
 * @return int Returns 0 on success.
 */
int leaderboardButton(int x, int y);


/**
 * @brief Handles button click events for the "Exit" button in the menu.
 * 
 * @param x The x-coordinate of the mouse click.
 * @param y The y-coordinate of the mouse click.
 * @return int Returns 0 on success.
 */
int exitButton(int x, int y);


/**
 * @brief Handles button click events for the "Menu" button in the game.
 * 
 * @param x The x-coordinate of the mouse click.
 * @param y The y-coordinate of the mouse click.
 * @param score Pointer to the player's score.
 * @param multiplier Pointer to the player's score multiplier.
 * @return int Returns 0 on success.
 */
int menuButton(int x, int y, int *score, double *multiplier);


/**
 * @brief Handles button click events for the "Leaderboard" button in the game.
 * 
 * @param x The x-coordinate of the mouse click.
 * @param y The y-coordinate of the mouse click.
 * @return int Returns 0 on success.
 */
int menuButtonLeader(int x, int y);


/**
 * @brief Draws the main menu screen.
 * 
 * @return int Returns 0 on success.
 */
int drawMenu();


/**
 * @brief Draws the game over screen.
 * 
 * @param player Pointer to the player structure.
 * @param score The player's final score.
 * @return int Returns 0 on success.
 */
int drawGame(player *player, int score);


/**
 * @brief Draws the scoreboard screen.
 * 
 * @param score The player's score.
 * @return int Returns 0 on success.
 */
int drawScoreBoard(int score);


/**
 * @brief Draws the player's score.
 * 
 * @param score The player's score.
 * @param x The x-coordinate of the score.
 * @param y The y-coordinate of the score.
 * @return int Returns 0 on success.
 */
int drawScore(int score, uint16_t x, uint16_t y);


/**
 * @brief Draws the leaderboard screen.
 * 
 * @return int Returns 0 on success.
 */
int processScanCode(uint8_t scancodewrite);


/**
 * @brief Draws the resolution screen.
 * 
 * @return int Returns 0 on success.
 */
int drawLeaderBoard();


/**
 * @brief Draws the resolution screen.
 * 
 * @return int Returns 0 on success.
 */
int drawResolution();


/**
 * @brief Handles button click events for the "Resolution 14C" button in the menu.
 * 
 * @param x The x-coordinate of the mouse click.
 * @param y The y-coordinate of the mouse click.
 * @return int Returns 0 on success.
 */
int res14C(int x, int y);


/**
 * @brief Handles button click events for the "Resolution 110" button in the menu.
 * 
 * @param x The x-coordinate of the mouse click.
 * @param y The y-coordinate of the mouse click.
 * @return int Returns 0 on success.
 */
int res110(int x, int y);


/**
 * @brief Handles button click events for the "Resolution 11A" button in the menu.
 * 
 * @param x The x-coordinate of the mouse click.
 * @param y The y-coordinate of the mouse click.
 * @return int Returns 0 on success.
 */
int res11A(int x, int y);


/**
 * @brief Handles button click events for the "Resolution 115" button in the menu.
 * 
 * @param x The x-coordinate of the mouse click.
 * @param y The y-coordinate of the mouse click.
 * @return int Returns 0 on success.
 */
int res115(int x, int y);

#endif

