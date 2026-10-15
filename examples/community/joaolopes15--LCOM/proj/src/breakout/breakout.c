#include "breakout.h"
#include "../assets/ball_xpm.h"
#include "../assets/bar_xpm.h"
#include "../assets/blue_brick_2_xpm.h"
#include "../assets/blue_brick_xpm.h"
#include "../assets/green_brick_2_xpm.h"
#include "../assets/green_brick_xpm.h"
#include "../assets/lives_xpm.h"
#include "../assets/numbers/number_eight_xpm.h"
#include "../assets/numbers/number_five_xpm.h"
#include "../assets/numbers/number_four_xpm.h"
#include "../assets/numbers/number_nine_xpm.h"
#include "../assets/numbers/number_one_xpm.h"
#include "../assets/numbers/number_seven_xpm.h"
#include "../assets/numbers/number_six_xpm.h"
#include "../assets/numbers/number_three_xpm.h"
#include "../assets/numbers/number_two_xpm.h"
#include "../assets/numbers/number_zero_xpm.h"
#include "../assets/orange_brick_2_xpm.h"
#include "../assets/orange_brick_xpm.h"
#include "../assets/red_brick_2_xpm.h"
#include "../assets/red_brick_xpm.h"
#include "../assets/x2powerup_xpm.h"
#include "../assets/yellow_brick_2_xpm.h"
#include "../assets/yellow_brick_xpm.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

breakout_t *breakout_init() {
  breakout_t *breakout = (breakout_t *) malloc(sizeof(breakout_t));

  if (breakout == NULL) {
    return NULL;
  }

  breakout->bar = create_sprite((xpm_map_t) bar_xpm);
  if (breakout->bar == NULL) {
    destroy_sprite(breakout->bar);
    free(breakout);
    return NULL;
  }

  breakout->bar->x = 350;
  breakout->bar->y = 500;

  breakout->ball = create_sprite((xpm_map_t) ball_xpm);

  breakout->lives = 3;

  breakout->score = 0;

  for (int i = 0; i < 60; i++) {
    breakout->brick_animating[i] = false;
    breakout->brick_anim_frames[i] = 0;
  }

  for (int i = 0; i < 12; i++) {
    breakout->red_animated_bricks[i] = NULL;
  }

  for (int i = 0; i < 12; i++) {
    breakout->orange_animated_bricks[i] = NULL;
  }

  for (int i = 0; i < 12; i++) {
    breakout->yellow_animated_bricks[i] = NULL;
  }

  for (int i = 0; i < 12; i++) {
    breakout->green_animated_bricks[i] = NULL;
  }

  for (int i = 0; i < 12; i++) {
    breakout->blue_animated_bricks[i] = NULL;
  }

  create_bricks(breakout);

  if (breakout->ball == NULL) {
    destroy_sprite(breakout->ball);
    free(breakout);
    return NULL;
  }

  breakout->ball->x = 395;
  breakout->ball->y = 485;
  breakout->ball->xspeed = 0;
  breakout->ball->yspeed = 0;
  breakout->ball_attached = true;

  breakout->balls[0] = breakout->ball;
  breakout->active_balls[0] = true;
  for (int i = 1; i < 5; i++) {
    breakout->balls[i] = NULL;
    breakout->active_balls[i] = false;
  }

  for (int i = 0; i < 10; i++) {
    breakout->powerups[i] = NULL;
    breakout->active_powerups[i] = false;
  }

  return breakout;
}

