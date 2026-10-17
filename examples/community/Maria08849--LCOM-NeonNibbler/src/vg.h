#pragma once

#include <lcom/lcf.h>
#include <minix/sysutil.h>

#include "rtc.h"
#include "sprite.h"

void *(vg_init) (uint16_t mode);
int(vg_store_menu_background)();
int(vg_store_game_background)();
void(free_buffers)();

int(vg_copy_menu_background_buffer)();
int(vg_copy_game_background_buffer)();
int(vg_copy_back_buffer)();

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int(vg_draw_pixel_in_buffer)(uint8_t *buffer, uint16_t x, uint16_t y, uint32_t color);

int(vg_fill_screen)(uint32_t color);

int(vg_draw_sprite)(Sprite *sp, uint16_t x, uint16_t y);
int(vg_draw_sprite_in_buffer)(uint8_t *buffer, Sprite *sp, uint16_t x, uint16_t y);
int(vg_draw_part_of_sprite)(Sprite *sp, uint16_t x, uint16_t y, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf);
int(vg_draw_ghost_score)(uint16_t x, uint16_t y, uint8_t ghost_score);

int(vg_draw_menu_nplay)();
int(vg_draw_menu_splay)();
int(vg_draw_collectibles)();
int(vg_draw_powerups)();

bool(gameOver)();

uint8_t(getBitsPerPixel)();
uint8_t(getBytesPerPixel)();
uint16_t(getYResolution)();
uint16_t(getXResolution)();

bool(will_collide_with_background)(Sprite *sprite, uint16_t nextX, uint16_t nextY);
bool(sprites_collided)(Sprite *sp1, Sprite *sp2, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
bool(will_collide_with_powerup)(Sprite *sprite, uint16_t x, uint16_t y);
bool(will_collide_with_collectible)(Sprite *sprite, uint16_t x, uint16_t y);

int(vg_draw_colon)(uint16_t x, uint16_t y);
int(vg_draw_g)(uint16_t x, uint16_t y);
int(vg_draw_h)(uint16_t x, uint16_t y);
int(vg_draw_o)(uint16_t x, uint16_t y);
int(vg_draw_s)(uint16_t x, uint16_t y);
int(vg_draw_t)(uint16_t x, uint16_t y);
int(vg_draw_c)(uint16_t x, uint16_t y);
int(vg_draw_e)(uint16_t x, uint16_t y);
int(vg_draw_r)(uint16_t x, uint16_t y);
int(vg_draw_0)(uint16_t x, uint16_t y);
int(vg_draw_1)(uint16_t x, uint16_t y);
int(vg_draw_2)(uint16_t x, uint16_t y);
