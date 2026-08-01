#include <lcom/i8254.h>
#include <lcom/lcom.h>
#include <lcom/vbe.h>

#include <stdint.h>

/*
 * Test-only guest for paired final-frame capture. It presents an empty frame,
 * writes one recognizable BGR pixel without presenting again, then waits until
 * the runtime's max-tick guard terminates it. A correct final dump synchronizes
 * shared framebuffer memory before cleanup and therefore contains RGB E14F9A.
 */
int main(void) {
  if (lcom_init() != LCOM_OK) return 1;

  lcom_vbe_mode_info_t info = {0};
  uint8_t *framebuffer = 0;
  lcom_irq_t timer_irq = {0};
  if (lcom_vbe_get_mode_info(LCOM_VBE_MODE_800_600_24, &info) != LCOM_OK ||
      lcom_vbe_set_mode(LCOM_VBE_MODE_800_600_24) != LCOM_OK ||
      lcom_phys_map(info.framebuffer_phys, info.framebuffer_size,
                    (void **)&framebuffer) != LCOM_OK ||
      lcom_irq_subscribe(TIMER0_IRQ, 0, &timer_irq) != LCOM_OK) {
    return 1;
  }

  if (lcom_vbe_present() != LCOM_OK) return 1;
  framebuffer[0] = 0x9Au;
  framebuffer[1] = 0x4Fu;
  framebuffer[2] = 0xE1u;

  for (;;) {
    lcom_event_t event = {0};
    if (lcom_event_wait(&event) != LCOM_OK) return 1;
  }
}