void create_bricks(breakout_t *breakout) {
  xpm_map_t red_brick_xpms[] = {(xpm_map_t) red_brick_xpm, (xpm_map_t) red_brick_2_xpm};
  xpm_map_t orange_brick_xpms[] = {(xpm_map_t) orange_brick_xpm, (xpm_map_t) orange_brick_2_xpm};
  xpm_map_t yellow_brick_xpms[] = {(xpm_map_t) yellow_brick_xpm, (xpm_map_t) yellow_brick_2_xpm};
  xpm_map_t green_brick_xpms[] = {(xpm_map_t) green_brick_xpm, (xpm_map_t) green_brick_2_xpm};
  xpm_map_t blue_brick_xpms[] = {(xpm_map_t) blue_brick_xpm, (xpm_map_t) blue_brick_2_xpm};

  for (int i = 0; i < 12; i++) {
    breakout->bricks[i] = create_sprite((xpm_map_t) red_brick_xpm);
    if (breakout->bricks[i] == NULL) {
      destroy_sprite(breakout->bricks[i]);
      free(breakout);
      return;
    }

    breakout->red_animated_bricks[i] = create_animated_sprite(red_brick_xpms, 2, 10);
    if (breakout->red_animated_bricks[i] == NULL) {
      destroy_sprite(breakout->bricks[i]);
      free(breakout);
      return;
    }

    int total_width = 12 * breakout->bricks[i]->width + 9 * 12;
    int start_x = (800 - total_width) / 2;
    breakout->bricks[i]->x = start_x + i * (breakout->bricks[i]->width + 10);
    breakout->bricks[i]->y = 100;

    breakout->red_animated_bricks[i]->sp->x = breakout->bricks[i]->x;
    breakout->red_animated_bricks[i]->sp->y = breakout->bricks[i]->y;
  }

  for (int i = 12; i < 24; i++) {
    breakout->bricks[i] = create_sprite((xpm_map_t) orange_brick_xpm);
    if (breakout->bricks[i] == NULL) {
      destroy_sprite(breakout->bricks[i]);
      free(breakout);
      return;
    }

    breakout->orange_animated_bricks[i - 12] = create_animated_sprite(orange_brick_xpms, 2, 10);
    if (breakout->orange_animated_bricks[i - 12] == NULL) {
      destroy_sprite(breakout->bricks[i]);
      free(breakout);
      return;
    }

    int total_width = 12 * breakout->bricks[i]->width + 9 * 12;
    int start_x = (800 - total_width) / 2;
    breakout->bricks[i]->x = start_x + (i - 12) * (breakout->bricks[i]->width + 10);
    breakout->bricks[i]->y = 130;

    breakout->orange_animated_bricks[i - 12]->sp->x = breakout->bricks[i]->x;
    breakout->orange_animated_bricks[i - 12]->sp->y = breakout->bricks[i]->y;
  }

  for (int i = 24; i < 36; i++) {
    breakout->bricks[i] = create_sprite((xpm_map_t) yellow_brick_xpm);
    if (breakout->bricks[i] == NULL) {
      destroy_sprite(breakout->bricks[i]);
      free(breakout);
      return;
    }

    breakout->yellow_animated_bricks[i - 24] = create_animated_sprite(yellow_brick_xpms, 2, 10);
    if (breakout->yellow_animated_bricks[i - 24] == NULL) {
      destroy_sprite(breakout->bricks[i]);
      free(breakout);
      return;
    }

    int total_width = 12 * breakout->bricks[i]->width + 9 * 12;
    int start_x = (800 - total_width) / 2;
    breakout->bricks[i]->x = start_x + (i - 24) * (breakout->bricks[i]->width + 10);
    breakout->bricks[i]->y = 160;

    breakout->yellow_animated_bricks[i - 24]->sp->x = breakout->bricks[i]->x;
    breakout->yellow_animated_bricks[i - 24]->sp->y = breakout->bricks[i]->y;
  }

  for (int i = 36; i < 48; i++) {
    breakout->bricks[i] = create_sprite((xpm_map_t) green_brick_xpm);
    if (breakout->bricks[i] == NULL) {
      destroy_sprite(breakout->bricks[i]);
      free(breakout);
      return;
    }

    breakout->green_animated_bricks[i - 36] = create_animated_sprite(green_brick_xpms, 2, 10);
    if (breakout->green_animated_bricks[i - 36] == NULL) {
      destroy_sprite(breakout->bricks[i]);
      free(breakout);
      return;
    }

    int total_width = 12 * breakout->bricks[i]->width + 9 * 12;
    int start_x = (800 - total_width) / 2;
    breakout->bricks[i]->x = start_x + (i - 36) * (breakout->bricks[i]->width + 10);
    breakout->bricks[i]->y = 190;

    breakout->green_animated_bricks[i - 36]->sp->x = breakout->bricks[i]->x;
    breakout->green_animated_bricks[i - 36]->sp->y = breakout->bricks[i]->y;
  }

  for (int i = 48; i < 60; i++) {
    breakout->bricks[i] = create_sprite((xpm_map_t) blue_brick_xpm);
    if (breakout->bricks[i] == NULL) {
      destroy_sprite(breakout->bricks[i]);
      free(breakout);
      return;
    }

    breakout->blue_animated_bricks[i - 48] = create_animated_sprite(blue_brick_xpms, 2, 10);
    if (breakout->blue_animated_bricks[i - 48] == NULL) {
      destroy_sprite(breakout->bricks[i]);
      free(breakout);
      return;
    }

    int total_width = 12 * breakout->bricks[i]->width + 9 * 12;
    int start_x = (800 - total_width) / 2;
    breakout->bricks[i]->x = start_x + (i - 48) * (breakout->bricks[i]->width + 10);
    breakout->bricks[i]->y = 220;

    breakout->blue_animated_bricks[i - 48]->sp->x = breakout->bricks[i]->x;
    breakout->blue_animated_bricks[i - 48]->sp->y = breakout->bricks[i]->y;
  }

  for (int i = 0; i < 60; i++) {
    breakout->active_bricks[i] = true;
    breakout->brick_animating[i] = false;
    breakout->brick_anim_frames[i] = 0;
  }
}

