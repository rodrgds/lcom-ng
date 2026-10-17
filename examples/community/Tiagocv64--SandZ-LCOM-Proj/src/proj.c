#include <lcom/lcf.h>
#include "proj.h"

#include <stdint.h>
#include <stdio.h>

#include "Bitmap.h"
#include "enum.h"

#include "libs/i8042.h"
#include "libs/i8254.h"
#include "libs/serial_port.h"
#include "libs/keyboard.h"
#include "libs/mouse.h"
#include "libs/timer.h"
#include "libs/video.h"
#include "libs/rtc.h"

#include "program.h"

extern enum program_state p_state;
extern enum online_state o_state;
extern unsigned long long counter;
extern bool wasRead_kb;
extern uint8_t data_kb;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  // lcf_trace_calls("/home/lcom/labs/lcom1819-t4g13/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  // lcf_log_output("/home/lcom/labs/proj/output.txt");

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

  if (argc == -1 || *argv[0] == 'T')
    printf("ola\n");

  //Timer interrupts' subscription
  uint8_t irq_timer = 0;
  if (timer_subscribe_int(&irq_timer))
    return 1;


  if (timer_set_frequency(0, 60))
    return 1;

  //Keyboard interrupts' subscription
  uint8_t irq_kbc = 2;
  if (keyboard_subscribe_int(&irq_kbc))
    return 1;

  // Mouse interrupts' subscription
  uint8_t irq_mouse = 3;
  if (mouse_subscribe_int(&irq_mouse))
    return 1;

  // IRC interrupts' subscription
  uint8_t irq_rtc = 4;
  if (rtc_subscribe_int(&irq_rtc))
    return 1;

  if (rtc_backup())
    return 1;

  if (rtc_enable_just_update_interrupts())
    return 1;

  rtc_initial_state_is_day();

  // UART interrupts' subscription
  uint8_t irq_uart = 5;
  if (uart_subscribe_int(&irq_uart))
    return 1;

  if (init_uart_proj())
    return 1;

  void *v_mem = vg_init(VBE_MODE_1280x1024);
  if (v_mem == NULL)
    return 1;

  int ipc_status, r;
  message msg;

  struct packet mouse_pak;

  init_program();

  counter = 0;

  while (p_state != QUIT_PROGRAM && data_kb != ESC_KEY) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_timer) { //MAIN_MENU, LEADERBOARDS, PICK_USER_NAME, PLAYING, PAUSE_GAME, END_GAME
            timer_int_handler();
            handle_timer_interrupt_proj();
          }
          if (msg.m_notify.interrupts & irq_kbc) {
            kbc_ih();
            handle_keyboard_interrupt_proj();
            wasRead_kb = false;
          }
          if (msg.m_notify.interrupts & irq_mouse){
            mouse_ih();
            handle_mouse_interrupt_proj(&mouse_pak);
          }
          if (msg.m_notify.interrupts & irq_rtc){
            rtc_ih();
          }
          if (msg.m_notify.interrupts & irq_uart) {
            uart_ih();
          }
          break;
        default:
          break;
      }
    }
  }

  uart_send_message("s0S", 3);

  destroy_program();

  if (vg_exit())
    return 1;

  if (uart_unsubscribe_int())
    return 1;

  if (rtc_restore())
    return 1;

  if (rtc_unsubscribe_int())
    return 1;

  if (mouse_unsubscribe_int())
    return 1;

  if (keyboard_unsubscribe_int())
    return 1;

  if (timer_unsubscribe_int())
    return 1;


  return 0;
}
