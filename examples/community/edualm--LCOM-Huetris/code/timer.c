#include <minix/syslib.h>
#include <minix/drivers.h>

#include "i8254.h"
#include "timer.h"

#include "board.h"

#include "interrupt.h"

#include "rtc.h"

static int timer_counter;

void timer_int_handler() {
    timer_counter++;

    if (!(timer_counter % 60)) {
    	board_timerHandler();
    }
}

int timer_subscribe() {
    timer_counter = 0;
    
    return int_subscribe(TIMER0_IRQ, IRQ_REENABLE, timer_int_handler);
}

int timer_unsubscribe() {
    return int_unsubscribe(0);
}