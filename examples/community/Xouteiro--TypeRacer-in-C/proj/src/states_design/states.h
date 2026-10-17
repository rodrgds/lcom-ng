#ifndef _LCOM_MODEL_H_
#define _LCOM_MODEL_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "controller/keyboard_mouse/KBC.h"
#include "controller/keyboard_mouse/mouse.h"
#include "controller/video/graphics.h"
#include "controller/rtc/rtc.h"
#include "design.h"
#include "xpm/typo_racer.xpm"
#include "xpm/play_button.xpm"
#include "xpm/mouse.xpm"
#include "xpm/exit_button.xpm"
#include "xpm/esc_menu.xpm"
#include "xpm/letters.xpm"
#include "xpm/numbers.xpm"
#include "xpm/you_win.xpm"
#include "xpm/play_again.xpm"
#include "xpm/accuracy.xpm"
#include "game/game.h"


typedef enum {
    RUNNING,
    OVER,
} SystemState;

typedef enum {
    START,
    GAME,
    END
} MenuState;

void (timer_update_state)();
void (keyboard_update_state)();
void (mouse_update_state)();
void (menu_buttons_update)();
void (sprites_setup)();
void (sprites_destroy)();
void (update_rtc_state)();
void (end_menu_buttons_update)();


#endif
