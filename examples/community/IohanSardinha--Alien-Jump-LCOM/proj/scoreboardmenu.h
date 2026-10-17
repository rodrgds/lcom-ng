#include <lcom/lcf.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "graphics.h"
#include "sprites.h"
#include "i8042.h"
#include "game.h"

void score_board_menu_frame();

void score_board_menu_keyboard(uint8_t data);

void score_board_menu_mouse(struct packet pckt);
