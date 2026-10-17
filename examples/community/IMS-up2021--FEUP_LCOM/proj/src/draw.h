#ifndef _DRAW_H_
#define _DRAW_H_

#include "entity.h"
#include "ui.h"

extern Ball *ball;
extern Wall *top_wall;
extern Wall *bottom_wall;
extern uint32_t score;

extern Button *play_button;
extern Button *quit;
extern Button *instruction_button;
extern Cursor *cursor_menu;

extern Player *player1;
extern Player *player2;

extern uint16_t x_max;
extern uint16_t y_max;

extern uint8_t *background;

extern bool changed;

bool init_digits(void);
bool init_buttons(void);
bool init_cursor(void);
bool init_ball(void);
bool init_walls(void);
bool init_players(void);

/** @defgroup ball_view ball_view
 * @
 *
 * Functions related to the view of the ball
 */

/**
 * @brief Ball drawer
 *
 * @param ball pointer to the ball that we need do be draw
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_ball(Ball *ball);

/**
 * @brief Button drawer
 *
 * @param button pointer to the button that we need do be draw
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_button(Button *button);

/**
 * @brief Cursor drawer
 *
 * @param cursor pointer to the cursor that we need do be draw
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_cursor(Cursor *cursor);

/**
 * @brief In the current state of the game, draws the game
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_game();

/**
 * @brief In the current state of the menu, draws the menu
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_menu();

int draw_player();


/**
 * @brief Digit [0-9] drawer
 *
 * @param digit digit to be drawn
 * @param x x-coordinate that to draw the digit
 * @param y y-coordinate that draw the digit
 * @return Return 0 upon success and non-zero otherwise
 */
bool draw_digit(Digit *digit, uint16_t x, uint16_t y);

/**
 * @brief Number drawer
 *
 * @param number number to be drawn
 * @param x x-coordinate that draw the number
 * @param y y-coordinate that to draw the number
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_number(uint32_t number, uint16_t x, uint16_t y);

/**
 * @brief Draws the current frame
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_frame();

/**
 * @brief Wall drawer
 *
 * @param wall  to the wall that we need do be draw
 * @return Return 0 upon success and non-zero otherwise
 */
int draw_wall(Wall *wall);

#endif 
