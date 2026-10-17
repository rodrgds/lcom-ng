#include <lcom/lcf.h>

#include "button.h"

#define BUTTON_TRANSPARENT_COLOR 0x000000

Button *construct_button(int16_t x, int16_t y, int16_t w, int16_t h, char *xpm[], char *xpm_selected[]) {
  Button *button = (Button *) malloc(sizeof(Button));

  if (!button) {
    printf("%s: malloc() error\n", __func__);
    return NULL;
  }

  button->x = x;
  button->y = y;
  button->w = w;
  button->h = h;
  button->selected = false;

  button->sprite = construct_sprite((xpm_map_t) xpm, BUTTON_TRANSPARENT_COLOR);

  if (!button->sprite) {
    printf("%s: construct_sprite(xpm, BUTTON_TRANSPARENT_COLOR: 0x%x) error\n", __func__, BUTTON_TRANSPARENT_COLOR);
    return NULL;
  }

  button->sprite_selected = construct_sprite((xpm_map_t) xpm_selected, BUTTON_TRANSPARENT_COLOR);

  if (!button->sprite_selected) {
    printf("%s: construct_sprite(xpm_selected, BUTTON_TRANSPARENT_COLOR: 0x%x) error\n", __func__, BUTTON_TRANSPARENT_COLOR);
    return NULL;
  }

  return button;
}

int check_button(Button *button, int16_t x, int16_t y) {
  if (!button) {
    printf("%s: button is NULL\n", __func__);
    return 1;
  }

  if (x >= button->x && (x <= button->x + button->w) && y >= button->y && (y <= button->y + button->h))
    button->selected = true;
  else
    button->selected = false;

  return 0;
}

void destroy_button(Button *button) {
  if (button)
    free(button);
}
