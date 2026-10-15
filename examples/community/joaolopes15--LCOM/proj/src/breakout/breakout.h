#ifndef _BREAKOUT_H_
#define _BREAKOUT_H_

#include <lcom/lcf.h>
#include "../sprite/sprite.h"
#include "../sprite/animated_sprite.h"

typedef struct {
  Sprite *bar;
  Sprite *ball;
  Sprite *balls[5];
  bool active_balls[5];
  Sprite *bricks[60];
  AnimSprite *red_animated_bricks[12];
  AnimSprite *orange_animated_bricks[12];
  AnimSprite *yellow_animated_bricks[12];
  AnimSprite *green_animated_bricks[12];
  AnimSprite *blue_animated_bricks[12];
  Sprite *powerups[10];
  bool active_powerups[10];
  bool active_bricks[60];
  bool brick_animating[60];
  int brick_anim_frames[60];
  int lives;
  int score;
  bool ball_attached;
} breakout_t;

breakout_t* breakout_init();

void create_bricks(breakout_t *breakout);

int draw_bricks(breakout_t *breakout);

int draw_lives(breakout_t *breakout);

int draw_score(breakout_t *breakout);

int draw_breakout(breakout_t *breakout);

int draw_powerup(breakout_t *breakout);

void spawn_powerup(breakout_t *breakout, int x, int y);

void update_powerups(breakout_t *breakout);

void handle_powerup_collisions(breakout_t *breakout);

void spawn_extra_ball(breakout_t *breakout);

void destroy_breakout(breakout_t *breakout);

void handle_all_ball_collisions(breakout_t *breakout);

void move_bar_with_ball_keyboard(breakout_t *breakout, int direction);

void move_bar_with_ball_mouse(breakout_t *breakout, int target_x, int target_y, int xspeed);

#endif
