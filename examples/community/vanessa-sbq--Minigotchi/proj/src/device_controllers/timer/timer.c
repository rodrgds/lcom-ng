#include "timer.h"
#include "i8254.h"

/* Statics */

static int hook_id = TIMER0_IRQ;
static int timer_counter = 0;

/* Setters and getters */

int getTimerCounter() {
    return timer_counter;
}

void setTimerCounter(int cnt) {
    timer_counter = cnt;
}

/* Functions that deal with interrupts */

/**
 * @brief Subscribes and enables Timer 0 interrupts.
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise.
 */
int (timer_subscribe_int)(uint8_t *bit_no) {
    if (bit_no == NULL){
        panic("Error in timer_subscribe_int, bit_no null\n");
        return 1; 
    }
    *bit_no = hook_id;
    if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != 0){
        panic("Error in timer_subscribe_int, sys_irqsetpolicy error\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Unsubscribes Timer 0 interrupts.
 * @return Return 0 upon success and non-zero otherwise.
 */
int (timer_unsubscribe_int)() {
    if (sys_irqrmpolicy(&hook_id) != 0){
        panic("Error in timer_unsubscribe_int, sys_irqrmpolicy error\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Timer 0 interrupt handler.
 * @brief Increments counter.
 */
void (timer_int_handler)() {
  timer_counter++;
}

