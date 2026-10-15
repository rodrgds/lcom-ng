#include <lcom/lcf.h>
#include <stdint.h>

#include "i8254.h"
#include "timer.h"

int timer_hook_id = 0;
int counter=0;

int(timer_set_frequency)(uint8_t timer, uint32_t freq){

    if (!(timer == 0 || timer == 1 || timer == 2)) return 1;

    if (freq < 19 || freq > TIMER_FREQ) return 1;

    uint16_t counter = TIMER_FREQ / freq;

    uint8_t LSB, MSB;

    if (util_get_LSB(counter, &LSB) ) return 1;
    if (util_get_MSB(counter, &MSB) ) return 1;

    uint8_t config;
    if (timer_get_conf(timer, &config) ) return 1;

    config &= 0xF;

    config |= TIMER_LSB_MSB;

    switch (timer){
        case 0:
            config |= TIMER_SEL0;
            break;
        case 1:
            config |= TIMER_SEL1;
            break;
        case 2:
            config |= TIMER_SEL2;
            break;
    }

    if (sys_outb(TIMER_CTRL, config) ) return 1;

    switch (timer){
        case 0:{
            if (sys_outb(TIMER_0, LSB) ) return 1;
            if (sys_outb(TIMER_0, MSB) ) return 1;
            break;
        }
        case 1: {
            if (sys_outb(TIMER_1, LSB) ) return 1;
            if (sys_outb(TIMER_1, MSB) ) return 1;
            break;
        }
        case 2: {
            if (sys_outb(TIMER_2, LSB) ) return 1;
            if (sys_outb(TIMER_2, MSB) ) return 1;
            break;
        }
    }

    return 0;
}

int(timer_subscribe_interrupt)(uint32_t *timer_irq) {

    *timer_irq = BIT(timer_hook_id);

    if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) ) return 1;

    return 0;
}

int(timer_unsubscribe_int)(){
    return sys_irqrmpolicy(&timer_hook_id);
}

int(timer_get_conf)(uint8_t timer, uint8_t *status) {

    if (!(timer == 0 || timer == 1 || timer == 2)) return 1;

    if (status == NULL) return 1;

    uint8_t command = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

    if (sys_outb(TIMER_CTRL, command)) return 1;

    return util_sys_inb(0x40 + timer, status);
}

void (timer_int_handler)(){
    counter++;
}
