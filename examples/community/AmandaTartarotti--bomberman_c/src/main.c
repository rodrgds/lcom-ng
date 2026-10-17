#include <lcom/lcf.h>
#include <stdbool.h>
#include "../inc/pixmap.h"
#include "../inc/graphic.h"
#include "../inc/timer.h"
#include "../inc/controller.h"
#include "../inc/keyboard.h"
#include "../inc/mouse.h"

extern uint8_t mouse_mask;
extern uint8_t mouse_data;
extern uint8_t timer_mask;
extern uint8_t keyboard_mask;
extern uint8_t rtc_mask;
extern char map[15][20];
extern bool change_vram_mode;

int main(int argc, char *argv[]){
    lcf_set_language("EN-US");
    lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
    lcf_log_output("/home/lcom/labs/proj/output.txt");
    if (argc > 2) {
        change_vram_mode = true;
        argc--;
    }

    if (lcf_start(argc, argv)) return 1;
    
    lcf_cleanup();

    return 0;
}

int proj_main_loop() {

    if(setup()) return 1;
  
    int ipc_status;
    
    message msg;

    bool mouse_int = true;

    uint8_t mouse_index = 0;
    struct packet pp;

    
    while (breakCondition()){
    if (driver_receive(ANY, &msg, &ipc_status) != 0) { 
      printf("driver_receive failed");
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & timer_mask) 
            if(timer_update()) return 1;
          if (msg.m_notify.interrupts & keyboard_mask)
            {
            kbc_ih();
            gamepad_response();
            }
          if (msg.m_notify.interrupts & rtc_mask)
            {
              rtc_ih();
            }
          if (msg.m_notify.interrupts & mouse_mask)
            {
              mouse_ih();
              if(mouse_index == 0 && (mouse_data & BIT(3))){
                pp.bytes[0] = mouse_data;
                if(mouse_data & BIT(0)) pp.lb = true;
                if(mouse_data & BIT(1)) pp.rb = true;
                if(mouse_data & BIT(2)) pp.mb = true;
                if(mouse_data & BIT(4)){
                  pp.delta_x = 0xff00;
                } 
                if(mouse_data & BIT(5)){
                  pp.delta_y = 0xff00;
                } 
                if(mouse_data & BIT(6)) pp.x_ov = true;
                if(mouse_data & BIT(7)) pp.y_ov = true;
                mouse_index++;
                continue;
              }
              else if(mouse_index == 1){
                pp.bytes[1] = mouse_data;
                pp.delta_x = (pp.delta_x | mouse_data);
                mouse_index ++;
                continue;
              }
              else if(mouse_index == 2){
                pp.bytes[2] = mouse_data;
                pp.delta_y = (pp.delta_y | mouse_data);
                arrow_update(pp.delta_x, pp.delta_y);
                int direction = update_mouse(pp.delta_x, pp.delta_y, pp.rb, mouse_int);
                if(mouse_response(direction)) return 1;
                mouse_index =0;
                pp.lb= false; pp.rb=false;pp.mb=false; pp.delta_x=0x0; pp.delta_y=0x0;pp.x_ov=false;pp.y_ov=false;
                if (mouse_int) mouse_int = false;
              }
            }  
          break;
        default:
          break; 
      }
    }
  }
     
    
    if(setdown()) return 1;

    return 0;
}

