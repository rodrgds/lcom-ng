#include "timer.h"

static lcom_irq_t timer_irq;
static uint32_t timer_counter;
static bool timer_subscribed;

int timer_set_frequency(uint8_t timer, uint32_t freq) {
    if (timer != 0 || freq == 0 || freq > TIMER_FREQ) return 1;
    uint32_t divisor = TIMER_FREQ / freq;
    if (divisor == 0 || divisor > 65536u) return 1;
    if (divisor == 65536u) divisor = 0;
    if (lcom_port_write8(TIMER_CTRL, TIMER_SEL0 | TIMER_LSB_MSB | TIMER_SQR_WAVE) != LCOM_OK) return 1;
    if (lcom_port_write8(TIMER_0, (uint8_t)(divisor & 0xFFu)) != LCOM_OK) return 1;
    if (lcom_port_write8(TIMER_0, (uint8_t)(divisor >> 8)) != LCOM_OK) return 1;
    return 0;
}

int timer_subscribe_int(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    if (lcom_irq_subscribe(TIMER0_IRQ, 0, &timer_irq) != LCOM_OK) return 1;
    timer_subscribed = true;
    *bit_no = timer_irq.bit_no;
    return 0;
}

int timer_unsubscribe_int(void) {
    if (!timer_subscribed) return 0;
    timer_subscribed = false;
    return lcom_irq_unsubscribe(&timer_irq) == LCOM_OK ? 0 : 1;
}

void timer_int_handler(void) {
    timer_counter++;
}

uint32_t get_timer_counter(void) {
    return timer_counter;
}

void reset_timer_counter(void) {
    timer_counter = 0;
}
