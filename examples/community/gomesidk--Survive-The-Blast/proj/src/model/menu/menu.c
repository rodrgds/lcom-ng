#include "menu.h"

struct menu_imp {
  Sprite *background;    
  Sprite *menu_title; 
  uint16_t title_xpos; 
  Button *single_mode;  
  Button *multi_mode;   
  Button *exit;    
};


Menu* create_menu(SpriteLoader *loader) {
  Menu *m = (Menu *) malloc(sizeof(Menu));
  if (m == NULL) {
    return NULL;
  }

  m->background = get_menu_background(loader);
  m->menu_title = get_menu_title(loader);
  m->title_xpos = get_sprite_width(m->background) / 2 - get_sprite_width(m->menu_title) / 2;

  uint16_t x_pos = get_sprite_width(m->background) / 2 - get_sprite_width(get_exit_button(loader)) / 2;
  uint16_t y_pos = get_sprite_height(m->background) - get_sprite_height(get_exit_button(loader));

  m->single_mode = create_button( x_pos , y_pos - 400 , get_single_mode_button(loader));
  m->multi_mode = create_button( x_pos , y_pos - 250 , get_multi_mode_button(loader));
  m->exit = create_button( x_pos , y_pos - 100 , get_exit_button(loader));

  return m;
}

void destroy_menu(Menu *m) {
  if (m == NULL) {
    return;
  }

  free(m);
}

void draw_menu(Menu *m) {
  if (m == NULL) {
    return;
  }

  draw_sprite(m->background, 0, 0);

  draw_sprite(m->menu_title, m->title_xpos, 100);

  draw_button(m->single_mode);
  draw_button(m->multi_mode);
  draw_button(m->exit); 
}


// 0 NO ACTION
// 1 Leave game  
// 2 Single mode game
// 3 Multiplayer mode game
int process_menu_mouse(Menu *m, Cursor *cursor) {
  if (m == NULL || cursor == NULL) {
    return 1; // Exit the game
  }

  if ( is_button_clicked(m->exit, cursor) ) {
    return 1; // Exit the game
  }else if ( is_button_clicked(m->single_mode, cursor) ) {
    return 2; // Go to single mode
  }else if ( is_button_clicked(m->multi_mode, cursor) ) {
    return 3; // Go to multiplayer mode
  }

  return 0; // Continue the game
}
