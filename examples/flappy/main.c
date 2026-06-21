#include "app.h"
#include "sound.h"

#include <lcom/i8042.h>
#include <lcom/i8254.h>
#include <lcom/lcom.h>
#include <lcom/vbe.h>

#include <stdint.h>

enum {
  INPUT_BYTES_PER_EVENT = 24
};

static void apply_input_result(app_t *app, app_input_result_t result) {
  if (result == APP_INPUT_FLAP) {
    sound_flap();
  } else if (result == APP_INPUT_QUIT) {
    app->running = 0;
  }
}

static int handle_i8042_byte(app_t *app) {
  uint8_t status = 0;
  uint8_t data = 0;
  if (lcom_port_read8(KBC_ST_REG, &status) != LCOM_OK) return 0;
  if ((status & KBC_ST_OBF) == 0) return 0;
  if (lcom_port_read8(KBC_OUT_BUF, &data) != LCOM_OK) return 0;
  if ((status & KBC_ST_ERR) != 0) return 1;

  if ((status & KBC_ST_AUX) == 0) {
    apply_input_result(app, app_key(app, data));
  }
  return 1;
}

int main(void) {
  if (lcom_init() != LCOM_OK) return 1;
  lcom_printf("LCOM Bird: SPACE/UP flaps, ESC exits.\n");

  lcom_vbe_mode_info_t info;
  if (lcom_vbe_get_mode_info(LCOM_VBE_MODE_800_600_24, &info) != LCOM_OK) return 1;
  if (lcom_vbe_set_mode(LCOM_VBE_MODE_800_600_24) != LCOM_OK) return 1;

  uint8_t *fb = 0;
  if (lcom_phys_map(info.framebuffer_phys, info.framebuffer_size, (void **)&fb) != LCOM_OK) return 1;
  draw_context_t ctx = {fb, info};

  sound_init();

  lcom_irq_t timer_irq;
  lcom_irq_t kbd_irq;
  if (lcom_irq_subscribe(TIMER0_IRQ, 0, &timer_irq) != LCOM_OK) return 1;
  if (lcom_irq_subscribe(KBC_IRQ, 0, &kbd_irq) != LCOM_OK) return 1;

  app_t app;
  app_init(&app);

  while (app.running) {
    lcom_event_t ev;
    if (lcom_event_wait(&ev) != LCOM_OK) break;

    if (ev.irq_mask & kbd_irq.mask) {
      for (int i = 0; app.running && i < INPUT_BYTES_PER_EVENT; i++) {
        if (!handle_i8042_byte(&app)) break;
      }
    }

    if (app.running && (ev.irq_mask & timer_irq.mask)) {
      app_update(&app);
      app_render(&app, &ctx);
      lcom_vbe_present();
    }
  }

  lcom_irq_unsubscribe(&kbd_irq);
  lcom_irq_unsubscribe(&timer_irq);
  lcom_phys_unmap(fb, info.framebuffer_size);
  lcom_exit();
  return 0;
}
