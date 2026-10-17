#include "keyboard.h"
#include "i8042.h"
#include <lcom/lcf.h>
//#include "utils.c"
//#include "kb_controller.h"

uint8_t scan_code = 0;

uint32_t counter_kb_controller = 0;

int keyboard_hook_id = 1;

/**
 * @brief Subscribes keyboard interrupts.
 *
 * Sets up the interrupt policy for the keyboard and enables exclusive access to IRQ1.
 *
 * @param bit_no Pointer to a variable to store the bit mask of the hook ID.
 * @return 0 on success, non-zero on failure.
 */
int (keyboard_subscribe_interruptions)(uint8_t *bit_no) 
{
    if (bit_no == NULL)
    {
        return 1;
    }

    *bit_no = BIT(keyboard_hook_id);
    
    return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id);
}

/**
 * @brief Unsubscribes keyboard interrupts.
 *
 * Removes the interrupt policy previously set for the keyboard.
 *
 * @return 0 on success, non-zero on failure.
 */
int (keyboard_unsubscribe_interruptions)() 
{
    return sys_irqrmpolicy(&keyboard_hook_id);
}

/**
 * @brief Keyboard interrupt handler.
 *
 * Reads the scan code from the keyboard output buffer and stores it in a global variable.
 */
void (kbc_ih)() 
{

    if (readKBControlleroutput(KBC_OUT_COMMAND, &scan_code, 0) != 0)
    {
        printf("Error reading scan_code.\n");

    }
}

/**
 * @brief Restores the keyboard to its default state.
 *
 * Reads the current command byte, enables interrupts (bit 0), and writes it back.
 *
 * @return 0 on success, non-zero on failure.
 */
int (keyboard_restore)() 
{
    uint8_t cmd_byte;

    if ((writeKBControllercommand(KBC_IN_COMMAND, KBC_READ_COMMAND) != 0) || (readKBControlleroutput(KBC_OUT_COMMAND, &cmd_byte, 0) != 0))
    {

        return 1;

    }

    cmd_byte |= EN_INT;  //operador bitwise OR. EN_INT é a bitmask para ativar o BIT(0)

    if (writeKBControllercommand(KBC_IN_COMMAND, KBC_WRITE_COMMAND) != 0 || writeKBControllercommand(KBC_WRITE_COMMAND, cmd_byte) != 0)
    {

        return 1;  
          
    }


    return 0;

}


/**
 * @brief Reads from a port and increments a counter.
 *
 * Wrapper around util_sys_inb that increments a global counter each time it is called.
 *
 * @param port The port to read from.
 * @param value Pointer to store the read value.
 * @return 0 on success, non-zero on failure.
 */
int util_sys_inb_counter(int port, uint8_t *value) 
{
    counter_kb_controller = counter_kb_controller + 1; 
    
    return util_sys_inb(port, value);
}

