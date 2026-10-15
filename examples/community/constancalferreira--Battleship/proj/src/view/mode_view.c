#include "mode_view.h"

extern Sprite* choose_mode;
extern Sprite* single_player;
extern Sprite* multi_player;

void draw_mode(uint8_t *buffer) {
  fill_screen(CREAM, buffer);
  draw_sprite(choose_mode, 60, 80, buffer); 
  draw_sprite(single_player, 85, 350, buffer);
  draw_sprite(multi_player, 80, 430, buffer);
}

