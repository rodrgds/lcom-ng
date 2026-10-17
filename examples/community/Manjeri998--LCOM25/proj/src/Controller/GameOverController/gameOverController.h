#ifndef _GAMEOVERCONTROLLER_H_
#define _GAMEOVERCONTROLLER_H_

#include <lcom/lcf.h>
#include <stdio.h>

#include "Controller/Devices/VideoCard/video_graphics.h"
#include "../../Model/SelectorMine/selectorMine.h"
#include "../state.h"
#include "../scancodes.h"

typedef enum {RETRY, EXIT_OVER} option_game_over;

#define SEL_RETRY_X 527
#define SEL_RETRY_Y 240
#define SEL_OVER_EXIT_X 496
#define SEL_OVER_EXIT_Y 440

extern uint8_t scan_code;

void handleScanCodeGameOver(SMine* mine, game_state* state);

#endif
