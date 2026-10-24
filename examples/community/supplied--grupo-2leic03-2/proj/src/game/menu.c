#include "menu.h"

static Button start_button;
static Button exit_button;

void menu_init(void) {
  button_init(&start_button, 300, 200, 200, 50); 
  button_init(&exit_button, 300, 300, 200, 50);
}

void menu_update(Game *game, const InputState *input) {
  button_update(&start_button, input);
  button_update(&exit_button, input);

  if (start_button.is_clicked) game->state = STATE_PLAYING;

  if (exit_button.is_clicked) game->state = STATE_EXIT;
}

void menu_draw(Game *game) {
  (void) game;
}
