#include "../headers/screen.h"

Screen* createScreen(void (*menu_handler)(int n), xpm_image_t background, FruitBasket fruit_basket) {
  Screen* new_screen = (Screen*) malloc(sizeof(Screen));
  
  new_screen->menu_handler = menu_handler;

  new_screen->background = background;

  new_screen->fruit_basket = fruit_basket;

  return new_screen;
}

void destroyScreen(Screen* screen) {
  destroyFruitBasket(&screen->fruit_basket);
  free(screen);
}
