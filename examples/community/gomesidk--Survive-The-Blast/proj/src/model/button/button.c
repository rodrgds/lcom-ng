#include "button.h"

struct button_imp {
    uint16_t x;        // X position of the button
    uint16_t y;        // Y position of the button
    Sprite *sprite;   // Pointer to the sprite associated with the button
};

Button *create_button(uint16_t x, uint16_t y, Sprite *sprite) {
  Button *button = (Button *) malloc(sizeof(Button));
  if (button == NULL) {
    return NULL; // Memory allocation failed
  }
  
  button->x = x;
  button->y = y;
  button->sprite = sprite;
  
  return button;
}

void destroy_button(Button *button) {
  if (button == NULL) {
    return; 
  }
  
  free(button); 
}


void draw_button(Button *button) {
  if (button == NULL || button->sprite == NULL) {
    return; 
  }
  
  draw_sprite(button->sprite, button->x, button->y);
}

bool is_button_clicked(Button *button, Cursor *cursor) {
  if (button == NULL || cursor == NULL) {
    return false; 
  }
  
  uint16_t cursor_x = get_cursor_Xpos(cursor);
  uint16_t cursor_y = get_cursor_Ypos(cursor);
  
  if (get_cursor_button_pressed(cursor, 0)) { 
    uint16_t button_x_min = button->x;
    uint16_t button_x_max = button->x + get_sprite_width(button->sprite);
    uint16_t button_y_min = button->y;
    uint16_t button_y_max = button->y + get_sprite_height(button->sprite);
    
    if (cursor_x >= button_x_min && cursor_x <= button_x_max &&
      cursor_y >= button_y_min && cursor_y <= button_y_max) {
      return true; 
    }
  }
  return false; 
}

