#ifndef STATES_H
#define STATES_H


#include <lcom/lcf.h>
#include <lcom/proj.h>
#include "Modules/mouse.h"
#include "controller/menu_controller.h"


typedef enum {
    STATE_MENU,
    STATE_GAME,
    STATE_EXIT
} AppState;

typedef struct {
    AppState next_state;
    GameConfig config;
} MenuResult;



MenuResult menu_state(uint8_t kbd_irq,uint8_t timer_irq);
int game_state(GameConfig config ,uint8_t kbd_irq, uint8_t timer_irq);

#endif
