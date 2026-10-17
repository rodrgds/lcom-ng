#include <lcom/lcf.h>

#include "game.h"

#include "ball.h"
#include "sprite.h"
#include "wall.h"

#include "../controller/keyboard.h"
#include "../controller/mouse.h"

extern scancode code;
extern struct packet mouse_packet;

Ball *ball = NULL;
Wall *wall = NULL;

uint16_t x_max;
uint16_t y_max;

uint32_t score;
uint8_t multiplier;

static uint32_t counter;
static uint32_t speedup;

int start_game(uint16_t xResolution, uint16_t yResolution, uint8_t difficulty) {
  uint8_t direction = (rand() % 2) ? 1 : -1; // ball starting direction may be left or right
  ball = construct_ball(xResolution / 2, yResolution / 2, difficulty * direction, difficulty * direction);
  if (!ball) {
    printf("%s: construct_ball(%d, %d, %d, %d) error\n", __func__, xResolution / 2, yResolution / 2, difficulty * direction, difficulty * direction);
    return 1;
  }

  wall = construct_wall(0, yResolution / 2 - 45 / difficulty, 10, 90 / difficulty);
  if (!wall) {
    printf("%s: construct_wall(%d, %d, %d, %d) error\n", __func__, 0, yResolution / 2 - 45 / difficulty, 10, 90 / difficulty);
    return 1;
  }

  if (construct_numbers()) {
    printf("%s: construct_numbers() error\n", __func__);
    return 1;
  }

  x_max = xResolution;
  y_max = yResolution;
  score = 0;
  multiplier = difficulty;
  counter = 0;
  speedup = 600 * multiplier;

  return 0;
}

void timer_game_handler() {
  counter++;
  int16_t y = ball->y + ball->sprite->height / 2; // ordenada do ponto mais à esquerda da bola
  if (ball->x <= wall->x + wall->w && y >= wall->y && y <= wall->y + wall->h) {
    // colisão da bola com a parede
    score += multiplier;
    move_ball_after_collision_with_wall(ball, wall);
  }
  move_ball(ball, x_max, y_max);
  if (counter % speedup == 0 && !speedup_ball(ball)) {
    multiplier++;
    speedup  = 600 * multiplier;
    counter = 0;
  }
}

void keyboard_game_handler() {
  if ((code.size == 2 && code.bytes[1] == KBD_ARROW_UP_MAKECODE_LSB) || (code.size == 1 && code.bytes[0] == KBD_W_MAKECODE))
    move_wall_up(wall);
  if ((code.size == 2 && code.bytes[1] == KBD_ARROW_DOWN_MAKECODE_LSB) || (code.size == 1 && code.bytes[0] == KBD_S_MAKECODE))
    move_wall_down(wall, y_max);
}

void mouse_game_handler() {
  if (mouse_packet.rb && !speedup_ball(ball)) {
    multiplier++;
    speedup  = 600 * multiplier;
    counter = 0;
  }
}

bool check_game_over() {
  return ball->x <= wall->x && (ball->y < wall->y || ball->y > wall->y + wall->h);
}

void end_game() {
  destroy_ball(ball);
  destroy_wall(wall);
  destroy_numbers();
}
