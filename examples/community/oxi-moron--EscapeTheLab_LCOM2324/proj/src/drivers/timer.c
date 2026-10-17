#include <lcom/lcf.h>
#include "timer.h"
#include "i8254_timer.h"

int timer_hook_id;
uint32_t counter = 0;

int (timer_get_frequency)(uint32_t* freq) {
    *freq = sys_hz();
    return 0;
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
    uint8_t cw;

    if (timer_get_conf(timer, &cw) != 0) {
        printf("ERROR: timer_get_conf\n");
        return 1;
    }

    cw &= 0x0F;

    switch (timer) {
        case 0:
            cw |= CW_TIMER0;
            break;
        case 1:
            cw |= CW_TIMER1;
            break;
        case 2:
            cw |= CW_TIMER2;
            break;
    }

    cw |= CW_LSB_MSB;

    if (timer_write_cmd(cw) != 0) {
        printf("ERROR: timer_write_cmd\n");
        return 1;
    }

    if (freq < 20 || freq > TIMER_FREQ) return 1;

    uint16_t div = TIMER_FREQ / freq;
    uint8_t lsb;
    uint8_t msb;

    if (util_get_LSB(div, &lsb) != 0) {
        printf("ERROR: util_get_LSB\n");
        return 1;
    }

    if (util_get_MSB(div, &msb) != 0) {
        printf("ERROR: util_get_msb\n");
        return 1;
    }

    if (sys_outb(TIMER_0 + timer, lsb) != 0) {
        printf("ERROR: sys_outb\n");
        return 1;
    }

    if (sys_outb(TIMER_0 + timer, msb) != 0) {
        printf("ERROR: sys_outb\n");
        return 1;
    }

    return 0;
}

int (timer_subscribe_int)(uint8_t* bit_no) {
    timer_hook_id = *bit_no;
    if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }
    return 0;
}

int (timer_unsubscribe_int)() {
    if(sys_irqrmpolicy(&timer_hook_id) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }
    return 0;
}

void (timer_ih) () {
    counter++;
}

int timer_write_cmd(uint8_t cmd) {
    if (sys_outb(TIMER_CTRL, cmd) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }
    return 0;
}

int timer_read_status(uint8_t* st, uint8_t timer) {
    if (util_sys_inb(TIMER_0 + timer, st) != 0) {
        printf("ERROR: %s\n", __func__ );
        return 1;
    }
    return 0;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

    uint8_t cw = (READ_BACK | RB_TIMER(timer) | RB_NO_COUNT);

    if (sys_outb(TIMER_CTRL, cw) != 0) {
        printf("ERROR: sys_outb\n");
        return 1;
    }

    if (util_sys_inb(TIMER_0 + timer, st) != 0) {
        printf("ERROR: sys_inb\n");
        return 1;
    }

    return 0;
}
