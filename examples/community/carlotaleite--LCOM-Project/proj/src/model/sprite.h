
#pragma once
#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdio.h>
#include "xpm_images/game_over.xpm"
#include "xpm_images/apple.xpm"
#include "xpm_images/mini_apple.xpm"

typedef struct {
int x, y; // current position
int width, height; // dimensions
//int xspeed, yspeed; // will not need it since ours does not move
uint32_t *map; // the pixmap
} Sprite;

Sprite *create_sprite(xpm_map_t pic);

void destroy_sprite(Sprite *sp);

int draw_sprite(Sprite *sp, int x, int y);

void create_sprites();

void destroy_sprites();

void render_game_over(int x, int y);

void render_apple(int x, int y);
