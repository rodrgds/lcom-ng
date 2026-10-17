#include <stdbool.h>
#include <stdint.h>


/**
 * @brief Subscribes keyboard interrupts
 * 
 */
int(kdb_subscribe_int)(uint8_t *bit_no);


/**
 * @brief Unsubscribes keyboard interrupts
 * 
 */
int(kdb_unsubscribe_int)();


/**
 * @brief Keyboard interrupt handler that knows whether the makecode is of 1 or 2 bytes and stores them in an array while also changing a bool variable so that we can know when we have received the complete make code
 * 
 */
void(kbc_ih)();
