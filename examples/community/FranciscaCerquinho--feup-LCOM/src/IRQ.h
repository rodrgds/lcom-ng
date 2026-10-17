#ifndef __IRQ_H
#define __IRQ_H

/** @defgroup IRQ IRQ
 * @{
 * Functions related to the IRQ methods
 */

#include "timer.h"
#include "mouse.h"
#include "kbd.h"

typedef struct
{
	unsigned long irq_set_timer;
	unsigned long irq_set_mouse;
	unsigned long irq_set_kbd;
}IRQ;

/**
 * Subscribes the used peripherals' interruptions
 */
IRQ* Subscribe();
/**
 * Unsubscribes the used peripherals' interruptions
 */
void Unsubscribe();

/** @} */

#endif
