#include <lcom/lcf.h>

/* Setters and getters */
int getTimerCounter();
void setTimerCounter(int cnt);

/* Functions that deal with interrupts */
int (timer_subscribe_int)(uint8_t *bit_no);
int (timer_unsubscribe_int)();

void (timer_int_handler)();
