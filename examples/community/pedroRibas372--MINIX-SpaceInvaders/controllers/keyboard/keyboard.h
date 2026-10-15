#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_


#include <lcom/lcf.h>
#include <lcom/utils.h>

#include <stdint.h>
#include "i8042.h"

#define DELAY 20000

#define A_MAKECODE 0x1E
#define D_MAKECODE 0x20

#define ESCAPE_MAKECODE 0x01
#define ESCAPE 0X81


#define ENTER_MAKECODE 0x1C
#define SPACE_MAKECODE 0x39

//Arrow Keys
#define SCANCODE_UP 0x48
#define SCANCODE_DOWN 0x50
#define TWO_BYTES 0xE0


/**
 * @brief Subscribes and enables keyboard interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbd_subscribe_int)(uint8_t *bit_no);


/**
 * @brief Unsubscribes keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbd_unsubscribe_int)();


/**
 * @brief Handles keyboard interrupts
 * 
 */
void (kbc_ih)();



/**
 * @brief Reads keyboard output buffer
 * 
 * Used for interrupt handling.
 *
 * @param data Keyboard output information
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (readByte)(uint8_t * data);




/**
 * @brief Enables keyboard functionality by interrupts
 * 
 * Logic:
 * 
 * Subscribe Interrupts -> Read Output Buffer -> Unsubscribe interrupts
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int keyboard_scan();
#endif
