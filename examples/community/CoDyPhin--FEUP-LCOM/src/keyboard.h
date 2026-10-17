#include <lcom/lcf.h>
//#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"

#ifdef LAB3
int count_sys_inb(int port, uint8_t *value);
#else
#define count_sys_inb(p,v) count_sys_inb(p,v)
#endif

/**@defgroup keyboard Keyboard
 * @{
 * Keyboard functions
 */

/**
 * @brief Subscribes the keyboard interrupts
 * @param bit_no Bit number where the IRQ is initialized
 * @return 1 if error, 0 if OK
 */
int (keyboard_subscribe_int)(uint8_t *bit_no);

/**
* @brief Unsubscribes the keyboard interrupts
* @return 1 if error, 0 if OK
*/
int (keyboard_unsubscribe_int)();

/**
 * @brief Get method for the current Scan Code
 * @return The current keyboard scan code
 */
uint8_t (getScanCode)();

/**
 * @brief Sys_inb overload to count the number of interrupts
 * @param port memory address where the value is written
 * @param value address to write the value
 * @return
 */
int (count_sys_inb)(int port, uint8_t *value);

/**
 * @brief Keyboard Interrupt Handler
 */
void (kbc_ih)();

int (scanCodeReader)(bool *make, uint8_t *size, uint8_t *bytes);
/** @} end of Keyboard */

