#include <lcom/lcf.h>
#include "game/draw/draw.h"
#include "game/game.h"
#include "game/context.h"

uint8_t TIMER_MASK, KEYBOARD_MASK, MOUSE_MASK;
extern struct gamecontext context;


int main(int argc, char *argv[]) {

  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/proj/src/debug/trace.txt");
  lcf_log_output("/home/lcom/labs/proj/src/debug/output.txt");
  if(lcf_start(argc, argv)) return 1;
  lcf_cleanup();

  return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {

  if(start_game() != 0){
    if(finish_game() != 0) return 1;
    return 0;
  }

  draw_screen();

  int ipc_status;
  message msg;
  while (context.gamestatus == ON) {
    
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Error");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & TIMER_MASK)    handle_timer_interrupt();
          if (msg.m_notify.interrupts & KEYBOARD_MASK) handle_keyboard_interrupt();
          if (msg.m_notify.interrupts & MOUSE_MASK) handle_mouse_interrupt();
        }
    }
  }
  
  if(finish_game() != 0) return 1;

  return 0;
}

