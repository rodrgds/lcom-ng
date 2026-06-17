#ifndef _LOOP_H_
#define _LOOP_H_

#include "system.h"
#include "input.h"
#include "state.h"
#include "timer.h"
#include "constants.h"
#include "scene.h"
#include "video.h"
#include "render.h"
#include "zombie.h"
#include "wave.h"
#include "world.h"
#include "board.h"
#include "lawnmower.h"
#include "plant_select.h"
#include "rtc.h"
// Starts the main game loop that handles hardware interrupts
int game_loop(void);

#endif
