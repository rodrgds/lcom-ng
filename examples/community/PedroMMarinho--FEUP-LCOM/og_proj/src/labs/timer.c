#include <lcom/lcf.h>

#include <stdint.h>
#include "timer.h"
#include "i8254.h"

static int elapsed = 0;
static int timer_hookId = 0;


int(timer_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    *bit_no = BIT(timer_hookId);
    return sys_irqsetpolicy(0, IRQ_REENABLE, &timer_hookId);
}

int(timer_unsubscribe_int)() {
    return sys_irqrmpolicy(&timer_hookId);
}

void(timer_int_handler)() {
    elapsed++;
}

int get_elapsed(){
  return elapsed;
}

void reset_elapsed(){
    elapsed = 0;
}

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {

    // Validate input
    if (timer > 2)
        return 1;
    // Validate frequency
    if (freq < TIMER_SMALLER_FREQ || freq > TIMER_FREQ)
        return 1;

    // Read status from timer
    uint8_t timerStatus;
    timer_get_conf(timer, &timerStatus);

    // Get timer related values
    uint8_t timerSelection = TIMER_SEL(timer);
    uint8_t timerPort = TIMER_PORT_SEL(timer);

    // Assemble Control Word
    uint8_t controlWord = (timerStatus & 0x0F) | timerSelection | TIMER_LSB_MSB;

    // Write Control Word to Control Register
    if (sys_outb(TIMER_CTRL, controlWord) != 0)
        return 1;

    // Calculate counter value
    uint16_t counterValue = TIMER_FREQ / freq;
    uint8_t counterLsb, counterMsb;
    util_get_LSB(counterValue, &counterLsb);
    util_get_MSB(counterValue, &counterMsb);
    // Write counter value to timer Port
    if (sys_outb(timerPort, counterLsb) != 0)
        return 1;
    if (sys_outb(timerPort, counterMsb) != 0)
        return 1;
    return 0;
}

int(timer_get_conf)(uint8_t timer, uint8_t *st) {

    // Validate data
    if (st == NULL)
        return 1;
    if (timer > 2)
        return 1;

    // Assemble Read-Back command
    uint8_t readBack = TIMER_RB | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

    // Write ReadBack command into Control Register
    if (sys_outb(TIMER_CTRL, readBack)) return 1;

    // Read from the timer Register
    if (util_sys_inb(TIMER_PORT_SEL(timer), st) != 0)
        return 1;

    return 0;
}


