#ifndef _RENDER_H_
#define _RENDER_H_

#include "pvz_platform.h"
#include <stdint.h>
#include "lawnmower.h"
#include "plant.h"
#include "zombie.h"
#include "constants.h"
#include "video.h"
#include "sprites.h"


void draw_plant(Plant* plant);
void draw_map(void);
void draw_cursor(int x, int y);
void draw_zombie(Zombie* zombie);
void draw_bullet(int x, int y, int frame);
void draw_cabbage_projectile(int x, int y, int frame);
void draw_sun(int x, int y, int frame);
void draw_hud(void);
void draw_seed_bank(PlantType selected_type);
void draw_seed_cards(void);
void draw_holding_plant_preview(int mouse_x, int mouse_y);
void draw_keyboard_grid_selection(int row, int col);
void draw_keyboard_plant_preview(int row, int col, PlantType type);
void draw_shovel(int x, int y);
void draw_shovel_cursor(int mouse_x, int mouse_y);
void draw_shovel_highlight(int x, int y);
void draw_lawnmowers(void);

// Load/free all plant sprites (call once at startup/cleanup)
int load_all_sprites(void);
void free_all_sprites(void);

#endif /* _RENDER_H_ */
