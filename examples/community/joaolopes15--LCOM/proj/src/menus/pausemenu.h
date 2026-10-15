#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "../sprite/sprite.h"
#include "menu_action_enum.h"

typedef struct {
  int selected_option;
  
  Sprite *paused_sprite;
  Sprite *continueS_sprite;
  Sprite *continue_sprite;
  Sprite *retryS_sprite;
  Sprite *retry_sprite;
  Sprite *exittomenuS_sprite;
  Sprite *exittomenu_sprite;
} pause_menu_t;

pause_menu_t* pause_menu_init();

menu_action_t pausemenu_process_input(pause_menu_t *pause_menu, uint8_t scancode);

menu_action_t pausemenu_process_mouse_click(pause_menu_t *pause_menu, int mouse_x, int mouse_y);

void pausemenu_update_hover(pause_menu_t *pause_menu, int mouse_x, int mouse_y);

void draw_pause_menu(pause_menu_t *pause_menu);

void destroy_pause_menu(pause_menu_t *pause_menu);

#endif
