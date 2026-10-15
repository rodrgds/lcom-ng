// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include "video.h"
#include "player.h"
#include "sprites.h"
#include "i8042.h"

extern uint8_t* spaceship_sprite;
extern uint8_t* spaceship_black_sprite;
extern xpm_image_t img_spaceship_sprite;
extern xpm_image_t img_spaceship_black_sprite;

int player_x = 325;
int player_y = 460;

int draw_player(uint16_t x, uint16_t y) {
  if(draw_xpm(spaceship_sprite, img_spaceship_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}

int delete_player(uint16_t x, uint16_t y) {
  if(draw_xpm(spaceship_black_sprite, img_spaceship_black_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}
