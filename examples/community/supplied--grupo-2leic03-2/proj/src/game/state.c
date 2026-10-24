#include "state.h"
#include "game.h"
#include "background.h"

#include "../graphics/ui.h"
#include "../graphics/font.h"
#include "../graphics/sprite.h"
#include "../graphics/graphics.h"
#include <stdio.h>
#include <string.h>

#define MENU_PANEL_X 240
#define MENU_PANEL_Y 110
#define MENU_PANEL_W 544
#define MENU_PANEL_H 540

#define START_BUTTON_X 392
#define START_BUTTON_Y 320
#define START_BUTTON_W 240
#define START_BUTTON_H 70

#define EXIT_BUTTON_X 392
#define EXIT_BUTTON_Y 410
#define EXIT_BUTTON_W 240
#define EXIT_BUTTON_H 70

#define GAME_OVER_PANEL_X 240
#define GAME_OVER_PANEL_Y 120
#define GAME_OVER_PANEL_W 544
#define GAME_OVER_PANEL_H 560

#define RESTART_BUTTON_X 392
#define RESTART_BUTTON_Y 420
#define RESTART_BUTTON_W 240
#define RESTART_BUTTON_H 70

#define MENU_BUTTON_X 392
#define MENU_BUTTON_Y 510
#define MENU_BUTTON_W 240
#define MENU_BUTTON_H 70

#define PAUSE_PANEL_X 270
#define PAUSE_PANEL_Y 220
#define PAUSE_PANEL_W 484
#define PAUSE_PANEL_H 280

#define HUD_PANEL_X 8
#define HUD_PANEL_Y 42
#define HUD_PANEL_W 220
#define HUD_PANEL_H 148
#define HUD_LABEL_X 24
#define HUD_VALUE_X 112
#define HUD_ROW_1 60
#define HUD_ROW_2 90
#define HUD_ROW_3 120
#define HUD_ROW_4 150
#define HUD_LIFE_ICON_X 112
#define HUD_LIFE_ICON_Y 142
#define HUD_LIFE_ICON_SPACING 8
#define HUD_LIFE_ICON_DOWNSCALE 2

#define RTC_PANEL_W 188
#define RTC_PANEL_H 76
#define RTC_PANEL_X (SCREEN_WIDTH - RTC_PANEL_W - 12)
#define RTC_PANEL_Y 42

static Button menu_start_btn;
static Button menu_exit_btn;
static Button game_over_restart_btn;
static Button game_over_menu_btn;

void state_init(void) {
  button_init(&menu_start_btn, START_BUTTON_X, START_BUTTON_Y, START_BUTTON_W, START_BUTTON_H);
  button_init(&menu_exit_btn, EXIT_BUTTON_X, EXIT_BUTTON_Y, EXIT_BUTTON_W, EXIT_BUTTON_H);
  button_init(&game_over_restart_btn, RESTART_BUTTON_X, RESTART_BUTTON_Y, RESTART_BUTTON_W, RESTART_BUTTON_H);
  button_init(&game_over_menu_btn, MENU_BUTTON_X, MENU_BUTTON_Y, MENU_BUTTON_W, MENU_BUTTON_H);
}

void state_update(Game *game, const InputState *input) {
  if (input->escape_clicked) {
    if (game->state == STATE_MENU) {
      game->state = STATE_EXIT;
    } else {
      game_reset_play_state(game);
      game->state = STATE_MENU;
    }    
    return;
  }

  if (game->state == STATE_PLAYING && input->pause_clicked) {
    game->state = STATE_PAUSED;
    return;
  }

  if (game->state == STATE_PAUSED) {
    if (input->pause_clicked) {
      game->state = STATE_PLAYING;
    }
    return;
  }

  if (game->state == STATE_MENU) {
    button_update(&menu_start_btn, input);
    button_update(&menu_exit_btn, input);

    if (menu_start_btn.is_clicked || input->shoot_clicked) {
      game_reset_play_state(game);
      game->state = STATE_PLAYING;
    }
    else if (menu_exit_btn.is_clicked) {
      game->state = STATE_EXIT;
    }
    return;
  }

  if (game->state == STATE_GAME_OVER) {
    button_update(&game_over_restart_btn, input);
    button_update(&game_over_menu_btn, input);

    if (game_over_restart_btn.is_clicked || input->shoot_clicked) {
      game_reset_play_state(game);
      game->state = STATE_PLAYING;
    }
    else if (game_over_menu_btn.is_clicked) {
      game_reset_play_state(game);
      game->state = STATE_MENU;
    }
    return;
  }
}

static void draw_cursor(const InputState *input) {
  if (cursor_sprite.loaded) {
    sprite_draw_scaled(&cursor_sprite, input->mouse_x, input->mouse_y, 1);
  } else {
    ui_draw_cursor(input);
  }
}

