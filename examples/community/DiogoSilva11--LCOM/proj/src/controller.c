#include <lcom/lcf.h>

#include <stdint.h>

#include "controller.h"

// ----------------------------------------------------------------------------------------

int timer_irq_set = 0;
int kbd_irq_set = 0;
int mouse_irq_set = 0;

int (init)() {
    uint8_t timer_bit_no = 0;
    if (timer_subscribe_int(&timer_bit_no)) // subscribe Timer 0 interrupts
        return 1;
    timer_irq_set = BIT(timer_bit_no);

    uint8_t kbc_bit_no = 0;
    if (kbd_subscribe_int(&kbc_bit_no)) // subscribe keyboard interrupts
        return 1;
    kbd_irq_set = BIT(kbc_bit_no); 

    uint8_t mouse_bit_no = 0;
    if (mouse_subscribe_int(&mouse_bit_no)) // subscribe mouse interrupts
        return 1;
    mouse_irq_set = BIT(mouse_bit_no);

    if (mouse_enable())
        return 1;

    if (vg_init(GAME_MODE) == NULL)
        return 1;

    return 0;
}

int (leave)() {
    if (timer_unsubscribe_int()) {
        if (vg_exit())
            return 1;
        return 1;
    }

    if (kbd_unsubscribe_int()) {
        if (vg_exit())
            return 1;
        return 1;
    }

    if (mouse_disable()) {
        if (vg_exit())
            return 1;
        return 1;
    }

    if (mouse_unsubscribe_int()) {
        if (vg_exit())
            return 1;
        return 1;
    }

    if (vg_exit())
        return 1;

    return 0;
}

// ----------------------------------------------------------------------------------------
