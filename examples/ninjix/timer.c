#include "timer.h"

static lcom_irq_t g_timer_irq;
static int g_timer_subscribed = 0;
static uint32_t g_timer_counter = 0;

int timer_set_frequency(uint8_t timer, uint32_t freq) {
  if (timer != 0 || freq == 0) return 1;
  uint32_t divisor = TIMER_FREQ / freq;
  if (divisor == 0) divisor = 1;
  if (divisor > 65536u) divisor = 65536u;
  uint16_t encoded = (uint16_t)(divisor == 65536u ? 0 : divisor);
  if (lcom_port_write8(TIMER_CTRL, TIMER_SEL0 | TIMER_LSB_MSB | TIMER_SQR_WAVE) != LCOM_OK) return 1;
  if (lcom_port_write8(TIMER_0, (uint8_t)(encoded & 0xFFu)) != LCOM_OK) return 1;
  return lcom_port_write8(TIMER_0, (uint8_t)(encoded >> 8)) == LCOM_OK ? 0 : 1;
}

int timer_subscribe_int(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  if (lcom_irq_subscribe(TIMER0_IRQ, 0, &g_timer_irq) != LCOM_OK) return 1;
  *bit_no = g_timer_irq.bit_no;
  g_timer_subscribed = 1;
  return 0;
}

int timer_unsubscribe_int(void) {
  if (!g_timer_subscribed) return 0;
  g_timer_subscribed = 0;
  return lcom_irq_unsubscribe(&g_timer_irq) == LCOM_OK ? 0 : 1;
}

void timer_int_handler(void) {
  g_timer_counter++;
}

int timer_get_conf(uint8_t timer, uint8_t *st) {
  if (timer != 0 || st == NULL) return 1;
  return lcom_port_read8(TIMER_0, st) == LCOM_OK ? 0 : 1;
}

uint32_t timer_get_counter(void) {
  return g_timer_counter;
}

void timer_set_counter(uint32_t value) {
  g_timer_counter = value;
}

int timer_calc_ticks_per_event(uint32_t timer_hz, uint32_t event_hz, uint32_t *ticks) {
  if (timer_hz == 0 || event_hz == 0 || ticks == NULL || event_hz > timer_hz) return 1;
  uint32_t value = timer_hz / event_hz;
  if (value == 0) value = 1;
  *ticks = value;
  return 0;
}
