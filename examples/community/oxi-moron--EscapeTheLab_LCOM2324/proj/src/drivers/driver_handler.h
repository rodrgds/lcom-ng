#ifndef G6_DRIVER_HANDLER_H
#define G6_DRIVER_HANDLER_H

#include <lcom/lcf.h>

#include "rtc.h"
#include "kbc.h"
#include "video.h"
#include "timer.h"
#include "mouse.h"
#include "serial_port.h"
#include "uart_macros.h"
#include "i8024_kbc.h"
#include "i8254_timer.h"
#include "rtc_macros.h"
#include "../game/game_state.h"
#include "../game/player.h"
#include "VBE.h"

int driver_setup();
int driver_cleanup();

int main_event_loop();

#endif //G6_DRIVER_HANDLER_H
