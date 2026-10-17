#include "menu.h"

static char* title = "lc typer";
static Button start_button;
static Button score_button;
static Button exit_button;

void menu_init() {
  start_button = button_create(video_get_h_res() / 2, BUTTON_POS_Y, "start");
  score_button = button_create(video_get_h_res() / 2, BUTTON_POS_Y + LINE_HEIGHT, "leaderboard");
  exit_button = button_create(video_get_h_res() / 2, BUTTON_POS_Y + 2 * LINE_HEIGHT, "exit");
}

FILE *file;

void menu_draw() {
  int middle = video_get_h_res() / 2;
  draw_sentence(title, middle - get_sentence_width(title) / 2, TITLE_POS_Y, PRIMARY_COLOR);
}

void menu_handle_event(Event event) {
  if (start_button.clicked) {
    start_button.clicked = 0;
    proj_set_state(GAME);
  }
  if (score_button.clicked) {
    score_button.clicked = 0;
    proj_set_state(SCORE);
  }
  if (exit_button.clicked) {
    exit_button.clicked = 0;
    proj_set_state(OVER);
  }
}

void menu_step(Event event) {
  button_step(&start_button, &event);
  button_step(&score_button, &event);
  button_step(&exit_button, &event);
  menu_handle_event(event);
  menu_draw();
}
