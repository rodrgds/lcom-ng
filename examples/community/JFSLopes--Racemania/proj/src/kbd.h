#ifndef KDB_LAB_5
#define KDB_LAB_5

#include <lcom/lcf.h>

/**
 * @brief Subscribes the Keyboard interrupts
 * 
 * @param bit_no Bit number that will be activated when I interrupt is generated
 * 
 * @return 0 on success, -1 otherwise
*/
int (keyboard_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes the Keyboard interrupts
 * 
 * @return OK on success, ERROR otherwise
*/
int (keyboard_unsubscribe_int)();

/**
 * @brief Reads the scancodes
 * 
 * @return ERROR if an error occured, NO_DATA_AVAIVABLE if the OBF wasn't full or OK if everything was ok.
*/
int (read_scancode) ();

/**
 * @brief Reads the OBF
 * 
 * This function reads the OBF and makes sure that the data comes from the right device (keyboard or mouse)
 * 
 * @param byte Stores the value on the OBF. If the value read was invalid, then byte will be 0.
 * @param from_mouse True if the value should be from the mouse or False if should be from the keyboard
 * 
 * @return OK on success, ERROR otherwise
*/
int (read_OBF) (uint8_t* byte, bool from_mouse);

/**
 * @brief Deals with the keyboard interrupts
*/
void (kbc_ih)();

/**
 * @brief Writes a command in the desired address
 * 
 * @param address Address where the command should be written
 * @param command Command intended to be written
 * 
 * @return OK on success, ERROR otherwise
*/
int (write_command) (uint8_t address, uint8_t command);

/**
 * @brief Enables the interrupts of the keyboard
 * 
 * @return OK on success, ERROR otherwise
*/
int (enable_interrupts) (void);

/**
 * @brief Resets the scancodes.
 */
void (reset_scancodes) ();

#endif
