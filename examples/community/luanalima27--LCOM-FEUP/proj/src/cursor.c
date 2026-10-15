// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include "video.h"
#include "player.h"
#include "sprites.h"
#include "i8042.h"

extern uint8_t* cursor_sprite;
extern uint8_t* cursor_black_sprite;
extern xpm_image_t img_cursor_sprite;
extern xpm_image_t img_cursor_black_sprite;
extern uint8_t* target_sprite;
extern uint8_t* target_black_sprite;
extern xpm_image_t img_target_sprite;
extern xpm_image_t img_target_black_sprite;

int draw_cursor(uint16_t x, uint16_t y) {
  if(draw_xpm(cursor_sprite, img_cursor_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}

int delete_cursor(uint16_t x, uint16_t y) {
  if(draw_xpm(cursor_black_sprite, img_cursor_black_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}

int draw_target(uint16_t x, uint16_t y) {
  if(draw_xpm(target_sprite, img_target_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}

int delete_target(uint16_t x, uint16_t y) {
  if(draw_xpm(target_black_sprite, img_target_black_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}

