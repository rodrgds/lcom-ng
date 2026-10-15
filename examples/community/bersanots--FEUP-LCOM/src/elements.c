#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "constants.h"
#include "elements.h"
#include "i8042.h"
#include "pixmaps.h"
#include "video.h"

Sprite *elements[NUM_ELEMENTS];
uint8_t *background;

/**
 * @brief Create all moving game elements as sprites
 * 
 */
void create_elements() {
  /* Create sprites */
  elements[RED_PLAYER_IDX] = create_sprite(red_player, 220, 403, 0, 0);   //red player
  elements[BLUE_PLAYER_IDX] = create_sprite(blue_player, 863, 403, 0, 0); //blue player
  elements[BALL_IDX] = create_sprite(ball, 553, 414, 0, 0);               //ball
}

void reset_positions() {
  //red player
  elements[RED_PLAYER_IDX]->x = 220;
  elements[RED_PLAYER_IDX]->y = 403;
  elements[RED_PLAYER_IDX]->xspeed = 0;
  elements[RED_PLAYER_IDX]->yspeed = 0;

  //blue player
  elements[BLUE_PLAYER_IDX]->x = 863;
  elements[BLUE_PLAYER_IDX]->y = 403;
  elements[BLUE_PLAYER_IDX]->xspeed = 0;
  elements[BLUE_PLAYER_IDX]->yspeed = 0;

  //ball
  elements[BALL_IDX]->x = 553;
  elements[BALL_IDX]->y = 414;
  elements[BALL_IDX]->xspeed = 0;
  elements[BALL_IDX]->yspeed = 0;
}

void destroy_elements() {
  for (int i = 0; i < NUM_ELEMENTS; i++) {
    destroy_sprite(elements[i]);
    elements[i] = NULL;
  }
}

int draw_static_elements() {
  if (draw_background())
    return 1;
  draw_goals();
  draw_scoreboard();
  draw_time_board();
  return 0;
}

Sprite **get_elements() {
  return elements;
}

uint8_t *get_background() {
  return background;
}

int draw_background() {
  xpm_image_t img;
  background = xpm_load(pitch, XPM_8_8_8_8, &img);

  if (img.width != get_hres() || img.height != get_vres()) {
    printf("Background doesn't fit the screen\n");
    return 1;
  }

  draw_pixmap(background, 0, 0, img.width, img.height);

  return 0;
}

int draw_goals() {
  xpm_image_t red_goal_img, blue_goal_img;

  //red goal
  uint8_t *red_goal_pixmap = xpm_load(red_goal, XPM_8_8_8_8, &red_goal_img);
  draw_pixmap(red_goal_pixmap, 57, 286, red_goal_img.width, red_goal_img.height);

  //blue goal
  uint8_t *blue_goal_pixmap = xpm_load(blue_goal, XPM_8_8_8_8, &blue_goal_img);
  draw_pixmap(blue_goal_pixmap, 1012, 286, blue_goal_img.width, blue_goal_img.height);

  return 0;
}

int draw_scoreboard() {
  xpm_image_t scoreboard_img, red_score_img, blue_score_img;

  //scoreboard
  uint8_t *scoreboard_pixmap = xpm_load(scoreboard, XPM_8_8_8_8, &scoreboard_img);
  draw_pixmap(scoreboard_pixmap, 483, 79, scoreboard_img.width, scoreboard_img.height);

  //red player score
  uint8_t *red_score_pixmap = xpm_load(zero, XPM_8_8_8_8, &red_score_img);
  draw_pixmap(red_score_pixmap, 534, 88, red_score_img.width, red_score_img.height);

  //blue player score
  uint8_t *blue_score_pixmap = xpm_load(zero, XPM_8_8_8_8, &blue_score_img);
  draw_pixmap(blue_score_pixmap, 587, 88, blue_score_img.width, blue_score_img.height);

  return 0;
}

int update_scores(int red, int blue) {
  xpm_image_t red_score_img, blue_score_img;

  uint8_t *red_score_pixmap, *blue_score_pixmap;

  switch (red) {
    case 0:
      red_score_pixmap = xpm_load(zero, XPM_8_8_8_8, &red_score_img);
      break;
    case 1:
      red_score_pixmap = xpm_load(one, XPM_8_8_8_8, &red_score_img);
      break;
    case 2:
      red_score_pixmap = xpm_load(two, XPM_8_8_8_8, &red_score_img);
      break;
    case 3:
      red_score_pixmap = xpm_load(three, XPM_8_8_8_8, &red_score_img);
      break;
    default:
      return 1;
  }

  switch (blue) {
    case 0:
      blue_score_pixmap = xpm_load(zero, XPM_8_8_8_8, &blue_score_img);
      break;
    case 1:
      blue_score_pixmap = xpm_load(one, XPM_8_8_8_8, &blue_score_img);
      break;
    case 2:
      blue_score_pixmap = xpm_load(two, XPM_8_8_8_8, &blue_score_img);
      break;
    case 3:
      blue_score_pixmap = xpm_load(three, XPM_8_8_8_8, &blue_score_img);
      break;
    default:
      return 1;
  }

  //red player score
  draw_pixmap(red_score_pixmap, 534, 88, red_score_img.width, red_score_img.height);

  //blue player score
  draw_pixmap(blue_score_pixmap, 587, 88, blue_score_img.width, blue_score_img.height);

  return 0;
}

