#include <lcom/lcom.h>
#include <lcom/i8042.h>
#include <lcom/i8254.h>
#include <lcom/vbe.h>
#include <lcom/ac97.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  if (lcom_init()) return 1;
  FILE *file = fopen("assets/message.txt", "r");
  char text[32] = {0};
  if (!file || !fgets(text, sizeof(text), file)) return 2;
  fclose(file);
  printf("asset:%s", text);
  void *invalid = NULL;
  if (lcom_phys_map(0xffffffffu, 16, &invalid) != LCOM_ERR) return 3;
  lcom_vbe_mode_info_t mode;
  uint8_t *pixels;
  if (lcom_vbe_get_mode_info(LCOM_VBE_MODE_800_600_24, &mode) ||
      lcom_vbe_set_mode(mode.mode) ||
      lcom_phys_map(mode.framebuffer_phys, mode.framebuffer_size, (void **)&pixels)) return 4;
  for (size_t i = 0; i < mode.framebuffer_size; i += 3) {
    pixels[i] = 51; pixels[i + 1] = 34; pixels[i + 2] = 17;
  }
  lcom_vbe_present();
  lcom_irq_t timer, keyboard, mouse;
  if (lcom_irq_subscribe(TIMER0_IRQ, 0, &timer) ||
      lcom_irq_subscribe(KBC_IRQ, 0, &keyboard) ||
      lcom_irq_subscribe(MOUSE_IRQ, 0, &mouse)) return 5;
  lcom_port_write8(KBC_CMD_REG, KBC_CMD_WRITE_MOUSE);
  lcom_port_write8(KBC_IN_BUF, 0xf4);
  lcom_ac97_buffer_info_t sound;
  int16_t *samples;
  if (lcom_ac97_get_buffer(&sound) || lcom_phys_map(sound.pcm_phys, sound.pcm_bytes, (void **)&samples)) return 6;
  for (int i = 0; i < 256; ++i) samples[i] = i % 2 ? -4096 : 4096;
  if (getenv("LCOM_COLLECTION_TEST")) printf("environment:%s\n", getenv("LCOM_COLLECTION_TEST"));
  puts("ready");
  int timer_reported = 0;
  for (;;) {
    lcom_event_t event;
    if (lcom_event_wait(&event)) return 7;
    if (!timer_reported && event.tick >= 60) { puts("tick60"); timer_reported = 1; }
    for (int i = 0; i < 32; ++i) {
      uint8_t status, byte;
      if (lcom_port_read8(KBC_ST_REG, &status) || !(status & KBC_ST_OBF)) break;
      if (lcom_port_read8(KBC_OUT_BUF, &byte)) return 8;
      printf("%s:%02x\n", status & KBC_ST_AUX ? "mouse" : "key", byte);
      if (!(status & KBC_ST_AUX) && byte == 0x2d) {
        lcom_exit(); exit(9);
      }
      if (!(status & KBC_ST_AUX) && byte == 0x1e) {
        pixels[0] = 0; pixels[1] = 255; pixels[2] = 0;
        lcom_vbe_present();
        lcom_port_write16(AC97_BM_BASE + AC97_PO_PICB, 512);
        lcom_port_write8(AC97_BM_BASE + AC97_PO_CR, AC97_PO_CR_RUN);
      }
      if (!(status & KBC_ST_AUX) && byte == 1) {
        if (argc > 1 && !strcmp(argv[1], "--keep-open")) {
          pixels[0] = 255; pixels[1] = 0; pixels[2] = 0;
          lcom_vbe_present();
        } else { lcom_exit(); return 0; }
      }
    }
  }
}