int draw_bricks(breakout_t *breakout) {
  for (int i = 0; i < 60; i++) {
    if (breakout->active_bricks[i]) {
      if (draw_sprite(breakout->bricks[i], breakout->bricks[i]->x, breakout->bricks[i]->y) != 0) {
        return 1;
      }
    }
    else if (breakout->brick_animating[i]) {
      if (i < 12 && breakout->red_animated_bricks[i] != NULL) {
        animate_sprite_once(breakout->red_animated_bricks[i]);
        if (draw_animated_sprite(breakout->red_animated_bricks[i],
                                 breakout->bricks[i]->x, breakout->bricks[i]->y) != 0) {
          return 1;
        }

        breakout->brick_anim_frames[i]--;
        if (breakout->brick_anim_frames[i] <= 0) {
          breakout->brick_animating[i] = false;
        }
      }
      else if (i >= 12 && i < 24 && breakout->orange_animated_bricks[i - 12] != NULL) {
        animate_sprite_once(breakout->orange_animated_bricks[i - 12]);
        if (draw_animated_sprite(breakout->orange_animated_bricks[i - 12],
                                 breakout->bricks[i]->x, breakout->bricks[i]->y) != 0) {
          return 1;
        }

        breakout->brick_anim_frames[i]--;
        if (breakout->brick_anim_frames[i] <= 0) {
          breakout->brick_animating[i] = false;
        }
      }
      else if (i >= 24 && i < 36 && breakout->yellow_animated_bricks[i - 24] != NULL) {
        animate_sprite_once(breakout->yellow_animated_bricks[i - 24]);
        if (draw_animated_sprite(breakout->yellow_animated_bricks[i - 24],
                                 breakout->bricks[i]->x, breakout->bricks[i]->y) != 0) {
          return 1;
        }

        breakout->brick_anim_frames[i]--;
        if (breakout->brick_anim_frames[i] <= 0) {
          breakout->brick_animating[i] = false;
        }
      }
      else if (i >= 36 && i < 48 && breakout->green_animated_bricks[i - 36] != NULL) {
        animate_sprite_once(breakout->green_animated_bricks[i - 36]);
        if (draw_animated_sprite(breakout->green_animated_bricks[i - 36],
                                 breakout->bricks[i]->x, breakout->bricks[i]->y) != 0) {
          return 1;
        }

        breakout->brick_anim_frames[i]--;
        if (breakout->brick_anim_frames[i] <= 0) {
          breakout->brick_animating[i] = false;
        }
      }
      else if (i >= 48 && i < 60 && breakout->blue_animated_bricks[i - 48] != NULL) {
        animate_sprite_once(breakout->blue_animated_bricks[i - 48]);
        if (draw_animated_sprite(breakout->blue_animated_bricks[i - 48],
                                 breakout->bricks[i]->x, breakout->bricks[i]->y) != 0) {
          return 1;
        }

        breakout->brick_anim_frames[i]--;
        if (breakout->brick_anim_frames[i] <= 0) {
          breakout->brick_animating[i] = false;
        }
      }
    }
  }
  return 0;
}

