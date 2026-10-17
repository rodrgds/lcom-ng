#include <lcom/lcf.h>
#include "proj.h"

extern SystemState systemState;
extern MenuState menuState;


int (main)(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/g3/proj/src/trace.txt");
  lcf_log_output("/home/lcom/labs/g3/proj/src/output.txt");
  if (lcf_start(argc, argv)) return 1;
  lcf_cleanup();
  return 0;
}

/**
 * @brief Initializes everything needed for the program.
 * @return 0 if successful, 1 otherwise
*/
int setup(){ 
  uint8_t bit_no;

  sprites_setup();

  if (timer_set_frequency(TIMER, 60)) return 1;

  // video buffer initialization (double buffering)
  if (design_set_frame_buffers(0x115)) return 1;

  // graphics mode initialization
  if (graphics_set_mode(0x115)) return 1;

  // device's interrupts initialization
  if (timer_interrupts_subscription(&bit_no)) return 1;
  if (keyboard_interrupts_subscription(&bit_no)) return 1;
  if (mouse_interrupts_subscription(&bit_no)) return 1;
  if (rtc_interrupts_subscription(&bit_no)) return 1;
  if (rtc_initialize()) return 1;

  // activate mouse stream-mode and data report
  if (mouse_write_command(ENABLE_STREAM_MODE)) return 1;
  if (mouse_write_command(ENABLE_DATA_REPORT)) return 1;

  return 0;
}

/**
 * @brief Cleans up the program (goes back to text mode, unsubscribes interrupts and destroys all created sprites).
 * This function **must** be called before exiting the program.
 * @return 0 if successful, 1 otherwise
*/
int proj_cleanup(){  
  // back to text mode
  if (vg_exit()) return 1;

  sprites_destroy();

  // device's interrupts deactivation
  if (timer_interrupts_unsubscription()) return 1;
  if (keyboard_interrupts_unsubscription()) return 1;
  if (mouse_interrupts_unsubscription()) return 1;
  if (rtc_interrupts_unsubscription()) return 1;

  // deactivate mouse stream-mode and data report
  if (mouse_write_command(DISABLE_DATA_REPORT)) return 1;
  if (mouse_write_command(DISABLE_DATA_REPORT)) return 1;

  return 0;
}

/**
 * @brief Main loop of the program.
 * @param argc number of arguments
 * @param argv array of arguments
 * @return 0 if successful, 1 otherwise
*/
int(proj_main_loop)(int argc, char* argv[]){
  if (setup()) return 1;
  
  design_draw_new_frame();

  int ipc_status;
  message msg;
  while (systemState == RUNNING) {
    
    if (driver_receive(ANY, &msg, &ipc_status)) {
      printf("Error");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & TIMER_MASK)    timer_update_state();  
          if (msg.m_notify.interrupts & KEYBOARD_MASK) keyboard_update_state();
          if (msg.m_notify.interrupts & MOUSE_MASK)    mouse_update_state();
          if (msg.m_notify.interrupts & RTC_MASK)      update_rtc_state();
        }
    }
  }
  
  if (proj_cleanup()) return 1;

  return 0;
}
