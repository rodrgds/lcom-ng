#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <lcom/lcf.h>

void game_assets_init(const vbe_mode_info_t *mode_info);
void game_assets_destroy(void);
void game_draw(void);
void game_update(void);
void game_handle_keyboard(uint8_t *scancode, size_t size);
void game_handle_mouse(const struct packet *p);
void draw_life();
void draw_platform();
void draw_ball();
void reset_game();
GameState show_gameover_screen();

#endif 