int draw_lives(breakout_t *breakout) {
  for (int i = 0; i < breakout->lives; i++) {
    if (draw_xpm((xpm_map_t) lives_xpm, (i * 15) + 5, 5) != 0) {
      return 1;
    }
  }
  return 0;
}

int draw_score(breakout_t *breakout) {
  const xpm_map_t digit_xpms[] = {
    (xpm_map_t) number_zero_xpm, (xpm_map_t) number_one_xpm, (xpm_map_t) number_two_xpm,
    (xpm_map_t) number_three_xpm, (xpm_map_t) number_four_xpm, (xpm_map_t) number_five_xpm,
    (xpm_map_t) number_six_xpm, (xpm_map_t) number_seven_xpm, (xpm_map_t) number_eight_xpm,
    (xpm_map_t) number_nine_xpm};

  if (breakout->score == 0) {
    if (draw_xpm(digit_xpms[0], 395, 5)) {
      return 1;
    }
    return 0;
  }

  char score_str[20];
  sprintf(score_str, "%d", breakout->score);
  int num_digits = strlen(score_str);

  int digit_width = 10;
  int total_width = num_digits * digit_width;
  int start_x = 400 - (total_width / 2);

  for (int i = 0; i < num_digits; i++) {
    int digit = score_str[i] - '0';
    if (draw_xpm(digit_xpms[digit], start_x + (i * digit_width), 5)) {
      return 1;
    }
  }

  return 0;
}

int draw_powerup(breakout_t *breakout) {
  for (int i = 0; i < 10; i++) {
    if (breakout->active_powerups[i] && breakout->powerups[i] != NULL) {
      if (draw_sprite(breakout->powerups[i], breakout->powerups[i]->x, breakout->powerups[i]->y) != 0) {
        return 1;
      }
    }
  }
  return 0;
}

void spawn_powerup(breakout_t *breakout, int x, int y) {
  if (breakout == NULL)
    return;

  for (int i = 0; i < 10; i++) {
    if (!breakout->active_powerups[i]) {
      breakout->powerups[i] = create_sprite((xpm_map_t) x2powerup_xpm);
      if (breakout->powerups[i] != NULL) {
        if (x < 0) {
          breakout->powerups[i]->x = 50 + (rand() % 701);
        }
        else {
          breakout->powerups[i]->x = x;
        }

        if (y < 0) {
          breakout->powerups[i]->y = 250 + (rand() % 200);
        }
        else {
          breakout->powerups[i]->y = y;
        }

        breakout->powerups[i]->yspeed = 2;
        breakout->active_powerups[i] = true;
        break;
      }
    }
  }
}

void update_powerups(breakout_t *breakout) {
  if (breakout == NULL)
    return;

  for (int i = 0; i < 10; i++) {
    if (breakout->active_powerups[i] && breakout->powerups[i] != NULL) {
      breakout->powerups[i]->y += breakout->powerups[i]->yspeed;

      if (breakout->powerups[i]->y > 550) {
        destroy_sprite(breakout->powerups[i]);
        breakout->powerups[i] = NULL;
        breakout->active_powerups[i] = false;
      }
    }
  }
}

