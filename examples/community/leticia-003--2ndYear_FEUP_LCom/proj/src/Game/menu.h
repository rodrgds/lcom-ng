#ifndef MENU_H
#define MENU_H

#include <stdint.h>
#include <stdbool.h>
#include "game_state.h"
#include "Drivers/VBE/vbe.h"
#include <stddef.h>
#include <lcom/xpm.h>

extern bool needs_redraw;

void menu_draw();
void menu_update_keyboard(uint8_t *scancode, uint8_t scancode_size);
GameState menu_get_state();
int menu_assets_init(const vbe_mode_info_t *mode_info);
void menu_assets_destroy(void);
void menu_draw_start(uint16_t x, uint16_t y, bool selected);

#endif
