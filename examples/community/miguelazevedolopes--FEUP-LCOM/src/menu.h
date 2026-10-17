
#include <lcom/lcf.h>
#include <stdint.h>
/** @defgroup menu menu.h
 * @{
 *
 * This file contains the functions responsible for most of the menus behaviour
 * 
 */

typedef struct{
    uint16_t x;
    uint16_t y;
    xpm_image_t img;
    uint8_t *pixmap;

} menu;

/**
 * @brief Initializes the menu sprites
 * 
 */
void initMenu();

/**
 * @brief Draws the pause menu.
 * 
 */
void loadPauseMenu();

/**
 * @brief Initializes the sprites used for the pause menu.
 * 
 */
void initPauseMenu();

/**
 * @brief Initializes the sprites used in the highscore menu.
 * 
 */
void initScoreMenu();

/**
 * @brief Draws the date. Used in the highscore menu.
 * 
 */
void drawDate();

/**
 * @brief Initializes the sprites used in the Game Over screen.
 * 
 */
void initGameOver();

/**
 * @brief Draws the game over screen.
 * 
 * @param winner the winner of the game.
 */
void drawGameOver(int winner);

/**
 * @brief Draws the max score menu.
 * 
 */
void drawMaxScoreMenu();

/**
 * @brief Checks if player clicks exit on the high score menu exit button.
 * 
 * @param pk is the mouse packet. Used for colision and click detection.
 * @return 1 if the player clicks exit, and 0 otherwise.
 */
int goBackToMainMenu(struct packet *pk);

/**
 * @brief Draws pause menu.
 * 
 */
void drawPauseMenu();

/**
 * @brief Draws the main menu.
 * 
 */
void drawMenu();

/**
 * @brief Calls the drawMenu function and draws the mouse pointer.
 * 
 */
void loadMenu();

/**
 * @brief Calls the drawMaxScoreMenu function and draws the mouse pointer.
 * 
 */
void loadScoreMenu();

/**
 * @brief handles the mouse pointer movement.
 * 
 * @param pk 
 */
void move_pointer(struct packet *pk);

/**
 * @brief Checks if the player clicks on any of the menu options
 * 
 * @param pk is the mouse packet. Used for colision and click detection.
 * @return 1 for Play, 2 for MaxScore Menu and 3 to exit game completly.
 */
int detectMenuOptionClick(struct packet *pk);

/**
 * @brief Checks if the player clicks on any of the pause menu options
 * 
 * @param pk is the mouse packet. Used for colision and click detection.
 * @return 2 for Continue game, and 1 for Exit to main menu.
 */
int detectPauseMenuOptionClick(struct packet *pk);

/**
 * @brief Used to clear the screen every time, you change menu.
 * 
 */
void clearScreen();
/**@}*/
