#include "timer.h"
static int hook;
int timer_subscribe_int(uint8_t *bit_no) {
    if (!bit_no) return 1;
    *bit_no = (uint8_t)hook;
    return sys_irqsetpolicy(0, IRQ_REENABLE, &hook);
}
int timer_unsubscribe_int(void) { return sys_irqrmpolicy(&hook); }
void timer_int_handler(void) {}
