#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>
#include "keyboard.h"
#include "videocard.h"
#include "mouse.h"
#include "ball.h"
#include "background.h"
#include "bricks.h"
#include "animate_sprite.h"
#include "i8042.h"
#include "i8254.h"



#include "../assets/paddle.xpm"
#include "../assets/game_over.xpm"



#include "../assets/heart.xpm"
#include "../assets/numbers/0.xpm"
#include "../assets/numbers/1.xpm"
#include "../assets/numbers/2.xpm"
#include "../assets/numbers/3.xpm"
#include "../assets/numbers/4.xpm"
#include "../assets/numbers/5.xpm"
#include "../assets/numbers/6.xpm"
#include "../assets/numbers/7.xpm"
#include "../assets/numbers/8.xpm"
#include "../assets/numbers/9.xpm"




#include "../assets/explosao.xpm"


//menu

#include "../assets/logo.xpm"
#include "../assets/cursor.xpm"
#include "../assets/play_button.xpm"
#include "../assets/exit_button.xpm"



/**
 * @brief enum with all the possible game states
 * 
 */
typedef enum {MENU,GAME,EXIT} state;


int detect_colisions(Sprite* cursor,uint16_t x,uint16_t y, uint16_t width,uint16_t height);

/**
 * @brief draws the score on the second buffer
 * 
 * @param points the score we want to draw
 * @param numbers array with the sprites of numbers from zero to nine
 * @return int 0 in case it goes sucessfully, 1 otherwise
 */

int draw_score(int points,Sprite **numbers);

/**
 * @brief draws the game over screen
 * 
 * @param won 1 in case the player won, 0 otherwise
 * @param game_over_logo sprite with the game over logo
 * @param lose_logo sprite with the lose logo
 * @param win_logo sprite with the win logo
 * @param continue_logo sprite with the continue logo
 */
void game_over(uint8_t won,Sprite *game_over_logo,Sprite *lose_logo, Sprite *win_logo,Sprite *continue_logo);
/**
 * @brief main game loop
 * 
 * @return int 0 in case it goes sucessfully, 1 otherwise
 */
int breakout(state* game_state);





#endif
