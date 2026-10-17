#ifndef _LCOM_GAME_H_
#define _LCOM_GAME_H_
#include <lcom/lcf.h>
#include "i8042.h"
#include "violetchi.h"
#include "mouse.h"


//ENUMS

typedef enum {
  MENU,
  GAME,
  END
} States;

typedef enum {
  FOOD,
  ENERGY,
  BATH,
  LOVE,
  HAVE_FUN,
  PLAY_GAME,
  EXIT
} onButton;


//STRUCTS

typedef struct {
  struct packet *packet;
  int x;
  int y;
  bool action_completed;
  bool in_action;
} Mouse;


typedef struct {
    Violetchi violetchi;
    States state;
    onButton button;
    Mouse mouse;
    bool action; 
} Game;

void(start_game)(void);

int (play_game)(void);

int (update_state)();

int (update_love_state)();

#endif
