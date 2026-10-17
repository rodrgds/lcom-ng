#include <lcom/lcf.h>
#include <math.h>

#include "draw.h"
#include "game_state.h"
#include "ui.h"
#include "graphic.h"


#include "xpm/0.h"
#include "xpm/1.h"
#include "xpm/2.h"
#include "xpm/3.h"
#include "xpm/4.h"
#include "xpm/5.h"
#include "xpm/6.h"
#include "xpm/7.h"
#include "xpm/8.h"
#include "xpm/9.h"
#include "xpm/button_play.h"
#include "xpm/button_quit.h"
#include "xpm/cursor.h"
#include "xpm/background.h"
#include "xpm/ball.h"
#include "xpm/button_instructions.h"
#include "xpm/instructions.h"
#include "xpm/title.h"

vbe_mode_info_t mode_info;

Button *play_button;
Cursor *cursor_menu;
Button *quit;
Button *instruction_button;
Ball *ball;
Wall *top_wall;
Wall *bottom_wall;

Player *player1;
Player *player2;
Digit digits[10];


unsigned int player1_score = 0;
unsigned int player2_score = 0;


uint16_t x_max = 1024;
uint16_t y_max = 768;

game_state state;

uint8_t *background;
uint8_t *title;

#define WIDTH_NUMBER 20
#define COLOR_BACKGROUND 0x000000
#define WALL_COLOR 0xFFFFFF

bool changed = true;

bool init_ball(void) {
  ball = create_ball((xpm_map_t)ball_xpm);
  return true;
}

bool init_walls(void) {
  top_wall = create_wall(true);
  bottom_wall = create_wall(false);
  return true;
}

bool init_cursor(void) {
  cursor_menu = create_cursor(mouse_pos.x, mouse_pos.y, (xpm_map_t)cursor_xpm);
  return true;
}

bool init_buttons(void) {
  play_button = create_button(x_max / 3 - 133, 2 * y_max / 4 - 57, (xpm_map_t)button_play_xpm, play);
  quit = create_button(2 * x_max / 3 - 133, 2 * y_max / 4 - 57, (xpm_map_t)button_quit_xpm, exit_game);
  instruction_button = create_button(x_max / 2 - 230, 3 * y_max / 4 - 57, (xpm_map_t)button_instructions_xpm, instructions);

  return true;
}

bool init_players(void) {
  player1 = create_player(1);
  player2 = create_player(2);
  return true;
}

bool init_digits(void) {
  xpm_image_t image;
  
  uint8_t *sprite = xpm_load((xpm_map_t)xpm_0, XPM_INDEXED, &image);
  digits[0] = (Digit){ image.width, image.height, sprite };
  sprite = xpm_load((xpm_map_t)xpm_1, XPM_INDEXED, &image);
  digits[1] = (Digit){ image.width, image.height, sprite };
  sprite = xpm_load((xpm_map_t)xpm_2, XPM_INDEXED, &image);
  digits[2] = (Digit){ image.width, image.height, sprite };
  sprite = xpm_load((xpm_map_t)xpm_3, XPM_INDEXED, &image);
  digits[3] = (Digit){ image.width, image.height, sprite };
  sprite = xpm_load((xpm_map_t)xpm_4, XPM_INDEXED, &image);
  digits[4] = (Digit){ image.width, image.height, sprite };
  sprite = xpm_load((xpm_map_t)xpm_5, XPM_INDEXED, &image);
  digits[5] = (Digit){ image.width, image.height, sprite };
  sprite = xpm_load((xpm_map_t)xpm_6, XPM_INDEXED, &image);
  digits[6] = (Digit){ image.width, image.height, sprite };
  sprite = xpm_load((xpm_map_t)xpm_7, XPM_INDEXED, &image);
  digits[7] = (Digit){ image.width, image.height, sprite };
  sprite = xpm_load((xpm_map_t)xpm_8, XPM_INDEXED, &image);
  digits[8] = (Digit){ image.width, image.height, sprite };
  sprite = xpm_load((xpm_map_t)xpm_9, XPM_INDEXED, &image);
  digits[9] = (Digit){ image.width, image.height, sprite };
  background = xpm_load((xpm_map_t)background_xpm, XPM_INDEXED, &image);
  title = xpm_load((xpm_map_t)title_xpm, XPM_INDEXED, &image);
  return true;
}

int draw_ball(Ball *ball) {
  if (!ball) {
    printf("%s: NULL ball\n", __func__);
    return 1;
  }

  if ((int)ball->x_pos <= 0) {
    player2_score++;
    ball->x_pos = x_max / 2;
    ball->y_pos = y_max / 2;
    ball->angle = M_PI / 3;
    ball->vX = 5; ball->vY = -5;
    return 0;
  } else if ((int)ball->x_pos >= x_max - (int)ball->width) {
    player1_score++;
    ball->x_pos = x_max / 2;
    ball->y_pos = y_max / 2;
    ball->angle = 4 * M_PI / 3;
    ball->vX = -5; ball->vY = -5;
    return 0;
  }

  if (player1_score == 10 || player2_score == 10) {
    state = EXIT;
    return 0;
  }

  for (uint16_t h = 0; h < ball->height; h++) {
    for (uint16_t w = 0; w < ball->width; w++) {
      if (video_draw_pixel(ball->x_pos + w, ball->y_pos + h, ball->sprite[w + h * ball->width])) {
        return 1;
      }
    }
  }

  return 0;
}

