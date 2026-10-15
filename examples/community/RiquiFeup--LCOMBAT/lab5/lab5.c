// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/lab5.h>
#include <stdint.h>
#include <stdio.h>

// Local includes
#include "graphics.h"
#include "kbc.h"
#include "i8042.h"
#include "vbe.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  lcf_cleanup();

  return 0;
}

int (video_test_init)(uint16_t mode, uint8_t delay) {
  // 1. Switch to the selected graphics mode.
  if (set_graphic_mode(mode) != 0) return 1;

  // 2. Wait for the requested number of seconds.
  sleep(delay);

  // 3. Return control to Minix and restore text mode.
  if (vg_exit() != 0) return 1;

  return 0;
}

int (video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  // 1. Map VRAM first.
  if (map_vram(mode) != 0) return 1;

  // 2. Switch to graphics mode.
  if (set_graphic_mode(mode) != 0) return 1;

  // 3. Draw the rectangle.
  vg_draw_rectangle(x, y, width, height, color);

  // 4. Subscribe keyboard interrupts and wait for ESC.
  uint8_t kbd_bit_no;
  if (kbd_subscribe_int(&kbd_bit_no) != 0) return 1;
  uint32_t irq_kbd = BIT(kbd_bit_no);

  int r, ipc_status;
  message msg;

  while (kbc_get_scancode() != ESC_BREAKCODE) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) continue;

    if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {
      if (msg.m_notify.interrupts & irq_kbd) {
        kbc_ih();
      }
    }
  }

  kbd_unsubscribe_int();
  vg_exit(); // Return to text mode.

  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  // 1. Map VRAM first.
  if (map_vram(MODE_105) != 0) return 1;

  // 2. Switch to graphics mode.
  if (set_graphic_mode(MODE_105) != 0) return 1;

  if (vg_draw_xpm(xpm, x, y) != 0) return 1;

  // 4. Subscribe keyboard interrupts and wait for ESC.
  uint8_t kbd_bit_no;
  if (kbd_subscribe_int(&kbd_bit_no) != 0) return 1;
  uint32_t irq_kbd = BIT(kbd_bit_no);

  int r, ipc_status;
  message msg;

  while (kbc_get_scancode() != ESC_BREAKCODE) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) continue;

    if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {
      if (msg.m_notify.interrupts & irq_kbd) {
        kbc_ih();
      }
    }
  }

  kbd_unsubscribe_int();
  vg_exit(); // Return to text mode.

  return 0;
}