int draw_breakout(breakout_t *breakout) {
  if (breakout == NULL || breakout->bar == NULL || breakout->ball == NULL) {
    return 1;
  }

  if (draw_sprite(breakout->bar, breakout->bar->x, breakout->bar->y) != 0) {
    return 1;
  }

  draw_bricks(breakout);

  draw_lives(breakout);

  draw_score(breakout);

  update_powerups(breakout);
  handle_powerup_collisions(breakout);
  draw_powerup(breakout);

  for (int i = 0; i < 5; i++) {
    if (breakout->active_balls[i] && breakout->balls[i] != NULL) {
      if (draw_sprite(breakout->balls[i], breakout->balls[i]->x, breakout->balls[i]->y) != 0) {
        return 1;
      }
    }
  }

  int count = 0;

  for (int i = 0; i < 60; i++) {
    if (breakout->active_bricks[i] == false) {
      count++;
    }
  }

  if (count == 60) {
    for (int i = 0; i < 12; i++) {
      if (breakout->red_animated_bricks[i] != NULL) {
        destroy_animated_sprite(breakout->red_animated_bricks[i]);
        breakout->red_animated_bricks[i] = NULL;
      }
    }

    for (int i = 0; i < 12; i++) {
      if (breakout->orange_animated_bricks[i] != NULL) {
        destroy_animated_sprite(breakout->orange_animated_bricks[i]);
        breakout->orange_animated_bricks[i] = NULL;
      }
    }

    for (int i = 0; i < 12; i++) {
      if (breakout->yellow_animated_bricks[i] != NULL) {
        destroy_animated_sprite(breakout->yellow_animated_bricks[i]);
        breakout->yellow_animated_bricks[i] = NULL;
      }
    }

    for (int i = 0; i < 12; i++) {
      if (breakout->green_animated_bricks[i] != NULL) {
        destroy_animated_sprite(breakout->green_animated_bricks[i]);
        breakout->green_animated_bricks[i] = NULL;
      }
    }

    for (int i = 0; i < 12; i++) {
      if (breakout->blue_animated_bricks[i] != NULL) {
        destroy_animated_sprite(breakout->blue_animated_bricks[i]);
        breakout->blue_animated_bricks[i] = NULL;
      }
    }

    create_bricks(breakout);
    draw_bricks(breakout);

    breakout->ball->x = 395;
    breakout->ball->y = 485;
    breakout->ball->xspeed = 3;
    breakout->ball->yspeed = -3;
  }

  return 0;
}

void handle_powerup_collisions(breakout_t *breakout) {
  if (breakout == NULL || breakout->bar == NULL)
    return;

  for (int i = 0; i < 10; i++) {
    if (breakout->active_powerups[i] && breakout->powerups[i] != NULL) {
      if (breakout->powerups[i]->y + breakout->powerups[i]->height >= breakout->bar->y &&
          breakout->powerups[i]->y <= breakout->bar->y + breakout->bar->height &&
          breakout->powerups[i]->x + breakout->powerups[i]->width >= breakout->bar->x &&
          breakout->powerups[i]->x <= breakout->bar->x + breakout->bar->width) {

        breakout->score += 500;

        spawn_extra_ball(breakout);

        destroy_sprite(breakout->powerups[i]);
        breakout->powerups[i] = NULL;
        breakout->active_powerups[i] = false;
      }
    }
  }
}

