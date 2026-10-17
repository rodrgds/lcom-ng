#include "sprite.h"

/**
 * @defgroup game
 * @{
 * @brief Functions that allow the game to happen
 */

/**
 * @brief Location of where the game is.
 */
enum location {ENTRANCE, MENU, SINGLEPLAYER, GAMEOVER, INSTRUCTIONS, PAUSE, EXIT};

/**
 * @brief Type of interruption occured.
 */
enum type_of_interrupt {TIMER,MOUSE,KEYBOARD,RTC};

/**
 * @brief Checks if the cursor is on top of a button.
 * @param button to be checked
 * @return true if it is
 * @return false if not
 */
bool check_click(Sprite *button);

/**
 * @brief Updates the cursor on the screan upon movement.
 */
void update_cursor();

/**
 * @brief Draws the player on the screen.
 * 
 */
void show_man();

/**
 * @brief Draws the backgroung of the game on the screen with the pause button.
 */
void show_background();

/**
 * @brief Changes the cursor.
 * @param num If 0, changes to a spoon, if 1 to a mira
 */
void changecursor(uint8_t num);

/**
 * @brief Change the map of a Sprite to another picture.
 * @param but Sprite to be changed
 * @param num Number of the picture in the array
 */
void changebutton(Sprite* but, int num);

/**
 * @brief Draws the hearts on the screen.
 */
void show_lives();

/**
 * @brief Randomly chooses the food to fall.
 * @param fruit_number Number corresponding to the food.
 * @param x Inicial position.
 * @param speed Speed that the food falls
 * @return Food* 
 */
Food* choose_food(int fruit_number,int x, int speed);

/**
 * @brief Draws the fruit on the screen.
 */
void draw_fruit();

/**
 * @brief Changes the fruit position so it doesn't fall on the same place.
 */
void changeFruitsPos();

/**
 * @brief Destroys the fruit.
 */
void clear_fruits();

/**
 * @brief Checks if fruit touched the flor or the man.
 */
void check_colisions();

/**
 * @brief Checks if the cursor has destroyed the fruit.
 */
void check_destruction();

/**
 * @brief Prepares the screen to exit the game.
 * @param numBackgound Depending if the game is running or it hasn't stil run.
 */
void exit_game(int numBackgound);

/**
 * @brief Main function of the project that does all the logic of the game.
 * @param loc Location of the game
 * @param interrupt Type of interruption occured
 */
void handler(enum location *loc, enum type_of_interrupt interrupt);

/** @} end of game */
