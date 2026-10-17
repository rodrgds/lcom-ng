#include "timer.h"

int timer_counter = 0;
int timer_hook_id = 0;

int subscribe_timer(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;

    *bit_no = BIT(timer_hook_id);

    if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != 0) return 1;
    
    return 0;
}

int unsubscribe_timer() {
    if (sys_irqrmpolicy(&timer_hook_id) != 0) return 1;
    return 0;
}

void (timer_ih)() {
    timer_counter++;
}
