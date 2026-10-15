#ifndef MAINMENU_H
#define MAINMENU_H

#include "../sprite/sprite.h"
#include "menu_action_enum.h"

typedef struct {
  int selected_option;

  Sprite *welcome_sprite;
  Sprite *to_sprite;
  Sprite *logo_sprite;
  Sprite *startS_sprite;
  Sprite *start_sprite;
  Sprite *howtoplayS_sprite;
  Sprite *howtoplay_sprite;
  Sprite *exitS_sprite;
  Sprite *exit_sprite;
} main_menu_t;

main_menu_t* main_menu_init();

menu_action_t mainmenu_process_input(main_menu_t *main_menu, uint8_t scancode);

menu_action_t mainmenu_process_mouse_click(main_menu_t *main_menu, int mouse_x, int mouse_y);

void mainmenu_update_hover(main_menu_t *main_menu, int mouse_x, int mouse_y);

void draw_main_menu(main_menu_t *main_menu);

void destroy_main_menu(main_menu_t *main_menu);

#endif
