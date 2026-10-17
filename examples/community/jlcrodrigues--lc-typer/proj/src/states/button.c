#include "button.h"

Button button_create(int pos_x, int pos_y, char* text) {
  Button b;
  b.width = get_sentence_width(text);
  b.pos_x = pos_x - (b.width >> 1);
  b.pos_y = pos_y;
  b.text = text;
  b.hover = 0;
  b.clicked = 0;
  return b;
}

void button_draw(Button* button) {
  uint32_t color = (button->hover) ? PRIMARY_COLOR : SECONDARY_COLOR;
  draw_sentence(button->text, button->pos_x, button->pos_y, color);
}

void button_handle_event(Button* button) {
  mouse_sprite* mouse = proj_get_mouse();
  if (mouse->x_pos >= button->pos_x && mouse->x_pos <= button->pos_x + button->width) {
    if (mouse->y_pos >= button->pos_y && mouse->y_pos <= button->pos_y + FONT_HEIGHT) {
      button->hover = 1;
      button->clicked = mouse->clicked;
      return;
    }
  }
  button->hover = 0;
  button->clicked = 0;
}

void button_step(Button* button, Event* event) {
  if (event->type == MOUSE)
    button_handle_event(button);
  button_draw(button);
}
