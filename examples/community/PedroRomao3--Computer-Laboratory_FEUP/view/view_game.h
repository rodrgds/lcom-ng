#ifndef _VIEW_GAME_H_
#define _VIEW_GAME_H_

#include "devices/video/graphics.h"
#include "devices/kbc/mouse.h"
#include "sprite.h"
#include "../model/player.h"
#include "view.h"

#define X_POS 600
#define Y_POS 200
#define ANIMATION_COMPLETE 2

extern MouseLoc mouse_loc;
extern Player p1;
extern Player p2;
extern Player pc;
extern GameCoins g_coins;

typedef struct {
  int final_x, final_y;
  int * current_card_id;
  int * final_card_id;
  bool virada;
  int step;
} AnimCard;

/**
 @brief Draws the game table, cards, buttons, and various elements on the screen.
This function is responsible for rendering the game table screen, including cards,
buttons, coin sprites, player's money, and time. It updates the screen based on the
current game state, mouse location, and button presses.
 @return 0 on success, 1 on error.
*/
int drawMesa();


/**
 @brief Draws a specific card at a specific position on the game table.
This function takes a card index and a position index and draws the card sprite
at the designated position on the game table. It utilizes a predefined list of
positions for where the cards can be drawn.
 @param card The index of the card to be drawn.
 @param pos The index of the position where the card will be drawn.
 @return 0 on success, 1 on error.
*/
int drawCard(int card, int pos);


/**
 @brief Draws the animation for cards being dealt on the game table.
This function animates the movement of the cards being dealt to the player and
the computer (dealer). It calculates the intermediate positions of the cards based
on the final destination and the number of animation frames (GAME_FREQUENCY).
Once the animation is complete, it updates the card state for the player and
the computer.
 @return 0 on success, 1 on error, 2 when the animation is completed.
*/
int drawAnimCard(AnimCard *card);

/**
 @brief Draws a coin sprite on the screen.
 @param x The x position of the mouse cursor.
 @param y The y position of the mouse cursor.
 @param coinIndex The index of the coin sprite to draw.
 @param s1 The x position to draw the sprite.
 @param s2 The y position to draw the sprite.
 @param f A boolean flag indicating whether the sprite is selected.
 @return int Returns 1 if there was an error drawing the sprite, otherwise returns 0.
This function draws a coin sprite on the screen at the specified position. If the
position is currently being hovered by the mouse cursor, or if the sprite is
flagged as selected, it will draw the selected coin sprite. Otherwise, it will draw
the unselected coin sprite.
Complexity: O(1)
*/
int drawCoinSprite(int x, int y, int coinIndex, int s1, int s2, bool is_selected);


#endif