void spawn_extra_ball(breakout_t *breakout) {
  if (breakout == NULL)
    return;

  Sprite *active_ball_list[5];
  int active_count = 0;

  if (breakout->ball != NULL) {
    active_ball_list[active_count] = breakout->ball;
    active_count++;
  }

  for (int j = 1; j < 5; j++) {
    if (breakout->active_balls[j] && breakout->balls[j] != NULL) {
      active_ball_list[active_count] = breakout->balls[j];
      active_count++;
    }
  }

  if (active_count == 0)
    return;

  int source_index = rand() % active_count;
  Sprite *source_ball = active_ball_list[source_index];

  for (int i = 1; i < 5; i++) {
    if (!breakout->active_balls[i]) {
      breakout->balls[i] = create_sprite((xpm_map_t) ball_xpm);
      if (breakout->balls[i] != NULL) {
        breakout->balls[i]->x = source_ball->x;
        breakout->balls[i]->y = source_ball->y;

        if (source_ball->xspeed != 0 || source_ball->yspeed != 0) {
          breakout->balls[i]->xspeed = -source_ball->xspeed + (rand() % 3 - 1);
          breakout->balls[i]->yspeed = source_ball->yspeed + (rand() % 3 - 1);
        }
        else {
          breakout->balls[i]->xspeed = 2 + (rand() % 3);
          breakout->balls[i]->yspeed = -3;
        }

        breakout->active_balls[i] = true;
        break;
      }
    }
  }
}

void destroy_breakout(breakout_t *breakout) {
  if (breakout == NULL) {
    return;
  }

  if (breakout->bar != NULL) {
    destroy_sprite(breakout->bar);
  }

  for (int i = 0; i < 60; i++) {
    if (breakout->bricks[i] != NULL) {
      destroy_sprite(breakout->bricks[i]);
    }
  }

  for (int i = 0; i < 12; i++) {
    if (breakout->red_animated_bricks[i] != NULL) {
      destroy_animated_sprite(breakout->red_animated_bricks[i]);
    }
  }

  for (int i = 0; i < 12; i++) {
    if (breakout->orange_animated_bricks[i] != NULL) {
      destroy_animated_sprite(breakout->orange_animated_bricks[i]);
    }
  }

  for (int i = 0; i < 12; i++) {
    if (breakout->yellow_animated_bricks[i] != NULL) {
      destroy_animated_sprite(breakout->yellow_animated_bricks[i]);
    }
  }

  for (int i = 0; i < 12; i++) {
    if (breakout->green_animated_bricks[i] != NULL) {
      destroy_animated_sprite(breakout->green_animated_bricks[i]);
    }
  }

  for (int i = 0; i < 12; i++) {
    if (breakout->blue_animated_bricks[i] != NULL) {
      destroy_animated_sprite(breakout->blue_animated_bricks[i]);
    }
  }

  if (breakout->ball != NULL) {
    destroy_sprite(breakout->ball);
    for (int i = 0; i < 10; i++) {
      if (breakout->powerups[i] != NULL) {
        destroy_sprite(breakout->powerups[i]);
      }
    }

    for (int i = 0; i < 5; i++) {
      if (breakout->balls[i] != NULL) {
        destroy_sprite(breakout->balls[i]);
      }
    }

    free(breakout);
  }
}

