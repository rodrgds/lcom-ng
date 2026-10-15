#ifndef _GAME_H_
#define _GAME_H_

#include "../conf.h"
#include "../keyboard/KBC.h"
#include "../keyboard/keyboard.h"
#include "../timer/timer.h"
#include "../video/VBE.h"
#include "../video/graphics.h"
#include "tile.h"
#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdio.h>


extern int game_timer_counter;
extern bool game_over;
extern Note current_note;
extern vbe_mode_info_t mode_info;
extern NoteQueue note_queue;
extern int spawn_interval;

int(game_loop)(void);

#endif // _GAME_H_
