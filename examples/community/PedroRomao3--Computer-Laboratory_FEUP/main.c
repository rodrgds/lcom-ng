#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "devices/video/graphics.h"
#include "devices/timer/timer.h"
#include "devices/kbc/mouse.h"
#include "devices/kbc/keyboard.h"
#include "settings.h"
#include "controller/control.h"
#include "devices/serialPort/serial_port.h"
#include "view/view_menu.h"

extern GameState state;

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");

  lcf_trace_calls("/home/lcom/labs/g5/proj/src/output/trace.txt");

  lcf_log_output("/home/lcom/labs/g5/proj/src/output/output.txt");

  if (lcf_start(argc, argv)) return 1;

  lcf_cleanup();

  return 0;
}


int setup(){
  if (timer_set_frequency(0, GAME_FREQUENCY) != 0) return 1;
  if (change_graphics_mode(VIDEO_MODE) != 0) return 1;
  
  uint8_t mk = 0, tk = 0, kk = 0,sk = 0;
  //serp
  //write to FCR
  if(fifo_en()){printf("fifo error");return 1;}
  if(setup_lcr(8,1)){printf("lcr error");return 1;}
  if(ier_enable_receive()){printf("ier rec error");return 1;}
  if(ier_enable_transmit_lsr()){printf("transmit error");return 1;}
  if(set_bit_rate(10)){printf("bit rate error");return 1;}
  if(serial_subscribe_int(&sk)){printf("ser sub error");return 1;}
  //serp end

  if (timer_subscribe_int(&tk)) return 1;
  if (keyboard_subscribe_int(&kk)) return 1;
  if (mouse_subscribe_int(&mk)) return 1;
  if (send_mouse_command(ENABLE_STREAM_MODE)) return 1;
  if (send_mouse_command(ENABLE_DATA_REPORT)) return 1;
  while (set_time() == 1){}
  createAllSprites();
  drawSprite(background, 0, 0);
  return 0;
}

int clouse(){
  if (serp_undo()) return 1;
  if (mouse_unsubscribe_int()) return 1;
  if (keyboard_unsubscribe_int()) return 1;
  if (send_mouse_command(DISABLE_DATA_REPORT)) return 1;
  if (timer_unsubscribe_int()) return 1;
  destroyAllSprites();
  if (vg_exit() != 0) return 1;
  return 0;
}

int (proj_main_loop)(int argc, char *argv[]){
  if(setup()){
    clouse();
    return 1;
  }

  int ipc_status;
  message msg;
  
  while (state != END){

    if (driver_receive(ANY, &msg, &ipc_status) != 0) { 
      printf("driver_receive failed \n");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & mask_timer) {
            timer_interrupt();
          }
          if (msg.m_notify.interrupts & mask_keyboard){
            keyboard_interrupt();
          }
          if (msg.m_notify.interrupts & mask_mouse){
            mouse_interrupt();
          }
          if (msg.m_notify.interrupts & mask_ser_p){
            ser_p_interrupt();
          }  
          break;
        default:
          break; 
      }
    }
  }
  clouse();
  return 0;
}
