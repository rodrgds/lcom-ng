#include <lcom/lcf.h>
#include <lcom/proj.h>

#include <stdint.h>

#include "gpu/gpu.h"
#include "kbd_mouse/kbd.h"
#include "kbd_mouse/mouse.h"
#include "mvc/controller.h"
#include "mvc/model.h"
#include "mvc/viewer.h"
#include "mvc/xpms.h"
#include "rtc/rtc.h"
#include "serialport/serialport.h"
#include <lcom/timer.h>
// #include "serialport/serialp.h"

// temporary
extern uint8_t scancode;
extern uint8_t byte_index;
extern game_state_t game_state;
extern struct packet mouse_packet;

bool exit_game = false;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/g3/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g3/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {
  uint8_t hook_id_tmr = 0;
  uint8_t hook_id_kdb = 1;
  uint8_t hook_id_mouse = 2;
  uint8_t hook_id_rtc = 3;
  uint8_t hook_id_serialp = 4;

  if (timer_subscribe_int(&hook_id_tmr) != 0) {
    printf("Error in timer_sub_int()\n");
    return 1;
  }

  if (kbd_subscribe_int(&hook_id_kdb) != 0) {
    printf("Error in kb_subscribe_int()\n");
    return 1;
  }

  if(mouse_write(0xEA)!=0){ //enable stream mode
    printf("Error in mouse_write()\n");
    return 1;
  }
  
  if (mouse_write(0xF4) != 0) { // enable data reporting
    printf("Error in mouse_write()\n");
    return 1;
  }

  if (mouse_subscribe_int(&hook_id_mouse) != 0) {
    printf("Error in mouse_subscribe_int()\n");
    return 1;
  }

  if (rtc_subscribe_int(&hook_id_rtc) != 0) {
    printf("Error in rtc_subscribe_int()\n");
    return 1;
  }

  //update real time for the first time
  rtc_update_real_time();

  if(serialp_subscribe_int(&hook_id_serialp) != 0){
      printf("Error in serialp_subscribe_int()\n");
      return 1;
  }

  // start graphics
  build_frame_buffer(GPU_800x600);
  if (set_graphics_mode(GPU_800x600) != 0) {
    vg_exit();
    printf("Error in set_graphics_mode()\n");
    return 1;
  }

  load_xpms();

  draw_menu();

  swap_buffer();

  int ipc_status;
  message msg;
  int r;


  while (!exit_game) {
    
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:

          if (msg.m_notify.interrupts & hook_id_kdb) {
            kbc_ih();
            game_states_list[game_state].controller(KEYBOARD);
          }

          if (msg.m_notify.interrupts & hook_id_tmr) {
            timer_int_handler();
            game_states_list[game_state].controller(TIMER);
          }

          if (msg.m_notify.interrupts & hook_id_mouse) {
            mouse_ih();

            // sync packets
            // mouse_sync_packet();
            if(byte_index > 2){         
              mouse_bytes_to_packet(&mouse_packet);
              byte_index = 0;
              game_states_list[game_state].controller(MOUSE);
            }
          }

          if (msg.m_notify.interrupts & hook_id_rtc) {
            rtc_ih();
          }

          if (msg.m_notify.interrupts & hook_id_serialp) {
              serialp_ih();
              // read_data_serialp();
              game_states_list[game_state].controller(SERIALPORT);
          }

          break;

        default:
          break;
      }
    }
  }

  DEBUG_MSG("GAME STATE: %d\n", game_state);

  if (timer_unsubscribe_int() != 0) {
    printf("Error in timer_unsubscribe_int()\n");
    return 1;
  }

  if (kbd_unsubscribe_int() != 0) {
    printf("Error in kbd_unsubscribe_int()\n");
    return 1;
  }

  if (mouse_write(0xF5) != 0) { // disable data reporting
    printf("Error in mouse_write()\n");
    return 1;
  }

  if (mouse_unsubscribe_int() != 0) {
    printf("Error in mouse_unsubscribe_int()\n");
    return 1;
  }

  if (rtc_unsubscribe_int() != 0) {
    printf("Error in rtc_unsubscribe_int()\n");
    return 1;
  }


  if(serialp_unsubscribe_int() != 0){
      printf("Error in serialp_unsubscribe_int()\n");
      return 1;
  }

  DEBUG_MSG("Exiting program\n");
  vg_exit();
  DEBUG_MSG("Exited graphics mode\n");

  return 0;
}
