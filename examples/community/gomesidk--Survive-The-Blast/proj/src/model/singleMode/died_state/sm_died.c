#include "sm_died.h"

struct SmDied_imp {
  uint16_t xPos_title; // X position for the title
  uint16_t xPos_your_score; // X position for the your score text
  Sprite *died_title;
  Sprite *died_background;
  Sprite *your_score;
  Button *back_to_menu;
  Button *play_again;
  Score *score; 
};

SmDied* create_sm_died(SpriteLoader *loader, Score *score) {
  SmDied *d = (SmDied *) malloc(sizeof(SmDied));
  if (d == NULL) {
    return NULL;
  }

  d->died_title = get_died_title(loader);
  d->your_score = get_your_score_text(loader);
  d->died_background = get_died_background(loader);

  int back_x = get_sprite_width(d->died_background) / 2 - get_sprite_width(get_back_to_menu(loader)) / 2;
  int back_y = get_sprite_height(d->died_background) - get_sprite_height(get_back_to_menu(loader)) - 80; // 50 pixels from the bottom
  int again_x = get_sprite_width(d->died_background) / 2 - get_sprite_width(get_play_again(loader)) / 2;
  int again_y = back_y - get_sprite_height(get_play_again(loader)) - 70; // 20 pixels above the back button

  d->back_to_menu = create_button(back_x, back_y, get_back_to_menu(loader));
  d->play_again = create_button(again_x, again_y, get_play_again(loader));

  d->xPos_title = (get_mode_info().XResolution - get_sprite_width(d->died_title)) / 2;
  d->xPos_your_score = (get_mode_info().XResolution - get_sprite_width(d->your_score)) / 2;
  d->score = score;

  return d;
}

void destroy_sm_died(SmDied *d) {
  if (d == NULL) {
    return;
  }
  free(d);
}

void draw_sm_died(SmDied *d) {
  if (d == NULL) {
    return;
  }

  draw_sprite(d->died_background, 0, 0);
  draw_sprite(d->died_title, d->xPos_title, 150);
  draw_sprite(d->your_score, d->xPos_your_score, 270);
  draw_score(d->score, d->xPos_your_score + 150, 370);
  draw_button(d->play_again);
  draw_button(d->back_to_menu);
}

// 0 Continue game
// 1 Go back to menu
int process_sm_died_mouse(SmDied *d, Cursor *cursor) {
  if (d == NULL || cursor == NULL) {
    return 1; 
  }

  if (is_button_clicked(d->back_to_menu, cursor)) {
    return 1; // Go back to menu  
  }

  if (is_button_clicked(d->play_again, cursor)) {
    return 2; // Play again
  }

  return 0; 
}

// 0 Continue in the died state
// 1 Exit to menu
int process_sm_died_kbd(SmDied *d, KeyPressed *key) {
  if (d == NULL || key == NULL) {
    return 1;
  }

  if (is_esc_pressed(key)) {
    return 1; 
  }

  return 0;
}
