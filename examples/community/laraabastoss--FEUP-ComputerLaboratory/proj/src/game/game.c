#include <stdio.h>
#include <stdlib.h>

#include "game.h"

extern ProjState projState;
extern GameState gameState;
extern MouseInfo mouse_packet;
extern uint8_t mouse_index;

void game_loop(){
    int ipc_status;
    message msg;

    while (projState != EXIT) {
    
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Error");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
        switch(_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: 
            if (msg.m_notify.interrupts & BIT(0)){
                handle_int_timer();
            }
            if (msg.m_notify.interrupts & BIT(1)){
                if (gameState == MP_GAME){
                (keyboard_interrupt_handler)();  
                handle_int_keyboard_multiplayer_transmit();
                }
                else {
                (keyboard_interrupt_handler)();  
                handle_int_keyboard();
                }
            } 
            if (msg.m_notify.interrupts & BIT(2)){
                (mouse_interrupt_handler)();
                if (mouse_index == 3) {
                    mouse_fill_packet();
                    if (mouse_packet.left_click) { 
                        handle_int_mouse_click();
                    }
                    new_frame();
                    mouse_index = 0;
                }
            }    
            if (msg.m_notify.interrupts & 5)   {
                handle_int_rtc();
            }
            if (msg.m_notify.interrupts & BIT(7))    {
                    sp_process_data();
                    handle_sp_data_multiplayer();
                    handle_int_sp();
            } 

        }
    }
  }
}