static uint32_t sprite_color_at(const Sprite *sprite, uint16_t x, uint16_t y) {
  uint32_t color = 0;
  uint32_t index = (uint32_t) y * sprite->width + x;

  memcpy(&color,
         sprite->map + index * sprite->bytes_per_pixel,
         sprite->bytes_per_pixel);

  return color;
}

static void draw_small_life_icon(const Sprite *sprite, int x, int y) {
  for (uint16_t row = 0; row < sprite->height; row += HUD_LIFE_ICON_DOWNSCALE) {
    for (uint16_t col = 0; col < sprite->width; col += HUD_LIFE_ICON_DOWNSCALE) {
      uint32_t color = sprite->transparent_color;

      for (uint16_t dy = 0; dy < HUD_LIFE_ICON_DOWNSCALE && row + dy < sprite->height; dy++) {
        for (uint16_t dx = 0; dx < HUD_LIFE_ICON_DOWNSCALE && col + dx < sprite->width; dx++) {
          color = sprite_color_at(sprite, col + dx, row + dy);

          if (color != sprite->transparent_color) {
            break;
          }
        }

        if (color != sprite->transparent_color) {
          break;
        }
      }

      if (color == sprite->transparent_color) {
        continue;
      }

      graphics_draw_rectangle_clipped(x + col / HUD_LIFE_ICON_DOWNSCALE,
                                      y + row / HUD_LIFE_ICON_DOWNSCALE,
                                      1,
                                      1,
                                      color);
    }
  }
}

static void draw_life_icons(int lives) {
  if (lives <= 0) {
    font_draw_number(0, HUD_LIFE_ICON_X, HUD_ROW_4, COLOR_RED, 2);
    return;
  }

  if (!player_sprite.loaded) {
    font_draw_number(lives, HUD_LIFE_ICON_X, HUD_ROW_4, COLOR_RED, 2);
    return;
  }

  int icon_width = (int) player_sprite.width / HUD_LIFE_ICON_DOWNSCALE;
  int step = icon_width + HUD_LIFE_ICON_SPACING;

  for (int i = 0; i < lives; i++) {
    draw_small_life_icon(&player_sprite, HUD_LIFE_ICON_X + i * step, HUD_LIFE_ICON_Y);
  }
}

void state_draw_playing_hud(const Game *game) {
  ui_draw_panel(HUD_PANEL_X, HUD_PANEL_Y, HUD_PANEL_W, HUD_PANEL_H, COLOR_WHITE, COLOR_BLACK);

  font_draw_text("SCORE", HUD_LABEL_X, HUD_ROW_1, COLOR_WHITE, 2);
  font_draw_number(game->score, HUD_VALUE_X, HUD_ROW_1, COLOR_WHITE, 2);

  font_draw_text("KILLS", HUD_LABEL_X, HUD_ROW_2, COLOR_GREEN, 2);
  font_draw_number(game->asteroids_destroyed, HUD_VALUE_X, HUD_ROW_2, COLOR_GREEN, 2);

  char time_text[16];
  int time_seconds = game->frame_counter / TARGET_FPS;
  int mins = time_seconds / 60;
  int secs = time_seconds % 60;
  snprintf(time_text, sizeof(time_text), "%02d:%02d", mins, secs);

  font_draw_text("TIME", HUD_LABEL_X, HUD_ROW_3, COLOR_WHITE, 2);
  font_draw_text(time_text, HUD_VALUE_X, HUD_ROW_3, COLOR_WHITE, 2);

  font_draw_text("LIVES", HUD_LABEL_X, HUD_ROW_4, COLOR_RED, 2);
  draw_life_icons(game->ship.lives);

  ui_draw_panel(RTC_PANEL_X, RTC_PANEL_Y, RTC_PANEL_W, RTC_PANEL_H, COLOR_CYAN, COLOR_BLACK);
  font_draw_text("RTC", RTC_PANEL_X + 16, RTC_PANEL_Y + 16, COLOR_CYAN, 2);
  font_draw_text(game->rtc_time_str, RTC_PANEL_X + 68, RTC_PANEL_Y + 16, COLOR_WHITE, 2);
  font_draw_text(game->rtc_date_str, RTC_PANEL_X + 16, RTC_PANEL_Y + 48, COLOR_WHITE, 1);

  if (game->round_message_timer > 0 && game->level <= 10) {
    char round_text[32];
    snprintf(round_text, sizeof(round_text), "ROUND %d", game->level);
    ui_draw_text_centered(round_text, 0, SCREEN_HEIGHT / 2 - 30, SCREEN_WIDTH, 60, COLOR_WHITE, 4);
  }
}