int draw_time_board() {
  xpm_image_t time_board_img, min_img, sec_1_img, sec_2_img;

  //time board
  uint8_t *scoreboard_pixmap = xpm_load(time_board, XPM_8_8_8_8, &time_board_img);
  draw_pixmap(scoreboard_pixmap, 532, 136, time_board_img.width, time_board_img.height);

  //minutes digit
  uint8_t *min_pixmap = xpm_load(zero, XPM_8_8_8_8, &min_img);
  draw_pixmap(min_pixmap, 534, 141, min_img.width, min_img.height);

  //seconds 1st digit
  uint8_t *sec_1_pixmap = xpm_load(zero, XPM_8_8_8_8, &sec_1_img);
  draw_pixmap(sec_1_pixmap, 569, 141, sec_1_img.width, sec_1_img.height);

  //seconds 2nd digit
  uint8_t *sec_2_pixmap = xpm_load(zero, XPM_8_8_8_8, &sec_2_img);
  draw_pixmap(sec_2_pixmap, 594, 141, sec_2_img.width, sec_2_img.height);

  return 0;
}

int update_time(int min, int sec1, int sec2) {
  xpm_image_t min_img, sec_1_img, sec_2_img;

  uint8_t *min_pixmap, *sec_1_pixmap, *sec_2_pixmap;

  switch (min) {
    case 0:
      min_pixmap = xpm_load(zero, XPM_8_8_8_8, &min_img);
      break;
    case 1:
      min_pixmap = xpm_load(one, XPM_8_8_8_8, &min_img);
      break;
    case 2:
      min_pixmap = xpm_load(two, XPM_8_8_8_8, &min_img);
      break;
    case 3:
      min_pixmap = xpm_load(three, XPM_8_8_8_8, &min_img);
      break;
    default:
      return 1;
  }

  switch (sec1) {
    case 0:
      sec_1_pixmap = xpm_load(zero, XPM_8_8_8_8, &sec_1_img);
      break;
    case 1:
      sec_1_pixmap = xpm_load(one, XPM_8_8_8_8, &sec_1_img);
      break;
    case 2:
      sec_1_pixmap = xpm_load(two, XPM_8_8_8_8, &sec_1_img);
      break;
    case 3:
      sec_1_pixmap = xpm_load(three, XPM_8_8_8_8, &sec_1_img);
      break;
    case 4:
      sec_1_pixmap = xpm_load(four, XPM_8_8_8_8, &sec_1_img);
      break;
    case 5:
      sec_1_pixmap = xpm_load(five, XPM_8_8_8_8, &sec_1_img);
      break;
    default:
      return 1;
  }

  switch (sec2) {
    case 0:
      sec_2_pixmap = xpm_load(zero, XPM_8_8_8_8, &sec_2_img);
      break;
    case 1:
      sec_2_pixmap = xpm_load(one, XPM_8_8_8_8, &sec_2_img);
      break;
    case 2:
      sec_2_pixmap = xpm_load(two, XPM_8_8_8_8, &sec_2_img);
      break;
    case 3:
      sec_2_pixmap = xpm_load(three, XPM_8_8_8_8, &sec_2_img);
      break;
    case 4:
      sec_2_pixmap = xpm_load(four, XPM_8_8_8_8, &sec_2_img);
      break;
    case 5:
      sec_2_pixmap = xpm_load(five, XPM_8_8_8_8, &sec_2_img);
      break;
    case 6:
      sec_2_pixmap = xpm_load(six, XPM_8_8_8_8, &sec_2_img);
      break;
    case 7:
      sec_2_pixmap = xpm_load(seven, XPM_8_8_8_8, &sec_2_img);
      break;
    case 8:
      sec_2_pixmap = xpm_load(eight, XPM_8_8_8_8, &sec_2_img);
      break;
    case 9:
      sec_2_pixmap = xpm_load(nine, XPM_8_8_8_8, &sec_2_img);
      break;
    default:
      return 1;
  }

  //minutes digit
  draw_pixmap(min_pixmap, 534, 141, min_img.width, min_img.height);

  //seconds 1st digit
  draw_pixmap(sec_1_pixmap, 569, 141, sec_1_img.width, sec_1_img.height);

  //seconds 2nd digit
  draw_pixmap(sec_2_pixmap, 594, 141, sec_2_img.width, sec_2_img.height);

  return 0;
}

int draw_winner_message(winner win_player) {
  xpm_image_t img;
  uint8_t *msg_pixmap;

  switch (win_player) {
    case RED:
      msg_pixmap = xpm_load(red_win, XPM_8_8_8_8, &img);
      break;
    case BLUE:
      msg_pixmap = xpm_load(blue_win, XPM_8_8_8_8, &img);
      break;
    default:
      return 1;
  }

  draw_pixmap(msg_pixmap, 248, 745, img.width, img.height);

  return 0;
}
