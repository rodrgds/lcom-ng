#ifndef _LCOM_EVENT_HANDLER_H
#define _LCOM_EVENT_HANDLER_H

#include <lcom/lcf.h>

#include "game/game.h"
#include "game/menu.h"
#include "game/gameover.h"
#include "game/wait_menu.h"
#include "utils.h"

typedef enum State {
  MENU,
  WAIT,
  GAME,
  GAMEOVER,
  ENDGAME
} State;

State(handle_timer)(State state, rtc_time *timeRTC);

State(handle_keyboard)(State state, uint8_t *keyboardBytes);

State(handle_mouse)(State state, struct mousePacket *pp);

State(handle_serial)(State state);
#endif /* _LCOM_EVENT_HANDLER_H */
