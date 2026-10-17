#ifndef _VIEW_H_
#define _VIEW_H_

#include "devices/video/graphics.h"
#include "devices/kbc/mouse.h"
#include "devices/kbc/keyboard.h"
#include "sprite.h"
#include "layout.h"

extern struct packet mouse_data;
extern MouseLoc mouse_loc;

int drawButton(Sprite *normalSprite, Sprite *pressedSprite, int xStart, int yStart);

/**
 @brief Draws the cursor on the screen based on the current mouse location and button state.
This function takes into account the current mouse location and button state
(left, right, and middle buttons) to draw the appropriate cursor icon.
Additionally, it supports changing the cursor to a hand icon within a specific
area of the screen, controlled by the f parameter.
 @param f Flag to control the hand icon display:
     - 0: Default behavior (cursor or click icons).
     - 1: Hand icon when the mouse is within the specified area.
     - 2: Always display hand icon, regardless of the mouse location.
 @return 0 on success, 1 on error.
*/
int drawCursor(int f);

/**
 @brief Draws a name on the screen using sprites.
This function draws a given name on the screen using sprites, where each
letter and number is represented by a specific sprite in the `letters` and
`numbers` arrays. The function iterates over each character in the input
name, checks whether it is a letter or a number, and then draws the
corresponding sprite for that character. The x and y coordinates determine
the position where the name is drawn on the screen.
 @param x The x-coordinate of the starting position of the name.
 @param y The y-coordinate of the starting position of the name.
 @param name A pointer to the character array containing the name to be drawn.
 @return Returns 0 on success, or 1 if an error occurred while drawing the name.
 @note The `letters` and `numbers` arrays are assumed to be global variables.
 @warning The input name must be a null-terminated string, i.e., the last
character in the array must be the null character '\0'.
 @par Complexity:
    @li Time: O(n), where n is the length of the input name.
    @li Space: O(1), as this function only creates a fixed number of temporary variables.
*/
int drawText(int x, int y, const char name[MAX_LENGTH]);


/**
 @brief Draws the time in the game
This function receives the time in seconds and draws it in the format MM:SS.
It uses the drawText() function to draw each digit of the minutes and seconds,
and the doubleDot sprite to separate them.
 @param x X coordinate of the position to draw the time
 @param y Y coordinate of the position to draw the time
 @param sec Time in seconds
 @return int 1 if there was an error during drawing, 0 otherwise
 @note This function has a complexity of O(1).
*/
int drawTime(int x, int y, int seconds);

#endif
