#include <lcom/lcf.h>
#include <lcom/proj.h>

#include "devices/graphics/graphics.h"
#include "devices/kbc/keyboard.h"
#include "devices/kbc/mouse.h"
#include "devices/uart/uart.h"
#include "event_handler.h"
#include "game/sprite.h"
#include <lcom/timer.h>

struct mousePacket mouse_packet;
uint8_t keyboard_scancode[2];
enum State state = MENU;
rtc_time timeRTC;

int(main)(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");
  lcf_log_output("/home/lcom/labs/proj/src/output.txt");

  if (lcf_start(argc, argv))
    return 1;

  lcf_cleanup();

  return 0;
}

int(proj_main_loop)(int argc, char **argv) {
  uint8_t keyboard_irq_set, mouse_irq_set, timer_irq_set, ser_irq_set;

  if (rtc_read_time(&timeRTC) != 0) {
    printf("Error in rtc_read_time inside: %s\n", __func__);
    return EXIT_FAILURE;
  }

  conf_t config;
  config.bit_rate = 115200;
  config.no_bits = 8;
  config.stop_bits = 1;
  config.parity = 1;

  if (ser_conf(0x3F8, config) != OK)
    return EXIT_FAILURE;
  if (set_graphics_mode(DIRECT_COLOR_WITH_32BITS) != OK)
    return EXIT_FAILURE;
  if (set_frame_buffer(DIRECT_COLOR_WITH_32BITS) != OK)
    return EXIT_FAILURE;
  if (timer_subscribe_int(&timer_irq_set) != OK)
    return EXIT_FAILURE;
  if (kbc_subscribe_int(&keyboard_irq_set) != OK)
    return EXIT_FAILURE;
  if (ser_subscribe_int(&ser_irq_set) != OK)
    return EXIT_FAILURE;
  if (mouse_enable_scrolling() != OK)
    return EXIT_FAILURE;
  if (mouse_enable_data_reporting_mine() != OK)
    return EXIT_FAILURE;
  if (mouse_subscribe_int(&mouse_irq_set) != OK)
    return EXIT_FAILURE;
  load_sprites();

  message msg;
  int ipc_status, r;
  while (state != ENDGAME) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source == HARDWARE)) {

      if (msg.m_notify.interrupts & BIT(timer_irq_set)) {
        rtc_update_time(&timeRTC);
        state = handle_timer(state, &timeRTC);
      }

      if (msg.m_notify.interrupts & BIT(keyboard_irq_set)) {
        kbc_ih();
        if (kbc_scancode_is_done()) {
          kbc_get_scancode(keyboard_scancode);
          state = handle_keyboard(state, keyboard_scancode);
        }
      }

      if (msg.m_notify.interrupts & BIT(ser_irq_set)) {
        ser_ih();
        handle_serial(state);
      }

      if (msg.m_notify.interrupts & BIT(mouse_irq_set)) {
        mouse_ih();
        if (mouse_packet_is_done()) {
          mouse_get_packet(&mouse_packet);
          state = handle_mouse(state, &mouse_packet);
        }
      }
    }
  }

  delete_sprites();

  if (mouse_unsubscribe_int() != OK)
    return EXIT_FAILURE;
  if (mouse_disable_data_reporting() != OK)
    return EXIT_FAILURE;
  if (ser_unsubscribe_int() != OK)
    return EXIT_FAILURE;
  if (kbc_unsubscribe_int() != OK)
    return EXIT_FAILURE;
  if (timer_unsubscribe_int() != OK)
    return EXIT_FAILURE;
  if (ser_exit() != OK)
    return EXIT_FAILURE;
  if (vg_exit() != OK)
    return EXIT_FAILURE;
  return 0;
}
