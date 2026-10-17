#ifndef _CONTROLBASE_H_
#define _CONTROLBASE_H_

#include <lcom/lcf.h>
#include <stdint.h>

#include "devices/video/graphics.h"
#include "devices/timer/timer.h"
#include "devices/kbc/mouse.h"
#include "devices/kbc/keyboard.h"
#include "devices/serialPort/serial_port.h"
#include "settings.h"
#include "helper.h"
#include "view/sprite.h"
#include "view/view.h"
#include "view/view_menu.h"
#include "view/view_game.h"
#include "../model/player.h"
#include "../model/table.h"


extern int counter;
extern uint8_t scancode; 
extern struct packet mouse_data;
extern uint8_t byte_pos;
extern MouseLoc mouse_loc;
extern Player p1;
extern Player p2;
extern Player pc;
extern GameCoins g_coins;
extern char to_save[50];


typedef enum {
  MENU,
  GAME,
  LOOSE,
  ANIM,
  STAND,
  NO_WINNER,
  WIN,
  CHIP,
  EDIT,
  TABLE,
  WAIT,
  OPRDY,
  END
} GameState;


int accept_duel();


#endif
