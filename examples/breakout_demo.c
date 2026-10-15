#include "example_ui.h"

#include <lcom/i8042.h>
#include <lcom/i8254.h>
#include <lcom/lcom.h>
#include <lcom/vbe.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define BRICK_ROWS 4
#define BRICK_COLS 9

typedef enum { READY, PLAYING, PAUSED, GAME_OVER, CLEARED } round_state_t;
typedef struct {
  int paddle_x, ball_x, ball_y, ball_vx, ball_vy;
  uint8_t bricks[BRICK_ROWS][BRICK_COLS];
  uint32_t score;
  int lives;
  round_state_t round;
  bool help, left_down, right_down;
  example_modifiers_t modifiers;
} breakout_state_t;

static void reset_ball(breakout_state_t *state) {
  state->ball_x = state->paddle_x + 48;
  state->ball_y = 430;
  state->ball_vx = 4;
  state->ball_vy = -4;
  state->round = READY;
}

static void init_breakout(breakout_state_t *state) {
  *state = (breakout_state_t){.paddle_x = 340, .lives = 3};
  reset_ball(state);
  for (int row = 0; row < BRICK_ROWS; row++)
    for (int col = 0; col < BRICK_COLS; col++) state->bricks[row][col] = 1;
}

static void update_breakout(breakout_state_t *state) {
  if (state->help || state->round == PAUSED || state->round == GAME_OVER || state->round == CLEARED) return;
  if (state->left_down) state->paddle_x -= 7;
  if (state->right_down) state->paddle_x += 7;
  if (state->paddle_x < 54) state->paddle_x = 54;
  if (state->paddle_x > 650) state->paddle_x = 650;
  if (state->round == READY) {
    state->ball_x = state->paddle_x + 48;
    return;
  }

  state->ball_x += state->ball_vx;
  state->ball_y += state->ball_vy;
  if (state->ball_x < 55) { state->ball_x = 55; state->ball_vx = -state->ball_vx; }
  if (state->ball_x > 745) { state->ball_x = 745; state->ball_vx = -state->ball_vx; }
  if (state->ball_y < 103) { state->ball_y = 103; state->ball_vy = -state->ball_vy; }

  if (state->ball_y + 7 >= 448 && state->ball_y <= 466 && state->ball_vy > 0 &&
      state->ball_x + 7 >= state->paddle_x && state->ball_x - 7 <= state->paddle_x + 96) {
    state->ball_y = 441;
    state->ball_vy = -state->ball_vy;
    int hit = state->ball_x - state->paddle_x;
    state->ball_vx = hit < 32 ? -5 : (hit > 64 ? 5 : state->ball_vx);
  }

  for (int row = 0; row < BRICK_ROWS; row++) for (int col = 0; col < BRICK_COLS; col++) {
    if (!state->bricks[row][col]) continue;
    int x = 68 + col * 72, y = 124 + row * 28;
    if (state->ball_x + 7 >= x && state->ball_x - 7 <= x + 64 &&
        state->ball_y + 7 >= y && state->ball_y - 7 <= y + 22) {
      state->bricks[row][col] = 0;
      state->ball_vy = -state->ball_vy;
      state->score += 10;
      if (state->score == BRICK_ROWS * BRICK_COLS * 10) state->round = CLEARED;
      return;
    }
  }
  if (state->ball_y > 536) {
    if (--state->lives == 0) state->round = GAME_OVER;
    else reset_ball(state);
  }
}

static void centered(example_canvas_t *ctx, int y, const char *text, uint32_t color, int scale) {
  example_label(ctx, (800 - example_label_width(text, scale)) / 2, y, text, color, scale);
}

static void draw_breakout(example_canvas_t *ctx, const breakout_state_t *state, uint32_t frame) {
  (void)frame;
  example_fill_rect(ctx, 0, 0, 800, 600, UI_DARK);
  example_label(ctx, 44, 20, "Breakout", UI_LIGHT, 2);
  example_label(ctx, 44, 58, "Clear the wall. Keep the ball in play.", UI_DIM, 1);
  char score[40]; snprintf(score, sizeof(score), "%03u", state->score);
  example_label(ctx, 574, 18, "SCORE", UI_DIM, 1);
  example_label(ctx, 574, 40, score, UI_LIGHT, 2);
  example_label(ctx, 684, 18, "LIVES", UI_DIM, 1);
  for (int i = 0; i < 3; i++) example_round_rect(ctx, 686 + i * 24, 47, 14, 14, 7, i < state->lives ? 0x77D3C4 : 0x354654);
  example_round_rect(ctx, 44, 92, 712, 444, 8, 0x1B2937);
  example_fill_rect(ctx, 52, 94, 696, 2, 0x354654);
  const uint32_t colors[] = {0xEE775B, 0xEEAE55, 0x59AB88, 0x5B8FD2};
  for (int row = 0; row < BRICK_ROWS; row++) for (int col = 0; col < BRICK_COLS; col++) {
    if (!state->bricks[row][col]) continue;
    example_round_rect(ctx, 68 + col * 72, 124 + row * 28, 64, 22, 3, colors[row]);
    example_fill_rect(ctx, 74 + col * 72, 128 + row * 28, 52, 1, 0xF9E7D2);
  }
  example_round_rect(ctx, state->paddle_x, 448, 96, 18, 5, 0x77D3C4);
  example_fill_rect(ctx, state->paddle_x + 8, 451, 80, 2, 0xDFF7EE);
  if (state->round != GAME_OVER && state->round != CLEARED)
    example_round_rect(ctx, state->ball_x - 7, state->ball_y - 7, 14, 14, 7, 0xFFF2B3);
  example_label(ctx, 44, 558, "Arrows / A D  Move      Space  Play / pause      R  Restart", UI_DIM, 1);
  example_label(ctx, 44, 580, "F1  How to play      Esc  Close", UI_DIM, 1);

  if (state->help) {
    example_round_rect(ctx, 166, 244, 468, 198, 8, 0x101C28);
    example_label(ctx, 194, 264, "How to play", UI_LIGHT, 2);
    example_label(ctx, 194, 312, "Move with Left / Right or A / D.", UI_LIGHT, 1);
    example_label(ctx, 194, 338, "Space serves, pauses and resumes the ball.", UI_LIGHT, 1);
    example_label(ctx, 194, 364, "Clear all 36 bricks. You have three lives.", UI_LIGHT, 1);
    example_label(ctx, 194, 402, "F1 or Esc  Back to game", 0x77D3C4, 1);
  } else if (state->round != PLAYING) {
    const char *title = state->round == PAUSED ? "Paused" : state->round == GAME_OVER ? "Out of lives" : state->round == CLEARED ? "Wall cleared!" : state->lives == 3 ? "Ready to play?" : "Another chance";
    const char *hint = state->round == GAME_OVER || state->round == CLEARED ? "R or Enter to play again" : state->round == PAUSED ? "Space or P to resume" : "Space or Enter to serve";
    example_round_rect(ctx, 224, 274, 352, 118, 8, 0x101C28);
    centered(ctx, 292, title, UI_LIGHT, 2);
    centered(ctx, 346, hint, 0x77D3C4, 1);
  }
}

