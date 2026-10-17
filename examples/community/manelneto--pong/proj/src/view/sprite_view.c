#include <lcom/lcf.h>

#include "sprite_view.h"

#include "../controller/video.h"

#define NUMBER_WIDTH 20

extern Sprite *times;
extern Sprite *zero;
extern Sprite *one;
extern Sprite *two;
extern Sprite *three;
extern Sprite *four;
extern Sprite *five;
extern Sprite *six;
extern Sprite *seven;
extern Sprite *eight;
extern Sprite *nine;

int draw_sprite(Sprite *sprite, uint16_t x, uint16_t y) {
  uint16_t height = sprite->height;
  uint16_t width = sprite->width;
  uint32_t background = sprite->background;
  uint32_t color;
  for (uint16_t h = 0; h < height; h++)
    for (uint16_t w = 0; w < width; w++) {
      color = sprite->colors[w + h * width];
      if (color == background)
        continue;
      if (video_draw_pixel(x + w, y + h, color)) {
        printf("%s: video_draw_pixel(x + w: %d, y + h: %d, color: 0x%x) error\n", __func__, x + w, y + h, color);
        return 1;
      }
    }
  return 0;
}

int draw_times(uint16_t x, uint16_t y) {
  if (draw_sprite(times, x, y)) {
    printf("%s: draw_sprite(times, x: %d, y: %d) error\n", __func__, x, y);
    return 1;
  }
  return 0;
}

int draw_digit(uint8_t digit, uint16_t x, uint16_t y) {
  switch (digit) {
    case 0:
      if (draw_sprite(zero, x, y)) {
        printf("%s: draw_sprite(zero, x: %d, y: %d) error\n", __func__, x, y);
        return 1;
      }
      break;
    case 1:
      if (draw_sprite(one, x, y)) {
        printf("%s: draw_sprite(one, x: %d, y: %d) error\n", __func__, x, y);
        return 1;
      }
      break;
    case 2:
      if (draw_sprite(two, x, y)) {
        printf("%s: draw_sprite(two, x: %d, y: %d) error\n", __func__, x, y);
        return 1;
      }
      break;
    case 3:
      if (draw_sprite(three, x, y)) {
        printf("%s: draw_sprite(three, x: %d, y: %d) error\n", __func__, x, y);
        return 1;
      }
      break;
    case 4:
      if (draw_sprite(four, x, y)) {
        printf("%s: draw_sprite(four, x: %d, y: %d) error\n", __func__, x, y);
        return 1;
      }
      break;
    case 5:
      if (draw_sprite(five, x, y)) {
        printf("%s: draw_sprite(five, x: %d, y: %d) error\n", __func__, x, y);
        return 1;
      }
      break;
    case 6:
      if (draw_sprite(six, x, y)) {
        printf("%s: draw_sprite(six, x: %d, y: %d) error\n", __func__, x, y);
        return 1;
      }
      break;
    case 7:
      if (draw_sprite(seven, x, y)) {
        printf("%s: draw_sprite(seven, x: %d, y: %d) error\n", __func__, x, y);
        return 1;
      }
      break;
    case 8:
      if (draw_sprite(eight, x, y)) {
        printf("%s: draw_sprite(eight, x: %d, y: %d) error\n", __func__, x, y);
        return 1;
      }
      break;
    case 9:
      if (draw_sprite(nine, x, y)) {
        printf("%s: draw_sprite(nine, x: %d, y: %d) error\n", __func__, x, y);
        return 1;
      }
      break;
  }
  return 0;
}

int draw_number(uint32_t number, uint16_t x, uint16_t y) {
  uint8_t digit;
  while (number > 0) {
    digit = number % 10;
    if (draw_digit(digit, x, y)) {
      printf("%s: draw_digit(digit: %d, x: %d, y: %d)\n", __func__, digit, x, y);
      return 1;
    }
    number /= 10;
    x -= NUMBER_WIDTH;
  }
  return 0;
}
