#include <lcom/lcf.h>

#include "game_view.h"

#include "../model/game.h"

#include "ball_view.h"
#include "sprite_view.h"
#include "wall_view.h"

extern Ball *ball;
extern Wall *wall;
extern uint32_t score;
extern uint8_t multiplier;

extern uint16_t x_max;
extern uint16_t y_max;

int draw_game() {
  if (draw_times(x_max * 0.05, y_max * 0.05)) {
    printf("%s: draw_times(x_max * 0.05: %f, y_max * 0.05: %f)\n", __func__, x_max * 0.05, y_max * 0.05);
    return 1;
  }

  if (draw_digit(multiplier, x_max * 0.05 + 20, y_max * 0.05)) {
    printf("%s: draw_digit(multiplier: %d, x_max * 0.05 + 20: %f, y_max * 0.05: %f) error\n", __func__, multiplier, x_max * 0.05 + 20, y_max * 0.05);
    return 1;
  }

  if (draw_number(score, x_max * 0.95, y_max * 0.05)) {
    printf("%s: draw_number(score: %d, x_max * 0.95: %f, y_max * 0.05: %f) error\n", __func__, score, x_max * 0.95, y_max * 0.05);
    return 1;
  }

  if (draw_ball(ball)) {
    printf("%s: draw_ball(ball) error\n", __func__);
    return 1;
  }

  if (draw_wall(wall)) {
    printf("%s: draw_wall(wall) error\n", __func__);
    return 1;
  }

  return 0;
}
