#ifndef _LCOM_FONT_H_
#define _LCOM_FONT_H_

#include "sprites/sprites.h"

typedef struct {
  sprite_t *letter_A;
  sprite_t *letter_B;
  sprite_t *letter_C;
  sprite_t *letter_D;
  sprite_t *letter_E;
  sprite_t *letter_F;
  sprite_t *letter_G;
  sprite_t *letter_H;
  sprite_t *letter_I;
  sprite_t *letter_J;
  sprite_t *letter_K;
  sprite_t *letter_L;
  sprite_t *letter_M;
  sprite_t *letter_N;
  sprite_t *letter_O;
  sprite_t *letter_P;
  sprite_t *letter_Q;
  sprite_t *letter_R;
  sprite_t *letter_S;
  sprite_t *letter_T;
  sprite_t *letter_U;
  sprite_t *letter_V;
  sprite_t *letter_W;
  sprite_t *letter_X;
  sprite_t *letter_Y;
  sprite_t *letter_Z;
  sprite_t *zero;
  sprite_t *one;
  sprite_t *two;
  sprite_t *three;
  sprite_t *four;
  sprite_t *five;
  sprite_t *six;
  sprite_t *seven;
  sprite_t *eight;
  sprite_t *nine;
  sprite_t *space;
} font_t;

font_t *font;

/**
 * @brief Initializes the font
 */
void (font_init)();

/**
 * @brief Draws text on the screen
 * 
 * @param text Text to be drawn
 * @param x X coordinate of the text
 * @param y Y coordinate of the text
 * @return int 0 if success, 1 otherwise
 */
int (draw_text)(char* text, int x, int y, uint8_t color);

/**
 * @brief Destroys the font
 */
void (font_destroy)();

#endif // _LCOM_FONT_H_
