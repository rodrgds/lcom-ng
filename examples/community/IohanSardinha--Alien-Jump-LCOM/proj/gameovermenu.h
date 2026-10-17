#include <lcom/lcf.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"
#include "graphics.h"
#include "sprites.h"
#include "i8042.h"
#include "game.h"
#include "RTC.h"

void game_over_menu_frame();

void game_over_menu_keyboard(uint8_t data);

void game_over_menu_mouse(struct packet pckt);
