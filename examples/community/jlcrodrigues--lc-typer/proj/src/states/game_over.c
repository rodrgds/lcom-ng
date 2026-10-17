#include "game_over.h"

static int init = 0;
static Button exit_button;
static Button next_button;

void game_over_init(void) {
  exit_button = button_create(video_get_h_res() - FONT_WIDTH,
   PADDING, "x");
  next_button = button_create(video_get_h_res() >> 1,
    TEXT_Y_POS + (LINE_HEIGHT * 3), "next");
  init = 1;
}

void game_over_draw(Game* game) {
  int16_t h_res = video_get_h_res();
  int width;
  int wpm = (int)(
    ((double)(game->player_position) / 5)
     / ((double)(game->time_elapsed) / 60)
    );
  game->wpm = wpm;
  int acc = (int)(
    ((double)(game->text_size) * 100)
     / ((double)(game->text_size + game->typo_count))
    );
  width = get_sentence_width("wpm");
  draw_sentence("wpm", h_res / 4 - width / 2, TEXT_Y_POS, PRIMARY_COLOR);
  char wpm_str[16];
  snprintf(wpm_str, sizeof(wpm_str), "%d", wpm);
  width = get_sentence_width(wpm_str);
  draw_sentence(wpm_str, h_res / 4 - width / 2, TEXT_Y_POS + LINE_HEIGHT, PRIMARY_COLOR);

  width = get_sentence_width("acc");
  draw_sentence("acc", 3 * h_res / 4 - width / 2, TEXT_Y_POS, PRIMARY_COLOR);
  char acc_str[8];
  snprintf(acc_str, sizeof(acc_str), "%d%%", acc);
  width = get_sentence_width(acc_str);
  draw_sentence(acc_str, 3 * h_res / 4 - width / 2, TEXT_Y_POS + LINE_HEIGHT, PRIMARY_COLOR);
}

void game_over_handle_event(Game* game, Event event) {
  if (event.type == KEYBOARD) {
    if (event.info.keyboard.buff == BREAKCODE_ENTER) {
      score_board_update(game);
      proj_set_state(GAME);
    }
    else if (event.info.keyboard.buff == BREAKCODE_ESC) {
      score_board_update(game);
      proj_set_state(MENU);
    }
  }
  if (next_button.clicked) {
    next_button.clicked = 0;
    score_board_update(game);
    proj_set_state(GAME);
  }
  if (exit_button.clicked) {
    next_button.clicked = 0;
    score_board_update(game);
    proj_set_state(MENU);
  }
}

void game_over_step(Game* game, Event event) {
  if (!init) game_over_init();
  button_step(&exit_button, &event);
  button_step(&next_button, &event);
  game_over_handle_event(game, event);
  game_over_draw(game);
}
