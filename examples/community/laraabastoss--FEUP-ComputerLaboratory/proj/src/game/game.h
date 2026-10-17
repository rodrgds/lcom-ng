#ifndef _LCOM_GAME_H_
#define _LCOM_GAME_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "devices/timer/timer.h"
#include "devices/video/graphics.h"
#include "devices/keyboard/keyboard.h"
#include "devices/mouse/mouse.h"
#include "devices/rtc/rtc.h"
#include "devices/sp/serial_port.h"
#include "handle_int.h"
#include "game/elements.h"

void game_loop();

#endif
