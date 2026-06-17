#include "app.h"

#include "assets.h"
#include "renderer.h"

#include <lcom/i8042.h>

#include <stdio.h>

enum {
  STATE_MENU = 0,
  STATE_PLAYING = 1,
  STATE_GAME_OVER = 2
};

enum {
  SCREEN_W = 800,
  SCREEN_H = 600,
  GROUND_Y = 520,
  BIRD_X = 146,
  BIRD_W = 48,
  BIRD_H = 36,
  PIPE_W = 72,
  PIPE_SPACING = 292,
  PIPE_START_X = 820,
  GAP_H = 154,
  FP_SHIFT = 4,
  FP_ONE = 1 << FP_SHIFT,
  GRAVITY_FP = 5,
  FLAP_VELOCITY_FP = -96,
  MAX_FALL_VELOCITY_FP = 122,
  PIPE_SPEED_FP = 34
};

static int overlaps(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh) {
  return ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by;
}

static int pipe_gap_for(int seed) {
  int wave = (seed * 67 + seed * seed * 13) % 254;
  return 112 + wave;
}

static void setup_pipe(pipe_pair_t *pipe, int x, int seed) {
  pipe->x_fp = x * FP_ONE;
  pipe->gap_y = pipe_gap_for(seed);
  pipe->passed = 0;
}

static void reset_round(app_t *app) {
  app->bird_y_fp = 244 * FP_ONE;
  app->bird_vy_fp = 0;
  for (int i = 0; i < 3; i++) {
    setup_pipe(&app->pipes[i], PIPE_START_X + i * PIPE_SPACING, i + 1);
  }
  app->score = 0;
  app->frame = 0;
  app->ground_scroll = 0;
}

static void finish_round(app_t *app) {
  if (app->score > app->best_score) app->best_score = app->score;
  app->state = STATE_GAME_OVER;
  menu_set(&app->menu, "GAME OVER", "RETRY", "QUIT");
}

static app_input_result_t start_round(app_t *app) {
  reset_round(app);
  app->state = STATE_PLAYING;
  app->bird_vy_fp = FLAP_VELOCITY_FP;
  return APP_INPUT_FLAP;
}

static app_input_result_t handle_menu_action(app_t *app, menu_action_t action) {
  if (action == MENU_ACTION_PRIMARY) return start_round(app);
  if (action == MENU_ACTION_QUIT) {
    app->running = 0;
    return APP_INPUT_QUIT;
  }
  return APP_INPUT_NONE;
}

static const char *const *bird_sprite_for_vy(int vy) {
  if (vy < -80) return bird_climb_hard_xpm;
  if (vy < -18) return bird_climb_xpm;
  if (vy > 80) return bird_dive_hard_xpm;
  if (vy > 28) return bird_dive_xpm;
  return bird_flat_xpm;
}

void app_init(app_t *app) {
  reset_round(app);
  app->state = STATE_MENU;
  app->running = 1;
  app->best_score = 0;
  menu_init(&app->menu);
  menu_set(&app->menu, "LCOM BIRD", "PLAY", "QUIT");
}

app_input_result_t app_key(app_t *app, uint8_t scancode) {
  if (app->state == STATE_MENU || app->state == STATE_GAME_OVER) {
    return handle_menu_action(app, menu_key(&app->menu, scancode));
  }

  int make = (scancode & KBD_BREAK_BIT) == 0;
  uint8_t code = (uint8_t)(scancode & ~KBD_BREAK_BIT);
  if (!make) return scancode == ESC_BREAK ? APP_INPUT_QUIT : APP_INPUT_NONE;
  if (code == 0x39 || code == 0x48) {
    app->bird_vy_fp = FLAP_VELOCITY_FP;
    return APP_INPUT_FLAP;
  }
  return APP_INPUT_NONE;
}

app_input_result_t app_mouse(app_t *app, int x, int y, int left_down) {
  if (app->state == STATE_MENU || app->state == STATE_GAME_OVER) {
    return handle_menu_action(app, menu_mouse(&app->menu, x, y, left_down));
  }

  app->menu.pointer_x = x;
  app->menu.pointer_y = y;
  int pressed = left_down && !app->menu.pointer_down;
  app->menu.pointer_down = left_down;
  if (pressed) {
    app->bird_vy_fp = FLAP_VELOCITY_FP;
    return APP_INPUT_FLAP;
  }
  return APP_INPUT_NONE;
}

void app_update(app_t *app) {
  app->frame++;
  app->ground_scroll = (app->ground_scroll + 3) % 48;
  if (app->state != STATE_PLAYING) return;

  app->bird_vy_fp += GRAVITY_FP;
  if (app->bird_vy_fp > MAX_FALL_VELOCITY_FP) app->bird_vy_fp = MAX_FALL_VELOCITY_FP;
  app->bird_y_fp += app->bird_vy_fp;

  int rightmost_x = 0;
  for (int i = 0; i < 3; i++) {
    int x = app->pipes[i].x_fp / FP_ONE;
    if (x > rightmost_x) rightmost_x = x;
  }

  for (int i = 0; i < 3; i++) {
    pipe_pair_t *pipe = &app->pipes[i];
    pipe->x_fp -= PIPE_SPEED_FP;
    int x = pipe->x_fp / FP_ONE;
    if (!pipe->passed && x + PIPE_W < BIRD_X) {
      pipe->passed = 1;
      app->score++;
      if (app->score > app->best_score) app->best_score = app->score;
    }
    if (x < -PIPE_W - 8) {
      rightmost_x += PIPE_SPACING;
      setup_pipe(pipe, rightmost_x, app->score + i + 3);
    }
  }

  int bird_y = app->bird_y_fp / FP_ONE;
  if (bird_y < 0 || bird_y + BIRD_H > GROUND_Y) {
    finish_round(app);
    return;
  }
  for (int i = 0; i < 3; i++) {
    int pipe_x = app->pipes[i].x_fp / FP_ONE;
    int gap_y = app->pipes[i].gap_y;
    if (overlaps(BIRD_X, bird_y, BIRD_W, BIRD_H, pipe_x, 0, PIPE_W, gap_y) ||
        overlaps(BIRD_X, bird_y, BIRD_W, BIRD_H, pipe_x, gap_y + GAP_H,
                 PIPE_W, GROUND_Y - (gap_y + GAP_H))) {
      finish_round(app);
      return;
    }
  }
}

