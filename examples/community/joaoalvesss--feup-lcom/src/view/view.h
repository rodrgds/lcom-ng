/**
 * @file view.h
 * @brief file that is part of and MVC system and responsible for drawing the game images and having the game logic implemented
 * 
 */

#ifndef _VIEW_H_
#define _VIEW_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "configuration.h"
#include "devices/video/video.h"
#include "devices/mouse/mouse.h"
#include "model/sprites.h"
#include "model/model.h"


/**
 * @brief using a state machine this function detects whenever a state is changed and if so, a new screen will be loaded
 * 
 */
void drawNewFrame();


/**
 * @brief this function is responsable of drawing the main menu sprite
 * 
 */
void drawStartMenu();


/**
 * @brief this function is responsable of drawing the a game with easy difficulty (4 by 2 card grid) and the frame around the cards so when playing with keyboard the user can identify the selected card at the moment
 * 
 */
void drawEasyGame();


/**
 * @brief this function is responsable of drawing the a game with medium difficulty (4 by 3 card grid) and the frame around the cards so when playing with keyboard the user can identify the selected card at the moment
 * 
 */
void drawMediumGame();


/**
 * @brief this function is responsable of drawing the a game with hard difficulty (4 by 4 card grid) and the frame around the cards so when playing with keyboard the user can identify the selected card at the moment
 * 
 */
void drawHardGame();


/**
 * @brief this function is responsable of drawing the end game menu sprite
 * 
 */
void drawEndMenu();


/**
 * @brief this fuction is responsable of drawing the mouse into all menus, and detecting if the mouse is hover some button and changing the sprite that is being drawn
 * 
 */
void drawMouse();


/**
 * @brief create the "animation" of the dinossaur image (the back part of the cards), the "animation" is created having three xpms and alternating between them
 * 
 * @param i the amount of frames that will be drawn depending on the difficulty selected
 */
void drawFrames(int i);

void freeFrames();

/**
 * @brief responsible for changing the current sprite that is being displayed in the back part of the cards to the next sprite in order to create the "animation"
 * 
 * @param i in the three images that represent the "animation", i represents the number of the image that will be displayed next
 */
void changeFrame(int i);


/**
 * @brief this function copies the content of the first buffer to second buffer in order to allow the double buffering by memory copy
 * 
 * @return int: 0 if success, otherwise 1
 */
int flipBuffers();


/**
 * @brief draws the image given at the coords of the screen given
 * 
 * @param sprite the image that we want to draw
 * @param x the screen width of where we want to draw the sprite
 * @param y the screen height of where we want to draw the sprite
 * @return int: 0 if success otherwise 1
 */
int drawSpriteXpm(Sprite *sprite, int x, int y);


/**
 * @brief set the buffers in order to use double buffering to avoid flickers in the game
 * 
 * @param mode passed to know the buffer size needed to the mode selected so only the needed memory will be allocated
 * @return int: 0 if sucess, otherwise 1
 */
int setBuffers(uint16_t mode);


/**
 * @brief responsable for flipping the cards to show their content
 * 
 * @param card in the form xxyy1nnn, where xx represents the position on the x axis, y on the y axis, and nnn is the card type
 * @return int: 0 if card was flipped with success, 1 otherwise 
 */
int flip_card(uint8_t card);


/**
 * @brief responsable for flipping the cards to hide their content
 * 
 * @param card in the form xxyy1nnn, where xx represents the position on the x axis, y on the y axis, and nnn is the card type
 * @return int: 0 if card was unflipped sucess, 1 otherwise  
 */
int unflip_card(uint8_t card);


/**
 * @brief this function reads the current states and makes actions accordingly, such as printing a new frame, or in-game actions such as selecting and turning cards
 * 
 * @return int: 0 if everything gone correct otherwise 1
 */
int game();


/**
 * @brief this function creates a random card board for the game to work on, depending on difficulty. It also resets some states so that the game becomes ready to play
 * 
 * @return int: 0 if everything goes well, 1 otherwise
 */
int game_setup();

/**
 * @brief this function draws the game screen timer given the coords
 * @param x is the x coordenad where it draws the timer
 * @param y is the y coordenad where it draws the timer
 * @return int: 0 if everything goes well, 1 otherwise
 */
int display_time(int x,int y);


/**
 * @brief this function draws a digit given the digit number and the coords
 * @param digit is the number from 0 to 9 that is used to determine which digit to draw
 * @param x is the x coordenad where it draws the digit
 * @param y is the y coordenad where it draws the digit
 * @return int: 0 if everything goes well, 1 otherwise
 */
int displayDigit(uint32_t digit, int x , int y);


/**
 * @brief changes the card bits in order to know its position and the type of the card
 * 
 * @param n 1 or 2 depending on it being the first or the second selected by the user
 * @return int: 0 if success, 1 otherwise
 */
int select_card(int n);

#endif /* _VIEW_H_*/
