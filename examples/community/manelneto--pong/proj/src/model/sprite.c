#include <lcom/lcf.h>

#include "sprite.h"

#include "../xpm/0.xpm"
#include "../xpm/1.xpm"
#include "../xpm/2.xpm"
#include "../xpm/3.xpm"
#include "../xpm/4.xpm"
#include "../xpm/5.xpm"
#include "../xpm/6.xpm"
#include "../xpm/7.xpm"
#include "../xpm/8.xpm"
#include "../xpm/9.xpm"
#include "../xpm/x.xpm"

Sprite *zero;
Sprite *one;
Sprite *two;
Sprite *three;
Sprite *four;
Sprite *five;
Sprite *six;
Sprite *seven;
Sprite *eight;
Sprite *nine;
Sprite *times;

#define NUMBER_BACKGROUND 0xFFFFFF

Sprite *construct_sprite(xpm_map_t xpm, uint32_t background) {
  Sprite *sprite = (Sprite *) malloc(sizeof(Sprite));

  if (!sprite) {
    printf("%s: malloc() error\n", __func__);
    return NULL;
  }

  xpm_image_t image;
  sprite->colors = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &image);

  if (!sprite->colors) {
    free(sprite);
    printf("%s: xpm_load(xpm, XPM_8_8_8, &image) error\n", __func__);
    return NULL;
  }

  sprite->height = image.height;
  sprite->width = image.width;
  sprite->background = background;

  return sprite;
}

void destroy_sprite(Sprite *sprite) {
  if (sprite) {
    if (sprite->colors)
      free(sprite->colors);
    free(sprite);
  }
}

int construct_numbers() {
  zero = construct_sprite((xpm_map_t) zero_xpm, NUMBER_BACKGROUND);
  if (!zero) {
    printf("%s: construct_sprite(zero_xpm, NUMBER_BACKGROUND: 0x%x) error\n", __func__, NUMBER_BACKGROUND);
    return 1;
  }

  one = construct_sprite((xpm_map_t) one_xpm, NUMBER_BACKGROUND);
  if (!one) {
    printf("%s: construct_sprite(one_xpm, NUMBER_BACKGROUND: 0x%x) error\n", __func__, NUMBER_BACKGROUND);
    return 1;
  }

  two = construct_sprite((xpm_map_t) two_xpm, NUMBER_BACKGROUND);
  if (!two) {
    printf("%s: construct_sprite(two_xpm, NUMBER_BACKGROUND: 0x%x) error\n", __func__, NUMBER_BACKGROUND);
    return 1;
  }

  three = construct_sprite((xpm_map_t) three_xpm, NUMBER_BACKGROUND);
  if (!three) {
    printf("%s: construct_sprite(three_xpm, NUMBER_BACKGROUND: 0x%x) error\n", __func__, NUMBER_BACKGROUND);
    return 1;
  }

  four = construct_sprite((xpm_map_t) four_xpm, NUMBER_BACKGROUND);
  if (!four) {
    printf("%s: construct_sprite(four_xpm, NUMBER_BACKGROUND: 0x%x) error\n", __func__, NUMBER_BACKGROUND);
    return 1;
  }

  five = construct_sprite((xpm_map_t) five_xpm, NUMBER_BACKGROUND);
  if (!five) {
    printf("%s: construct_sprite(five_xpm, NUMBER_BACKGROUND: 0x%x) error\n", __func__, NUMBER_BACKGROUND);
    return 1;
  }

  six = construct_sprite((xpm_map_t) six_xpm, NUMBER_BACKGROUND);
  if (!six) {
    printf("%s: construct_sprite(six_xpm, NUMBER_BACKGROUND: 0x%x) error\n", __func__, NUMBER_BACKGROUND);
    return 1;
  }

  seven = construct_sprite((xpm_map_t) seven_xpm, NUMBER_BACKGROUND);
  if (!seven) {
    printf("%s: construct_sprite(seven_xpm, NUMBER_BACKGROUND: 0x%x) error\n", __func__, NUMBER_BACKGROUND);
    return 1;
  }

  eight = construct_sprite((xpm_map_t) eight_xpm, NUMBER_BACKGROUND);
  if (!eight) {
    printf("%s: construct_sprite(eight_xpm, NUMBER_BACKGROUND: 0x%x) error\n", __func__, NUMBER_BACKGROUND);
    return 1;
  }

  nine = construct_sprite((xpm_map_t) nine_xpm, NUMBER_BACKGROUND);
  if (!nine) {
    printf("%s: construct_sprite(nine_xpm, NUMBER_BACKGROUND: 0x%x) error\n", __func__, NUMBER_BACKGROUND);
    return 1;
  }

  times = construct_sprite((xpm_map_t) x_xpm, NUMBER_BACKGROUND);
  if (!times) {
    printf("%s: construct_sprite(x_xpm, NUMBER_BACKGROUND: 0x%x) error\n", __func__, NUMBER_BACKGROUND);
    return 1;
  }

  return 0;
}

void destroy_numbers() {
  destroy_sprite(zero);
  destroy_sprite(one);
  destroy_sprite(two);
  destroy_sprite(three);
  destroy_sprite(four);
  destroy_sprite(five);
  destroy_sprite(six);
  destroy_sprite(seven);
  destroy_sprite(eight);
  destroy_sprite(nine);
  destroy_sprite(times);
}
