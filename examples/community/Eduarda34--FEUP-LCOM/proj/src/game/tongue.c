#include <lcom/lcf.h>
#include "tongue.h"
#include "controller/video/colors.h"
bool show_tongue = false;
int tongue_timer = 0;
bool tongue_button_held = false;
extern StateGame game_st;

void tongue_init(void) {
  show_tongue = false;
  tongue_timer = 0;
  tongue_button_held = false;
}

void tongue_reset(void) {
  show_tongue = false;
  tongue_timer = 0;
}

void tongue_handle_mouse(bool left_button_pressed) {
  if (left_button_pressed && !tongue_button_held) {
    show_tongue = true;
    tongue_timer = TONGUE_DURATION;
    tongue_button_held = true;
  } else if (!left_button_pressed) {
    tongue_button_held = false;
  }
}

void tongue_update(void) {
  if (show_tongue) {
    tongue_timer--;
    if (tongue_timer <= 0) {
      show_tongue = false;
    }
  }
}

void tongue_draw(uint16_t screen_width, uint16_t screen_height) {
  if (game_st == GAME_OVER) return;
  if (snake_body == NULL || snake_length <= 0) return;

  if (!show_tongue) return;

  Point pos_tongue = tongue_direction(snake_body[0], current_direction, screen_width, screen_height);

  // Collision with body check 
  for (int i = 1; i < snake_length; i++) {
    if (snake_body[i].x == pos_tongue.x && snake_body[i].y == pos_tongue.y) {
      printf("Game Over: Snake's tongue hit its body\n");
      game_st = GAME_OVER;
      return;
    }
  }

  // Draw tongue
  switch (current_direction) {
    case UP: vg_draw_xpm(snake_tongue_up, pos_tongue.x, pos_tongue.y); break;
    case DOWN: vg_draw_xpm(snake_tongue_down, pos_tongue.x, pos_tongue.y); break;
    case LEFT: vg_draw_xpm(snake_tongue_left, pos_tongue.x, pos_tongue.y); break;
    case RIGHT: vg_draw_xpm(snake_tongue_right, pos_tongue.x, pos_tongue.y); break;
  }
}

Point tongue_direction(Point head_pos, Direction dir, uint16_t width, uint16_t height) {
  Point pos_tongue = head_pos;

  switch (dir) {
    case UP:
      pos_tongue.y = (head_pos.y == 0) ? height - CELL_SIZE : head_pos.y - CELL_SIZE;
      break;
    case DOWN:
      pos_tongue.y = (head_pos.y + CELL_SIZE >= height) ? 0 : head_pos.y + CELL_SIZE;
      break;
    case LEFT:
      pos_tongue.x = (head_pos.x == 0) ? width - CELL_SIZE : head_pos.x - CELL_SIZE;
      break;
    case RIGHT:
      pos_tongue.x = (head_pos.x + CELL_SIZE >= width) ? 0 : head_pos.x + CELL_SIZE;
      break;
  }

  return pos_tongue;
}
