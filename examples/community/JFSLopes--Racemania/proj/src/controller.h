#ifndef CONTROLLER_H
#define CONTROLLER_H

/**
 * @brief Handles interrupts from the timer, mouse, and keyboard.
 * 
 * @param timer_irq The IRQ line for the timer.
 * @param mouse_irq The IRQ line for the mouse.
 * @param kbd_irq The IRQ line for the keyboard.
 * @param rtc_irq The IRQ line for the rtc.
 */
void (controller) (int timer_irq, int mouse_irq, int kbd_irq, int rtc_irq);

#endif
