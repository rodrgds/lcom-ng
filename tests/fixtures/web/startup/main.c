#include <lcom/lcom.h>
#include <lcom/vbe.h>
#include <stdio.h>
#include <string.h>

int main(void) {
  if (lcom_init()) return 1;
  puts("Preparing first frame");
  lcom_event_t event;
  if (lcom_delay_ticks(60, &event)) return 2;
  lcom_vbe_mode_info_t mode;
  uint8_t *pixels;
  if (lcom_vbe_get_mode_info(LCOM_VBE_MODE_800_600_24, &mode) ||
      lcom_vbe_set_mode(mode.mode) ||
      lcom_phys_map(mode.framebuffer_phys, mode.framebuffer_size, (void **)&pixels)) return 3;
  memset(pixels, 0, mode.framebuffer_size);
  pixels[0] = 51; pixels[1] = 34; pixels[2] = 17;
  for (;;) {
    pixels[3]++;
    if (lcom_vbe_present() || lcom_delay_ticks(6, &event)) return 4;
  }
}
