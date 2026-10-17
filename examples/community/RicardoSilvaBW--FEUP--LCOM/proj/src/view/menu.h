/* Simple menu for level picker and settings */
#ifndef MENU_H
#define MENU_H

#include <lcom/lcf.h>

/* Run the menu. Blocks until user starts the game or exits.
 * selected_speed is filled with the chosen note speed (difficulty).
 * Returns 0 on start, 1 on request to quit/exit.
 */
int menu_run(uint8_t kb_irq_set, uint8_t mouse_irq_set, uint8_t timer_irq_set);

#endif /* MENU_H */
