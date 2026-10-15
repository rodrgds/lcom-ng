#include <lcom/lcf.h>
#include <lcom/lab5.h>
#include <stdint.h>
#include <stdio.h>
#include "video.h"
#include "alien.h"
#include "sprites.h"
#include "i8042.h"

#define ALIEN_START_X 20
#define ALIEN_START_Y 50
#define ALIEN_SPACING_X 60
#define ALIEN_SPACING_Y 45
#define ALIEN_MOVE_DISTANCE 10

extern uint8_t* alien_sprite;
extern xpm_image_t img_alien_sprite;
uint8_t* alien_black_sprite;
extern xpm_image_t img_alien_black_sprite;
Alien aliens[ALIEN_COUNT];
int alien_shoot_interval = 180;  // 3 seconds default

uint16_t screen_width;
uint16_t screen_height;
bool moving_right = true;

void init_aliens(uint16_t x_res, uint16_t y_res) {
  screen_width = x_res;
  screen_height = y_res;
  for (int row = 0; row < ALIEN_ROWS; row++) {
    for (int col = 0; col < ALIEN_COLUMNS; col++) {
      int i = row * ALIEN_COLUMNS + col;
      aliens[i].x = ALIEN_START_X + col * ALIEN_SPACING_X;
      aliens[i].y = ALIEN_START_Y + row * ALIEN_SPACING_Y;
      aliens[i].alive = true;
    }
  }
}

int draw_alien(uint16_t x, uint16_t y) {
  if (draw_xpm(alien_sprite, img_alien_sprite, x, y) != 0) {
    printf("xpm failed to load\n");
    return 1;
  }
  return 0;
}

int delete_alien(uint16_t x, uint16_t y) {
  if (draw_xpm(alien_black_sprite, img_alien_black_sprite, x, y) != 0) {
    printf("xpm failed to load\n");
    return 1;
  }
  return 0;
}

void draw_aliens() {
  for (int i = 0; i < ALIEN_COUNT; i++) {
    if (aliens[i].alive) {
      if (draw_alien(aliens[i].x, aliens[i].y) != 0) {
        printf("failed to draw alien #%d at (%d, %d)\n", i, aliens[i].x, aliens[i].y);
      }
    }
  }
}

void set_alien_dead(int index) {
  if (index >= 0 && index < ALIEN_COUNT) {
    aliens[index].alive = false;
    if (delete_alien(aliens[index].x, aliens[index].y) != 0) {
      printf("failed to delete alien #%d at (%d, %d)\n", index, aliens[index].x, aliens[index].y);
    }
  }
}

void move_aliens() {
  bool hit_edge = false;

  for (int i = 0; i < ALIEN_COUNT; i++) {
    if (aliens[i].alive) {
      if (moving_right && (aliens[i].x + ALIEN_SPACING_X >= screen_width)) {
        hit_edge = true;
        break;
      } else if (!moving_right && (aliens[i].x <= ALIEN_MOVE_DISTANCE)) {
        hit_edge = true;
        break;
      }
    }
  }

  for (int i = 0; i < ALIEN_COUNT; i++) {
    if (aliens[i].alive) {
      if (delete_alien(aliens[i].x, aliens[i].y) != 0) {
        printf("failed to delete alien #%d at (%d, %d)\n", i, aliens[i].x, aliens[i].y);
      }
    }
  }

  if (hit_edge) {
    for (int i = 0; i < ALIEN_COUNT; i++) {
      aliens[i].y += ALIEN_MOVE_DISTANCE; 
    }
    moving_right = !moving_right; 
  } else {
    for (int i = 0; i < ALIEN_COUNT; i++) {
      if (aliens[i].alive) {
        aliens[i].x += moving_right ? ALIEN_MOVE_DISTANCE : -ALIEN_MOVE_DISTANCE;
      }
    }
  }
}

bool check_all_aliens_dead() {
  for (int i = 0; i < ALIEN_COUNT; i++) {
    if (aliens[i].alive) {
      return false;
    }
  }
  return true;
}
