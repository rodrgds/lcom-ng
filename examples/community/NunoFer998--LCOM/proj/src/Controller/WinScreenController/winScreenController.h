#ifndef _WINSCREENCONTROLLER_H_
#define _WINSCREENCONTROLLER_H_

#include <lcom/lcf.h>
#include <stdio.h>

#include "Controller/Devices/VideoCard/video_graphics.h"
#include "../../Model/SelectorMine/selectorMine.h"
#include "../state.h"
#include "../scancodes.h"

typedef enum {PLAY_AGAIN, EXIT_WIN} option_win;

#define SEL_PLAY_AGAIN_X 505
#define SEL_PLAY_AGAIN_Y 250
#define SEL_EXIT_WIN_X 496
#define SEL_EXIT_WIN_Y 450

extern uint8_t scan_code;

void handleScanCodeWinScreen(SMine* mine, game_state* state);

#endif
