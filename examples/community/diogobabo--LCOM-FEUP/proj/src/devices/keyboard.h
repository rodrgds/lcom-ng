#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"
/**
 * @brief checks keyboard status for errors
 * 
 */
void (kbc_ih)	();
/**
 * @brief subscribes to keyboard interrupts 
 * 
 */
int (kb_subscribe)(uint8_t *bit_no);
/**
 * @brief unsubscribes to keyboard interrupts 
 * 
 */
int (kb_unsubscribe)();
/**
 * @brief restores the interrupts after polling
 * 
 */
void (enable_interrupts)();
