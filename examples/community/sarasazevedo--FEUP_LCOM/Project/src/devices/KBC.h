/**
 * @file KBC.h
 * 
 * @brief Functions for using the i8042 KBC
*/

#ifndef _KBC_H_
#define _KBC_H_

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"


/** @defgroup KBC KBC
 * @{
 *
 * @brief Functions for using the i8042 KBC
 */


/**
 * @brief Reads KBC's STAT_REG to get the status byte
 * 
 * @param stat Variable to store the read status
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbc_get_status)(uint8_t *stat);


/**
 * @brief Reads the output buffer register
 * 
 * @param op Variable to store the read output
 * @return Return 0 upon success and non-zero otherwise
 */
int (read_OBF)(uint8_t *op);



/**
 * @brief Checks for existing errors on the control register status
 * 
 * @param status The status to check for errors
 * @param from_mouse True if the data is coming from the mouse, false if is coming from the keyboard
 * @return Return 0 upon success and non-zero otherwise
 */
int (check_errors)(uint8_t status, bool from_mouse);





/**
 * @brief Reads OUT_BUF when full, verifies data validity and if it comes from the wanted device.
 * 
 * @param out 
 * @param from_mouse True if the data should be coming from the mouse, false if it should be coming from the keyboard
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbc_get_out)(uint8_t *out, bool from_mouse);


/**
 * @brief Writes to KBC's input buffers (port 0x64 or 0x60) when not full
 * 
 * @param port The port to write to
 * @param commandOrArg  The command or argument
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbc_write_input)(int port, uint8_t commandOrArg);




/**
 * @brief Sends KBC command to write Command Byte and writes one
 * 
 * @param commandByte The command byte to write
*/
int (kbc_configure)(uint8_t commandByte);




/**
 * @brief Sends KBC command to read Command Byte and reads it from OUT_BUF
 * 
 * @param commandByte Variable to store the read command byte
*/
int (kbc_get_conf)(uint8_t *commandByte);

/** @} end of KBC */


#endif /* __KBC_H */
