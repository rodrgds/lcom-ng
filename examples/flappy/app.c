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
  app->paused = false;
  app->menu.selected = 0;
  app->state = STATE_GAME_OVER;
  menu_set(&app->menu, "GAME OVER", "RETRY", "QUIT");
}

static app_input_result_t start_round(app_t *app) {
  reset_round(app);
  app->paused = false;
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
  *app = (app_t){.mouse_x = 400, .mouse_y = 300};
  reset_round(app);
  app->state = STATE_MENU;
  app->running = 1;
  app->best_score = 0;
  menu_init(&app->menu);
  menu_set(&app->menu, "LCOM BIRD", "PLAY", "QUIT");
}

app_input_result_t app_key(app_t *app, uint8_t scancode) {
  bool extended = app->extended;
  if (scancode == 0xE0) app->extended = true;
  else app->extended = false;
  if (example_modifier_byte(&app->modifiers, scancode) || scancode == 0xE0) return APP_INPUT_NONE;
  if (scancode & KBD_BREAK_BIT) return APP_INPUT_NONE;
  if (example_primary(&app->modifiers)) return APP_INPUT_NONE;
  if (!extended && (scancode == 0x3B || scancode == 0x23)) {
    app->help = !app->help;
    return APP_INPUT_NONE;
  }
  if (app->help) {
    if (scancode == 0x01) app->help = false;
    return APP_INPUT_NONE;
  }
  if (scancode == 0x01 || scancode == 0x10) return APP_INPUT_QUIT;
  if (app->state == STATE_MENU || app->state == STATE_GAME_OVER) {
    if (scancode == 0x13 && app->state == STATE_GAME_OVER) return start_round(app);
    return handle_menu_action(app, menu_key(&app->menu, scancode));
  }
  if (scancode == 0x19) {
    app->paused = !app->paused;
    return APP_INPUT_NONE;
  }
  if (app->paused) {
    if (scancode == 0x39 || scancode == 0x1C) app->paused = false;
    return APP_INPUT_NONE;
  }
  if (scancode == 0x39 || (extended && scancode == 0x48)) {
    app->bird_vy_fp = FLAP_VELOCITY_FP;
    return APP_INPUT_FLAP;
  }
  return APP_INPUT_NONE;
}

app_input_result_t app_mouse(app_t *app, int dx, int dy, bool down) {
  app->mouse_x += dx;
  app->mouse_y -= dy;
  if (app->mouse_x < 0) app->mouse_x = 0;
  if (app->mouse_x > 799) app->mouse_x = 799;
  if (app->mouse_y < 0) app->mouse_y = 0;
  if (app->mouse_y > 599) app->mouse_y = 599;
  bool clicked = down && !app->mouse_down;
  app->mouse_down = down;
  if (dx || dy) app->mouse_visible = true;
  if (app->help) return APP_INPUT_NONE;
  if (app->state != STATE_PLAYING)
    return handle_menu_action(app, menu_mouse(&app->menu, app->mouse_x, app->mouse_y, clicked));
  if (!clicked) return APP_INPUT_NONE;
  if (app->paused) { app->paused = false; return APP_INPUT_NONE; }
  app->bird_vy_fp = FLAP_VELOCITY_FP;
  return APP_INPUT_FLAP;
}

void app_update(app_t *app) {
  if (app->paused || app->help) return;
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

  if (app->state == STATE_PLAYING) {
    char score[32];
    snprintf(score, sizeof(score), "%d", app->score);
    fill_rect(ctx, 24, 20, 118, 54, 0x203742u);
    draw_text(ctx, 36, 30, score, 0xFFF3C4u, 4);
    char best[32];
    snprintf(best, sizeof(best), "BEST %d", app->best_score);
    text_shadow(ctx, SCREEN_W - 24 - (int)font_text_width(best, 2), 30, best, 0xFFFFFFu, 2);
  } else {
    menu_set(&app->menu, app->state == STATE_GAME_OVER ? "GAME OVER" : "LCOM BIRD",
             app->state == STATE_GAME_OVER ? "TRY AGAIN" : "PLAY", "QUIT");
    menu_render(&app->menu, ctx, app->best_score);
    if (app->state == STATE_GAME_OVER) {
      char score[32];
      snprintf(score, sizeof(score), "YOUR SCORE %d", app->score);
      fill_rect(ctx, 270, 218, 270, 24, 0x203742u);
      draw_text(ctx, (800 - (int)font_text_width(score, 2)) / 2, 224, score, 0xEAF7F0u, 2);
    }
  }
  fill_rect(ctx, 0, 556, 800, 44, 0x203742u);
  draw_text(ctx, 26, 568, "SPACE / UP / CLICK  FLAP    P  PAUSE    F1  HELP    ESC  QUIT", 0xEAF7F0u, 1);
  if (app->paused) {
    fill_rect(ctx, 224, 210, 352, 172, 0x203742u);
    draw_text(ctx, 292, 238, "PAUSED", 0xFFF3C4u, 5);
    draw_text(ctx, 274, 308, "P / SPACE  RESUME", 0xEAF7F0u, 2);
    draw_text(ctx, 298, 346, "ESC  QUIT", 0xBDD5DCu, 2);
  }
  if (app->help) {
    fill_rect(ctx, 120, 114, 560, 372, 0x203742u);
    draw_text(ctx, 174, 140, "HOW TO FLY", 0xFFF3C4u, 4);
    draw_text(ctx, 154, 210, "SPACE / UP / CLICK  FLAP", 0xEAF7F0u, 2);
    draw_text(ctx, 154, 250, "PASS THROUGH THE PIPE GAPS", 0xEAF7F0u, 2);
    draw_text(ctx, 154, 290, "P  PAUSE OR RESUME", 0xEAF7F0u, 2);
    draw_text(ctx, 154, 330, "R  RETRY AFTER A CRASH", 0xEAF7F0u, 2);
    draw_text(ctx, 154, 370, "ARROWS / TAB  MENU OPTIONS", 0xBDD5DCu, 2);
    draw_text(ctx, 154, 438, "F1 / ESC  BACK TO GAME", 0xFFF3C4u, 2);
  }
  if (app->mouse_visible) {
    for (int row = 0; row < 12; row++) {
      fill_rect(ctx, app->mouse_x, app->mouse_y + row, 1 + row / 2, 1, 0x203742u);
      if (row < 10) fill_rect(ctx, app->mouse_x + 1, app->mouse_y + row + 1, 1 + row / 3, 1, 0xFFFFFFu);
    }
  }
}