static void text_shadow(draw_context_t *ctx, int x, int y, const char *text, uint32_t color, int scale) {
  draw_text(ctx, x + scale, y + scale, text, 0x101820u, scale);
  draw_text(ctx, x, y, text, color, scale);
}

static void draw_cloud(draw_context_t *ctx, int x, int y, int scale) {
  uint32_t shade = 0xC9E9F2u;
  uint32_t white = 0xF2FBFFu;
  fill_rect(ctx, x + 8 * scale, y + 8 * scale, 44 * scale, 14 * scale, white);
  fill_rect(ctx, x + 18 * scale, y, 20 * scale, 22 * scale, white);
  fill_rect(ctx, x, y + 14 * scale, 62 * scale, 12 * scale, shade);
  fill_rect(ctx, x + 8 * scale, y + 10 * scale, 44 * scale, 10 * scale, white);
}

static void draw_background(draw_context_t *ctx, const app_t *app) {
  fill_rect(ctx, 0, 0, SCREEN_W, 150, 0x83D6F0u);
  fill_rect(ctx, 0, 150, SCREEN_W, 190, 0x6CC7E7u);
  fill_rect(ctx, 0, 340, SCREEN_W, 180, 0x88D9B2u);

  for (int i = 0; i < 5; i++) {
    int x = (i * 210 - (app->frame / 2) % 210) - 80;
    int y = 58 + (i % 3) * 36;
    draw_cloud(ctx, x, y, i % 2 == 0 ? 1 : 2);
  }

  for (int x = -80; x < SCREEN_W + 80; x += 140) {
    fill_rect(ctx, x, 410, 92, 110, 0x4AA06Cu);
    fill_rect(ctx, x + 28, 380, 72, 140, 0x5DBE7Au);
  }
  fill_rect(ctx, 0, GROUND_Y, SCREEN_W, SCREEN_H - GROUND_Y, 0x5EA85Au);
  fill_rect(ctx, 0, GROUND_Y, SCREEN_W, 8, 0xA5E36Cu);
  for (int x = -app->ground_scroll; x < SCREEN_W; x += 48) {
    fill_rect(ctx, x, GROUND_Y + 18, 28, 8, 0x397647u);
    fill_rect(ctx, x + 12, GROUND_Y + 42, 30, 7, 0x397647u);
  }
}

static void draw_pipe_pair(draw_context_t *ctx, const pipe_pair_t *pipe) {
  int x = pipe->x_fp / FP_ONE;
  int gap_y = pipe->gap_y;
  uint32_t dark = 0x246E3Cu;
  uint32_t mid = 0x37A858u;
  uint32_t light = 0x76D67Fu;
  fill_rect(ctx, x + 8, 0, PIPE_W - 16, gap_y, mid);
  fill_rect(ctx, x + 8, gap_y + GAP_H, PIPE_W - 16, GROUND_Y - gap_y - GAP_H, mid);
  fill_rect(ctx, x + 8, 0, 8, gap_y, dark);
  fill_rect(ctx, x + PIPE_W - 18, 0, 8, gap_y, light);
  fill_rect(ctx, x + 8, gap_y + GAP_H, 8, GROUND_Y - gap_y - GAP_H, dark);
  fill_rect(ctx, x + PIPE_W - 18, gap_y + GAP_H, 8, GROUND_Y - gap_y - GAP_H, light);
  fill_rect(ctx, x, gap_y - 24, PIPE_W, 24, dark);
  fill_rect(ctx, x + 4, gap_y - 20, PIPE_W - 8, 16, light);
  fill_rect(ctx, x, gap_y + GAP_H, PIPE_W, 24, dark);
  fill_rect(ctx, x + 4, gap_y + GAP_H + 4, PIPE_W - 8, 16, light);
}

void app_render(app_t *app, draw_context_t *ctx) {
  draw_background(ctx, app);

  for (int i = 0; i < 3; i++) {
    draw_pipe_pair(ctx, &app->pipes[i]);
  }

  int bird_y = app->bird_y_fp / FP_ONE;
  draw_xpm(ctx, bird_sprite_for_vy(app->bird_vy_fp), BIRD_X, bird_y, 3);

  char score[32];
  snprintf(score, sizeof(score), "SCORE %d", app->score);
  text_shadow(ctx, 24, 24, score, 0xFFFFFFu, 3);
  if (app->best_score > 0) {
    char best[32];
    snprintf(best, sizeof(best), "BEST %d", app->best_score);
    text_shadow(ctx, SCREEN_W - 24 - (int)font_text_width(best, 2), 28, best, 0xEFF6FFu, 2);
  }

  if (app->state == STATE_MENU) {
    menu_set(&app->menu, "LCOM BIRD", "PLAY", "QUIT");
    menu_render(&app->menu, ctx, app->best_score);
  } else if (app->state == STATE_GAME_OVER) {
    menu_set(&app->menu, "GAME OVER", "RETRY", "QUIT");
    menu_render(&app->menu, ctx, app->best_score);
  }
}
