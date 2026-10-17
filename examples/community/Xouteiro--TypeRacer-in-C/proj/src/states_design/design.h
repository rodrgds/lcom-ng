#ifndef _LCOM_MENU_H_
#define _LCOM_MENU_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "controller/video/graphics.h"
#include "controller/keyboard_mouse/mouse.h"
#include "states.h"
#include "sprite/sprite.h"
#include "game/game.h"

void (design_draw_new_frame)();
void (design_draw_initial_menu)();
void (design_draw_game_menu)();
void (design_draw_finish_menu)();
void (design_draw_mouse)();
void (design_swap_buffers)();
int (design_draw_sprite_xpm)(Sprite *sprite, int x, int y);
int (design_draw_sprite_button)(Sprite *sprite, int x, int y);
int (design_set_frame_buffers)(uint16_t mode);
int (phrase_writer)(char* word, int y_line);
int (wpm_writer)(int wpm);
int (accuracy_writer)(int acc);
int (is_lower)(char letter);
int (is_upper)(char letter);
int (is_number)(char letter);

#endif