void state_draw(Game *game, const InputState *input) {
  if (game->state == STATE_MENU) {
    background_draw(game);
    font_draw_text(game->rtc_date_str, 20, 20, COLOR_WHITE, 2);
    font_draw_text(game->rtc_time_str, SCREEN_WIDTH - 150, 20, COLOR_WHITE, 2);

    ui_draw_panel(MENU_PANEL_X, MENU_PANEL_Y, MENU_PANEL_W, MENU_PANEL_H, COLOR_WHITE, COLOR_BLACK);
    ui_draw_text_centered("SPACE DEFENDER", MENU_PANEL_X, 145, MENU_PANEL_W, 60, COLOR_WHITE, 4);

    ui_draw_button(&menu_start_btn, "START", COLOR_GREEN, COLOR_CYAN, COLOR_BLACK);
    ui_draw_button(&menu_exit_btn, "EXIT", COLOR_RED, COLOR_MAGENTA, COLOR_WHITE);

    ui_draw_text_centered("WASD MOVE", MENU_PANEL_X, 515, MENU_PANEL_W, 20, COLOR_WHITE, 2);
    ui_draw_text_centered("SPACE SHOOT", MENU_PANEL_X, 545, MENU_PANEL_W, 20, COLOR_WHITE, 2);
    ui_draw_text_centered("P PAUSE - ESC EXIT", MENU_PANEL_X, 575, MENU_PANEL_W, 20, COLOR_WHITE, 2);

    draw_cursor(input);
  }
  else if (game->state == STATE_PAUSED) {
    ui_draw_panel(PAUSE_PANEL_X, PAUSE_PANEL_Y, PAUSE_PANEL_W, PAUSE_PANEL_H, COLOR_WHITE, COLOR_BLACK);
    ui_draw_text_centered("PAUSED", PAUSE_PANEL_X, 265, PAUSE_PANEL_W, 60, COLOR_WHITE, 4);
    ui_draw_text_centered("PRESS P TO CONTINUE", PAUSE_PANEL_X, 350, PAUSE_PANEL_W, 20, COLOR_WHITE, 2);
    ui_draw_text_centered("WASD MOVE - SPACE SHOOT", PAUSE_PANEL_X, 385, PAUSE_PANEL_W, 20, COLOR_WHITE, 2);
    ui_draw_text_centered("ESC TO MENU", PAUSE_PANEL_X, 420, PAUSE_PANEL_W, 20, COLOR_WHITE, 2);
    
    draw_cursor(input);
  }
  else if (game->state == STATE_GAME_OVER) {
    background_draw(game);

    font_draw_text(game->rtc_date_str, 20, 20, COLOR_WHITE, 2);
    font_draw_text(game->rtc_time_str, SCREEN_WIDTH - 150, 20, COLOR_WHITE, 2);

    ui_draw_panel(GAME_OVER_PANEL_X, GAME_OVER_PANEL_Y, GAME_OVER_PANEL_W, GAME_OVER_PANEL_H, COLOR_RED, COLOR_BLACK);
    ui_draw_text_centered("GAME OVER", GAME_OVER_PANEL_X, 165, GAME_OVER_PANEL_W, 60, COLOR_RED, 4);

    char score_text[32];
    char kills_text[32];
    char time_text[32];

    snprintf(score_text, sizeof(score_text), "FINAL SCORE: %d", game->score);
    snprintf(kills_text, sizeof(kills_text), "KILLS: %d", game->asteroids_destroyed);
    int time_seconds = game->frame_counter / TARGET_FPS;
    int mins = time_seconds / 60;
    int secs = time_seconds % 60;
    snprintf(time_text, sizeof(time_text), "TIME: %02d:%02d", mins, secs);

    ui_draw_text_centered(score_text, GAME_OVER_PANEL_X, 280, GAME_OVER_PANEL_W, 20, COLOR_WHITE, 2);
    ui_draw_text_centered(kills_text, GAME_OVER_PANEL_X, 320, GAME_OVER_PANEL_W, 20, COLOR_GREEN, 2);
    ui_draw_text_centered(time_text, GAME_OVER_PANEL_X, 360, GAME_OVER_PANEL_W, 20, COLOR_WHITE, 2);

    ui_draw_button(&game_over_restart_btn, "RESTART", COLOR_GREEN, COLOR_CYAN, COLOR_BLACK);
    ui_draw_button(&game_over_menu_btn, "MENU", COLOR_BLUE, COLOR_CYAN, COLOR_WHITE);

    ui_draw_text_centered("CLICK OR SPACE", GAME_OVER_PANEL_X, 650, GAME_OVER_PANEL_W, 20, COLOR_WHITE, 2);

    draw_cursor(input);
  }
}
