#include "game.h"
#include "controller/keyboard.h"
#include "snake.h"
#include "world.h"
#include "controller/gpu.h"
#include "sprite.h"
#include <lcom/lcf.h>

extern Sprite *game_over_sprite;
extern int counter;
extern uint8_t scancode;
extern bool is_scancode_invalid;
GameState game_state = GAME_RUNNING;

Snake *snake;
World *world;

void(game_init)() {
  snake = snake_create(50);
  world = world_create();
  world_respawn_apple(world);
}

void(update_timer_state)() {
  timer_int_handler();
  printf("counter: %d\n", counter);
  if (counter % 5 == 0) {
    snake_tick(snake);
    world_update(world, snake);
    if (snake->hasLost) {
      //snake_reset(snake);
      //game_state = GAME_OVER;
      create_sprites();
      draw_sprite(game_over_sprite, 225, 100);//later do the calculations in order for it to be printed in the center
    }
    else{
    world_render(world);
    snake_render(snake);
    }
  }
  swap_buffers();
}

void(update_keyboard_state)() {
  kbc_ih();
  if (!is_scancode_invalid) {
    if (scancode == ESC_BREAK_CODE) {
      game_state = GAME_OVER;
    }
    Direction dir = snake_get_physical_direction(snake);
    switch (scancode) {
      case UP_ARROW_MAKE_CODE:
      case W_MAKE_CODE:
        if (dir != Down) {
          snake->dir = Up;
        }
        break;
      case DOWN_ARROW_MAKE_CODE:
      case S_MAKE_CODE:
        if (dir != Up) {
          snake->dir = Down;
        }
        break;
      case LEFT_ARROW_MAKE_CODE:
      case A_MAKE_CODE:
        if (dir != Right) {
          snake->dir = Left;
        }
        break;
      case RIGHT_ARROW_MAKE_CODE:
      case D_MAKE_CODE:
        if (dir != Left) {
          snake->dir = Right;
        }
        break;
      default:
        break;
    }
  }
}