void handle_all_ball_collisions(breakout_t *breakout) {
  if (breakout == NULL || breakout->bar == NULL) {
    return;
  }

  for (int ball_idx = 0; ball_idx < 5; ball_idx++) {
    if (!breakout->active_balls[ball_idx] || breakout->balls[ball_idx] == NULL) {
      continue;
    }

    Sprite *current_ball = breakout->balls[ball_idx];

    if (current_ball->x <= 0 || current_ball->x + current_ball->width >= vmi_p.XResolution) {
      current_ball->xspeed = -current_ball->xspeed;
    }
    if (current_ball->y <= 0) {
      current_ball->yspeed = -current_ball->yspeed;
    }

    if (current_ball->y + current_ball->height >= vmi_p.YResolution) {
      if (ball_idx == 0) {
        breakout->active_balls[ball_idx] = false;
      }
      else {
        destroy_sprite(current_ball);
        breakout->balls[ball_idx] = NULL;
        breakout->active_balls[ball_idx] = false;
      }
      continue;
    }

    if (current_ball->y + current_ball->height >= breakout->bar->y &&
        current_ball->y <= breakout->bar->y + breakout->bar->height &&
        current_ball->x + current_ball->width >= breakout->bar->x &&
        current_ball->x <= breakout->bar->x + breakout->bar->width) {

      int ball_center_x = current_ball->x + current_ball->width / 2;
      int bar_center_x = breakout->bar->x + breakout->bar->width / 2;

      float relative_hit = (float) (ball_center_x - bar_center_x) / (breakout->bar->width / 2);

      if (relative_hit < -1.0f)
        relative_hit = -1.0f;
      if (relative_hit > 1.0f)
        relative_hit = 1.0f;

      float current_speed = sqrt(current_ball->xspeed * current_ball->xspeed +
                                 current_ball->yspeed * current_ball->yspeed);

      float angle = relative_hit * (M_PI / 3.0f);

      current_ball->xspeed = (int) (current_speed * sin(angle));
      current_ball->yspeed = -(int) (current_speed * cos(angle));

      if (current_ball->yspeed > -3) {
        current_ball->yspeed = -3;
      }
    }

    for (int i = 0; i < 60; i++) {
      if (!breakout->active_bricks[i])
        continue;

      if (current_ball->y <= breakout->bricks[i]->y + breakout->bricks[i]->height &&
          current_ball->y + current_ball->height >= breakout->bricks[i]->y &&
          current_ball->x + current_ball->width >= breakout->bricks[i]->x &&
          current_ball->x <= breakout->bricks[i]->x + breakout->bricks[i]->width) {

        if (current_ball->y + current_ball->height - current_ball->yspeed <= breakout->bricks[i]->y ||
            current_ball->y - current_ball->yspeed >= breakout->bricks[i]->y + breakout->bricks[i]->height) {
          current_ball->yspeed = -current_ball->yspeed;
        }
        else {
          current_ball->xspeed = -current_ball->xspeed;
        }

        breakout->score += 100;
        breakout->active_bricks[i] = false;
        breakout->brick_animating[i] = true;
        breakout->brick_anim_frames[i] = 30;

        if ((rand() % 100) < 20) {
          spawn_powerup(breakout, breakout->bricks[i]->x + breakout->bricks[i]->width / 2,
                        breakout->bricks[i]->y + breakout->bricks[i]->height);
        }

        break;
      }
    }
  }

  bool all_balls_inactive = true;
  for (int i = 0; i < 5; i++) {
    if (breakout->active_balls[i]) {
      all_balls_inactive = false;
      break;
    }
  }

  if (all_balls_inactive) {
    breakout->lives--;

    breakout->ball->x = breakout->bar->x + (breakout->bar->width / 2) - (breakout->ball->width / 2);
    breakout->ball->y = 485;
    breakout->ball->xspeed = 0;
    breakout->ball->yspeed = 0;
    breakout->ball_attached = true;
    breakout->active_balls[0] = true;

    for (int i = 1; i < 5; i++) {
      if (breakout->balls[i] != NULL) {
        destroy_sprite(breakout->balls[i]);
        breakout->balls[i] = NULL;
      }
      breakout->active_balls[i] = false;
    }
  }
}

void move_bar_with_ball_keyboard(breakout_t *breakout, int direction) {
  if (breakout == NULL || breakout->bar == NULL || breakout->ball == NULL) {
    return;
  }

  int old_bar_x = breakout->bar->x;

  if (direction < 0) {
    move_sprite_left(breakout->bar);
  }
  else if (direction > 0) {
    move_sprite_right(breakout->bar);
  }

  if (breakout->ball_attached) {
    int bar_movement = breakout->bar->x - old_bar_x;
    breakout->ball->x += bar_movement;
  }
}

void move_bar_with_ball_mouse(breakout_t *breakout, int target_x, int target_y, int xspeed) {
  if (breakout == NULL || breakout->bar == NULL || breakout->ball == NULL) {
    return;
  }

  int old_bar_x = breakout->bar->x;

  move_sprite_mouse(breakout->bar, target_x, target_y, xspeed);

  if (breakout->ball_attached) {
    int bar_movement = breakout->bar->x - old_bar_x;
    breakout->ball->x += bar_movement;
  }
}
