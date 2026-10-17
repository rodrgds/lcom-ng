#ifndef __KBD_H
#define __KBD_H

#include "i8042.h"

/*SCAN CODES */
/*#define ESC_CODE 0x81
#define KEY_W 0x0011
#define KEY_A 0x001E
#define KEY_S 0x001F
#define KEY_D 0x0020*/

//extern unsigned long scanCode;
//unsigned long scanCodeAss;
//Kbd_asm_read();


/**
 * @brief Subscribes and enables Keyboard  interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int Kbd_subscribe_int(void);

/**
 * @brief Unsubscribes Keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int Kbd_unsubscribe_int();

/**
 * @brief Keyboard interrupt handler
 *
 */
void Kbd_int_handler(unsigned int bitmask);

/**
 * @brief Read scancode
 *
 *@return return scancode upon sucess and negative value otherwise
 */
int Kbd_read();

/**
 * @brief Write argument to Kbd
 *
 *@return 0 upon success, negative value otherwise
 */
int Kbd_write(unsigned int args);

/**
 * @brief Reads and prints scan code
 *
 *@return 0 upon success, negative value otherwise
 */
int Kbd_process();

/**
 * @brief Use assembly code to read data
 */
int Kbd_write(unsigned int args);

/**
 * @brief returns the last scan code read
 * @return las key code read, 0 if nothing has been read yet
 */
unsigned long Kbd_getLastScanCodeRead();


#endif /* __KBD_H */
