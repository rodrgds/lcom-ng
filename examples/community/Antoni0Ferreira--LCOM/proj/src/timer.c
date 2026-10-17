#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"


int (timer_set_frequency)(uint8_t timer, uint32_t freq) {


    if(timer < 0 || timer > 2) return 1;

    uint8_t status;
    if(timer_get_conf(timer,&status) != 0) return 1;

    uint8_t aux = status & 0x0F;
    uint8_t timerSel = timer << 6;
    uint8_t word = timerSel | aux | MSB_after_LSB;

    sys_outb(TIMER_CTRL, word);

    uint8_t divLSB = TIMER_FREQ / freq;
    uint8_t divMSB = (TIMER_FREQ / freq) >> 8;
    sys_outb(TIMER_0 + timer, divLSB);
    sys_outb(TIMER_0 + timer, divMSB);
    return 0;

}

int hook_timer_id = 0x00;
int counter = 0;

int (timer_subscribe_int)(uint8_t *bit_no) {


    *bit_no = hook_timer_id;
    if(sys_irqsetpolicy(0,IRQ_REENABLE,&hook_timer_id) != 0) return 1;
    return 0;

}

int (timer_unsubscribe_int)() {


    if(sys_irqrmpolicy(&hook_timer_id) != 0) return 1;
    return 0;
}

void (timer_int_handler)() {
    counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

    if(timer < 0 || timer > 2) return 1;
    uint8_t timerSel = timer + 0x40;
    uint8_t word = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
    sys_outb(TIMER_CTRL,word);

    util_sys_inb(timerSel,st);
    return 0;

    
}


