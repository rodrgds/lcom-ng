#include <lcom/lcf.h>

#include "ball_view.h"

#include "sprite_view.h"

int draw_ball(Ball *ball) {
  if (!ball) {
    printf("%s: ball is NULL\n", __func__);
    return 1;
  }

  if (draw_sprite(ball->sprite, ball->x, ball->y)) {
    printf("%s: draw_sprite(ball->sprite, ball->x: %d, ball->y: %d) error\n", __func__, ball->x, ball->y);
    return 1;
  }

  return 0;
}
