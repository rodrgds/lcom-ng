#include <lcom/lcom.h>
#include <lcom/i8042.h>
#include <lcom/vbe.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  lcom_irq_t keyboard;
  if (lcom_init() || lcom_irq_subscribe(KBC_IRQ, 0, &keyboard)) return 1;
  /* Real console output, including a long line and literal markup. */
  for (unsigned i = 0; i < 5000; ++i) putchar('x');
  puts("\n<Continue> Press Space to open the game");
  for (;;) {
    lcom_event_t event;
    uint8_t status, byte;
    if (lcom_event_wait(&event)) return 2;
    if (!(event.irq_mask & keyboard.mask)) continue;
    if (lcom_port_read8(KBC_ST_REG, &status) || !(status & KBC_ST_OBF)) continue;
    if (lcom_port_read8(KBC_OUT_BUF, &byte)) return 3;
    if (!(status & KBC_ST_AUX) && byte == 0xad) exit(6); /* X release */
    if (!(status & KBC_ST_AUX) && byte == 0xb9) break;
  }
  lcom_vbe_mode_info_t mode;
  uint8_t *pixels;
  if (lcom_vbe_get_mode_info(LCOM_VBE_MODE_800_600_24, &mode) ||
      lcom_vbe_set_mode(mode.mode) ||
      lcom_phys_map(mode.framebuffer_phys, mode.framebuffer_size, (void **)&pixels)) return 4;
  memset(pixels, 0, mode.framebuffer_size);
  pixels[0] = 103; pixels[1] = 71; pixels[2] = 29;
  for (;;) {
    lcom_event_t event;
    if (lcom_vbe_present() || lcom_event_wait(&event)) return 5;
  }
}
