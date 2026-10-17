#include <lcom/lcf.h>
#include "utils.h"
#include "graphics.h"
#include "sprites.h"
#include "i8042.h"
#include "game.h"

void main_menu_frame();

void main_menu_keyboard(uint8_t data);

void main_menu_mouse(struct packet pckt);

void load_sprites();
