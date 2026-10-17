#include "keyboard.h"

static int kbd_hook_id = KBD_IRQ;

static uint8_t scanCode = 0x00;

/**
 * @brief Subscribes and enables KBC interrupts.
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise.
 */
int subscribe_interrupts_kbd(uint8_t* bit_no){
    if (bit_no == NULL) return 1;
    *bit_no = kbd_hook_id;
    if (sys_irqsetpolicy(KBD_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &kbd_hook_id) != 0){
        return 1;
    }
    return 0;
}

/**
 * @brief Keyboard interrupt handler, reads the scancode from the output buffer upon an interrupt. 
 */
void (kbc_ih)(){
    readFromOutputBuffer(&scanCode); // Fetch the scancode.
}

/**
 * @brief Getter for scancode 
 */
uint8_t getScanCode() {
    return scanCode;
}

/**
 * @brief Unsubscribes KBC interrupts.
 * @return Return 0 upon success and non-zero otherwise.
 */
int unsubscribe_interrupts_kbd(){
    if (sys_irqrmpolicy(&kbd_hook_id) != 0){
        return 1;
    }
    return 0;
}