static void handle_key(uint8_t data, bool *extended, breakout_state_t *state, bool *running) {
  bool was_extended = *extended;
  if (example_modifier_byte(&state->modifiers, data)) {
    if (data != KBD_TWO_BYTE_PFX) *extended = false;
    else *extended = true;
    return;
  }
  if (data == KBD_TWO_BYTE_PFX) { *extended = true; return; }
  *extended = false;
  bool released = (data & KBD_BREAK_BIT) != 0;
  uint8_t code = data & 0x7F;
  if (was_extended) {
    if (code == 0x4B) state->left_down = !released && !state->help;
    if (code == 0x4D) state->right_down = !released && !state->help;
    return;
  }
  if (code == 0x1E) state->left_down = !released && !state->help;
  if (code == 0x20) state->right_down = !released && !state->help;
  if (released) return;
  if (code == 0x3B || code == 0x23) {
    state->help = !state->help;
    state->left_down = state->right_down = false;
    return;
  }
  if (state->help) { if (code == 0x01) state->help = false; return; }
  if (example_primary(&state->modifiers)) {
    if (code == 0x13) init_breakout(state);
    return;
  }
  if (code == 0x13) init_breakout(state);
  if (code == 0x01) *running = false;
  if (code == 0x39 || code == 0x19 || code == 0x1C) {
    if (state->round == READY) state->round = PLAYING;
    else if (state->round == PLAYING) state->round = PAUSED;
    else if (state->round == PAUSED) state->round = PLAYING;
    else if (code == 0x1C) init_breakout(state);
  }
}

int main(void) {
  if (lcom_init() != LCOM_OK) return 1;
  lcom_vbe_mode_info_t info;
  if (lcom_vbe_get_mode_info(LCOM_VBE_MODE_800_600_24, &info) != LCOM_OK) return 1;
  if (lcom_vbe_set_mode(LCOM_VBE_MODE_800_600_24) != LCOM_OK) return 1;
  uint8_t *fb = 0;
  if (lcom_phys_map(info.framebuffer_phys, info.framebuffer_size, (void **)&fb) != LCOM_OK) return 1;
  lcom_irq_t timer_irq, kbd_irq;
  if (lcom_irq_subscribe(TIMER0_IRQ, 0, &timer_irq) != LCOM_OK) return 1;
  if (lcom_irq_subscribe(KBC_IRQ, 0, &kbd_irq) != LCOM_OK) return 1;
  breakout_state_t state;
  init_breakout(&state);
  example_canvas_t canvas = {fb, info};
  bool extended = false, running = true;
  uint32_t frame = 0;
  draw_breakout(&canvas, &state, frame);
  lcom_vbe_present();
  while (running) {
    lcom_event_t ev;
    if (lcom_event_wait(&ev) != LCOM_OK) break;
    if ((ev.irq_mask & kbd_irq.mask) != 0) {
      /* Drain whole chords, including extended make/break sequences. */
      for (int i = 0; i < 24 && running; i++) {
        uint8_t status = 0, data = 0;
        if (lcom_port_read8(KBC_ST_REG, &status) != LCOM_OK || !(status & KBC_ST_OBF) || (status & KBC_ST_AUX)) break;
        if (lcom_port_read8(KBC_OUT_BUF, &data) != LCOM_OK) break;
        handle_key(data, &extended, &state, &running);
      }
    }
    if (ev.irq_mask & timer_irq.mask) {
      frame++;
      update_breakout(&state);
      draw_breakout(&canvas, &state, frame);
      lcom_vbe_present();
    }
  }
  draw_breakout(&canvas, &state, frame);
  lcom_vbe_present();
  lcom_printf("breakout demo score=%u lives=%d frames=%u\n", state.score, state.lives, frame);
  lcom_irq_unsubscribe(&kbd_irq);
  lcom_irq_unsubscribe(&timer_irq);
  lcom_phys_unmap(fb, info.framebuffer_size);
  lcom_exit();
  return 0;
}