int draw_button(Button *button) {
  if (!button) {
    printf("%s: NULL button\n", __func__);
    return 1;
  }

  for (uint16_t h = 0; h < button->height; h++) {
    for (uint16_t w = 0; w < button->width; w++) {
      if (video_draw_pixel(button->pos_x + w, button->pos_y + h, button->sprite[w + h * button->width])) {
        return 1;
      }
    }
  }

  return 0;
}

int draw_cursor(Cursor *cursor) {
  if (!cursor) {
    printf("%s: NULL cursor\n", __func__);
    return 1;
  }


  for (uint16_t h = 0; h < cursor->height; h++) {
    for (uint16_t w = 0; w < cursor->width; w++) {
      if (video_draw_pixel(mouse_pos.x + w, mouse_pos.y + h, cursor->sprite[w + h * cursor->width])) {
        return 1;
      }
    }
  }
  return 0;
}

int draw_game() {

  if (draw_number(player2_score, x_max * 0.95, y_max * 0.05)) {
    printf("%s: draw_number(score: %d, x_max * 0.95: %f, y_max * 0.05: %f) error\n", __func__, score, x_max * 0.95, y_max * 0.05);
    return 1;
  }

  if (draw_number(player1_score, x_max * 0.05, y_max * 0.05)) {
    printf("%s: draw_number(score: %d, x_max * 0.95: %f, y_max * 0.05: %f) error\n", __func__, score, x_max * 0.95, y_max * 0.05);
    return 1;
  }

  if (draw_ball(ball)) {
    printf("%s: draw_ball(ball) error\n", __func__);
    return 1;
  }

  if (draw_wall(top_wall)) {
    printf("%s: draw_wall(wall) error\n", __func__);
    return 1;
  }

  if (draw_wall(bottom_wall)) {
    printf("%s: draw_wall(wall) error\n", __func__);
    return 1;
  }

  if (draw_player(player1)) {
    printf("%s: draw_wall(wall) error\n", __func__);
    return 1;
  }
  
  if (draw_player(player2)) {
    printf("%s: draw_wall(wall) error\n", __func__);
    return 1;
  }
  

  return 0;
}

int draw_menu() {

  for (uint16_t h = 0; h < 200; h++) {
    for (uint16_t w = 0; w < 1000; w++) {
      if (video_draw_pixel(w + 12, 92 + h, title[w + h * 1000])) {
        return 1;
      }
    }
  }


  if (draw_button(play_button)) {
    printf("%s: draw_button(play) error\n", __func__);
    return 1;
  }

  if (draw_button(instruction_button)) {
    printf("%s: draw_button(instructions) error\n", __func__);
    return 1;
  }

  if (draw_button(quit)) {
    printf("%s: draw_button(quit) error\n", __func__);
    return 1;
  }

  if (draw_cursor(cursor_menu)) {
    printf("%s: draw_cursor(cursor_menu) error\n", __func__);
    return 1;
  }

  return 0;
}



bool draw_digit(Digit *digit, uint16_t x, uint16_t y) {
  for (uint16_t h = 0; h < digit->height; h++) {
    for (uint16_t w = 0; w < digit->width; w++) {
      if (video_draw_pixel(x + w, y + h, digit->sprite[w + h * digit->width])) {
        printf("%s: video_draw_pixel(x + w: %d, y + h: %d, color: 0x%x) error\n", __func__, x + w, y + h, digit->sprite[w + h * digit->width]);
        return 1;
      }
    }
  }

  return 0;
}

int draw_number(uint32_t number, uint16_t x, uint16_t y) {

  if (number < 9) {
    draw_digit(&digits[number], x, y);
    return 0;
  }

  uint32_t reverse = 0;

  for (; number > 0; number /= 10) reverse = reverse * 10 + (number % 10);

  for (uint32_t digit = reverse; digit > 0; digit /= 10) {
    draw_digit(&digits[digit % 10], x, y);
    x += digits[digit % 10].width;
  }

  return 0;
}


int draw_frame() {
  video_draw_background(background);

  switch (state)
  {
  case (MAIN_MENU): {
    if (changed) {
      xpm_image_t image;
      background = xpm_load((xpm_map_t)background_xpm, XPM_INDEXED, &image);
      changed = false;
    }
    return draw_menu();
  } case (PLAYING):
      return draw_game();
    case INSTRUCTIONS: {
      if (changed) {
        xpm_image_t image;
        background = xpm_load((xpm_map_t)instructions_xpm, XPM_INDEXED, &image);
        changed = false;
    }
    break;
  } default:
    break;
  }

  return 0;
}

int draw_wall(Wall *wall) {
  if (!wall) {
    printf("%s: wall is NULL\n", __func__);
    return 1;
  }

  if (video_draw_rectangle(wall->x_pos, wall->y_pos, wall->width, wall->height, 63)) {
    printf("%s: video_draw_rectangle(wall->x_pos; %d, wall->y_pos: %d, wall->width: %d, wall->height: %d, WALL_COLOR: 0x%x) error\n", __func__, wall->x_pos, wall->y_pos, wall->width, wall->height, WALL_COLOR);
    return 1;
  }

  return 0;
}

int draw_player(Player *player) {
  for (uint16_t w = 0; w < player->width; w++) {
    for (uint16_t h = 0; h < player->height; h++) {
      video_draw_rectangle(player->x_pos, player->y_pos + h, player->width, 1, 63);
    }
  }
  return 0;
}
