#ifndef _VIEW_MENU_H_
#define _VIEW_MENU_H_

#include "devices/video/graphics.h"
#include "devices/kbc/mouse.h"
#include "devices/kbc/keyboard.h"
#include "sprite.h"
#include "../model/player.h"
#include "../model/table.h"
#include "devices/timer/rtc.h"
#include "view.h"

extern MouseLoc mouse_loc;
extern Player p1;
extern time_info data;
extern person_info persons[MAX_PERSONS];
extern int person_count;
extern char to_save[MAX_LENGTH];

/**
 @brief Draws the main menu on the screen.
This function draws the main menu and its elements such as the title and buttons
(Start and Exit) on the screen, taking into account the current mouse location.
The pressed button state is displayed when the mouse is over the button.
 @return 0 on success, 1 on error.
*/
int drawMainMenu();

/**
 @brief Draws the "Loss" screen.
This function clears the buffer and displays the "Loss" screen when the player loses
the game. It also shows the player's remaining money.
 @return 0 on success, 1 on error.
*/
int drawLoose();


/**
 @brief Draws the "Win" screen with the player's final amount of money
This function first clears the screen buffer using the clearBuffer() function.
It then draws the "Win" sprite at position (0, 0) using the drawSprite() function.
The player's final amount of money is stored in the p1.money variable, which is
converted to a string using the snprintf() function and stored in the sum variable.
Finally, the sum string is drawn at position (380, 290) using the drawText() function.
 @return 0 if the function executes successfully, 1 if an error occurs while drawing the sprites
 @note The complexity of this function is O(1).
*/
int drawWin();

/**
 @brief Draws the "No Winner" screen, showing the "Tie" sprite and the player's money.
This function clears the graphics buffer and draws the "Tie" sprite at the top left corner of the screen.
It then displays the player's money in the bottom center of the screen.
 @return 0 if successful, 1 if an error occurred during drawing.
 @note This function assumes that the graphics buffer has already been initialized.
 @complexity O(1)
*/
int drawNoWinner();

/**
 @brief Draws a table with player data including their names, money, and time.
This function draws a table on the screen, displaying the names, money, and
time information for each player in the game. It clears the buffer, draws the
table layout, and fills in the table with the information from the data
structure. Only the first MAX_PERSONS_DRAW players are shown.
 @note The time information for each player is calculated using the calculate_time_difference() function.
 @return int Returns 0 upon successful completion.
 @par Complexity:
 Time complexity: O(MAX_PERSONS_DRAW)
 Space complexity: O(1)
*/
int drawTable();

/**
 @brief Draws the game data for a single player, including their name, money, and time difference.
This function draws the specified player's name, money, and the calculated time difference
(given as a time_difference structure) on the screen at the specified x and y coordinates.
The drawn data includes name, money (formatted as a 3-digit number), and time (formatted as
MM:DD:HH:mm).
 @param[in] diff A time_difference structure containing the player's time difference data.
 @param[in] name A string containing the player's name, up to 50 characters.
 @param[in] money An integer value representing the player's money.
 @param[in] x The x-coordinate for drawing the player's data.
 @param[in] y The y-coordinate for drawing the player's data.
 @return int Returns 0 upon successful completion.
 @par Complexity:
 Time complexity: O(1)
 Space complexity: O(1)
*/
int draw_game(time_difference diff, const char name[MAX_LENGTH], int money, int x, int y);

/**
 @brief Draws a table digit on the screen.
This function extracts the name and digit information from the to_save string and draws them on the screen.
It starts drawing the name at coordinates (10, 480) and increments the x-coordinate based on the length of the name.
Then, it iterates through the remaining characters in to_save and draws the corresponding digit sprites on the screen.
It also handles the positioning of double dots between digits and draws rectangles to highlight the last character or empty spaces.
 @return 0 if the table digit is successfully drawn.
*/
int drawTableDigit();

/**
 @brief Draws the "Wait" screen.
This function clears the buffer and draws the "Wait" screen using the menuName and wait sprites.
 @return Returns 0 on success, or 1 if there was an error drawing the sprites.
*/
int draw_wait();

#endif
