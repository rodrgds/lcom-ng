#include "game.h"
#include <lcom/lcf.h>
#include "../devices/timer/timer.h"
#include "../devices/graphics/graphics.h"
#include "../devices/keyboard/keyboard.h"
#include "../devices/mouse/mouse.h"
#include "interrupts_actions/interrupts.h"
#include "allocations_manager/allocations_manager.h"


extern mouse_t mouse_structure;
extern struct gamecontext context;
extern uint8_t TIMER_MASK, KEYBOARD_MASK, MOUSE_MASK;

int(start_game)(){

    if(timer_set_frequency(0, 60) != 0) return 1;
    if(changeTo_graphic_mode(0x115) != 0) return 1;
    if(allocate_double_buffer(0x115) != 0) return 1;

    manage_game_allocations();
    
    if(mouse_write_input(0xEA) != 0) return 1;
    if(mouse_write_input(0xF4) != 0) return 1;

    if(timer_subscribe_int(&TIMER_MASK) != 0) return 1;
    if(keyboard_subscribe_int(&KEYBOARD_MASK) != 0) return 1;
    if(mouse_subscribe_int(&MOUSE_MASK) != 0) return 1;

    manage_start_states();
    return 0;
}


int(finish_game)(){
  if(vg_exit() != 0) return 1;

  manage_game_deletes();

  if(timer_unsubscribe_int() != 0) return 1;
  if(keyboard_unsubscribe_int() != 0) return 1;
  if(mouse_unsubscribe_int() != 0) return 1;

  if (mouse_write_input(0xF5) != 0) return 1;

  return 0;
}
