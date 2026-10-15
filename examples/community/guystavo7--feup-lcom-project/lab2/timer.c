#include <lcom/lcf.h>

#include <stdint.h>

#include "i8254.h"
#include "timer.h"

static int counter = 0;
static int hookId = 0;

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {
    uint8_t st;
    if (timer_get_conf(timer, &st) != 0)
        return 1;
    uint8_t word = (st & 0x0F) | TIMER_LSB_MSB | (timer << 6);
    if (sys_outb(TIMER_CTRL, word) != 0)
        return 1;
    uint16_t divider = TIMER_FREQ / freq;
    uint8_t lsb, msb;
    if (util_get_LSB(divider, &lsb) != 0)
        return 1;
    if (util_get_MSB(divider, &msb) != 0)
        return 1;
    if (sys_outb(TIMER_0 + timer, lsb) != 0)
        return 1;
    if (sys_outb(TIMER_0 + timer, msb) != 0)
        return 1;

    return 0;
}

int timer_subscribe(uint8_t *bit_no) {
    *bit_no = hookId;
    if ((sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hookId) != 0)) return 1;
    return 0;
}

int(timer_unsubscribe_int)() {
    if (sys_irqrmpolicy(&hookId) != 0)
        return 1;
    return 0;
}

void(timer_int_handler)() { counter++; }

int(timer_get_conf)(uint8_t timer, uint8_t *st) {
    if (timer < 0 || timer > 2)
        return 1;

    uint8_t readBack = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

    if (sys_outb(TIMER_CTRL, readBack) != 0)
        return 1;

    if (util_sys_inb(TIMER_0 + timer, st) != 0)
        return 1;

    return 0;
}

int(timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
    union timer_status_field_val conf;

    switch (field) {
    case tsf_all:
        conf.byte = st;
        break;
    case tsf_base:
        conf.bcd = st & BIT(0);
        break;
    case tsf_initial:
        conf.in_mode = (st & (BIT(4) | BIT(5))) >> 4;
        break;
    case tsf_mode:
        conf.count_mode = ((st & (BIT(1) | BIT(2) | BIT(3))) >> 1);
        if (conf.count_mode > 5)
            conf.count_mode &= 0x03;
        break;
    }

    timer_print_config(timer, field, conf);
    return 0;
}

int timer_get_counter() { return counter; }
void timer_reset_counter() { counter = 0; }
