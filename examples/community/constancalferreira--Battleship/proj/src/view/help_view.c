#include "help_view.h"

extern Sprite* help;  
extern Sprite* help_options;
extern Sprite* go_back;

void draw_help(uint8_t *buffer) {
  fill_screen(DARK_ORANGE, buffer);
  draw_sprite(help, 100, 0, buffer);
  draw_sprite(help_options, 100, 200, buffer);
  draw_sprite(go_back, 600, 540, buffer);
}
