#ifndef GAMEOVERMENU_H
#define GAMEOVERMENU_H

#include "../sprite/sprite.h"
#include "menu_action_enum.h"

typedef struct {
  int selected_option;

  Sprite *gameover_sprite;
  Sprite *highscore_sprite;
  Sprite *retry_sprite;
  Sprite *retryS_sprite;
  Sprite *exittomenu_sprite;
  Sprite *exittomenuS_sprite;
  Sprite *exit_sprite;
  Sprite *exitS_sprite;
} game_over_menu_t;

game_over_menu_t *game_over_menu_init();

menu_action_t gameovermenu_process_input(game_over_menu_t *game_over_menu, uint8_t scancode);

menu_action_t gameovermenu_process_mouse_click(game_over_menu_t *game_over_menu, int mouse_x, int mouse_y);

void gameovermenu_update_hover(game_over_menu_t *game_over_menu, int mouse_x, int mouse_y);

void draw_game_over(game_over_menu_t *game_over_menu);

void destroy_game_over(game_over_menu_t *game_over_menu);

#endif
