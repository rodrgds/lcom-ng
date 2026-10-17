#ifndef _MAINMENUCONTROLLER_H_
#define _MAINMENUCONTROLLER_H_

#include <lcom/lcf.h>
#include <stdio.h>

#include "Controller/Devices/VideoCard/video_graphics.h"
#include "../../Model/SelectorMine/selectorMine.h"
#include "../state.h"
#include "../scancodes.h"

typedef enum {PLAY, INSTRUCTIONS, EXIT} option_menu;

#define SEL_PLAY_X 505
#define SEL_PLAY_Y 250
#define SEL_INST_X 607
#define SEL_INST_Y 350
#define SEL_EXIT_X 496
#define SEL_EXIT_Y 450

extern uint8_t scan_code;

void handleScanCodeMainMenu(SMine* mine, game_state* state);

#endif
