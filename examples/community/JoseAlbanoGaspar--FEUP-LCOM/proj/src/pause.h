#include <lcom/lcf.h>
#include "macros/i8254.h"
#include "macros/i8042.h"
#include "keyboard.h"

/**
 * @brief freezes the screen if the user presses 'p' key
 * @param irq_set_keyboard irq line of keyboard
 * @param irq_set_mouse irq line of mouse
 * @return Returns 0 upon success and non-zero otherwise
 */
int pause_loop(uint32_t irq_set_keyboard,uint32_t irq_set_mouse);
