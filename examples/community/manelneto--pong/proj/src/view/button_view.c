#include <lcom/lcf.h>

#include "button_view.h"

#include "sprite_view.h"

int draw_button(Button *button) {
  if (!button) {
    printf("%s: button is NULL\n", __func__);
    return 1;
  }

  if (!button->selected && draw_sprite(button->sprite, button->x, button->y)) {
    printf("%s: draw_sprite(button->sprite, button->x: %d, button->y: %d) error\n", __func__, button->x, button->y);
    return 1;
  }
  else if (button->selected && draw_sprite(button->sprite_selected, button->x, button->y)) {
    printf("%s: draw_sprite(button->sprite_selected, button->x: %d, button->y: %d) error\n", __func__, button->x, button->y);
    return 1;
  }

  return 0;
}
