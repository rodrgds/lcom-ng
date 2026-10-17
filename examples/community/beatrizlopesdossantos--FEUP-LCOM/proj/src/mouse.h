/**
 * @defgroup mouse
 * @{
 * @brief Functions of the Mouse
 */

/**
* Subscribes and enables Mouse 12 interrupts.
* @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
* @return Return 0 upon success and non-zero otherwise
*/
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
* Unsubscribes Mouse 12 interrupts.
* @return Return 0 upon success and non-zero otherwise
*/
int (mouse_unsubscribe_int)();

/**
* Writes the command WRITE_BYTE_MOUSE to the command register.
* @return Return 0 upon success and non-zero otherwise
*/
int write_cmd_kbd();

/**
* Enables stream mode data reporting, by sending the respective command to the mouse.
*
* This function is provided by the LCF, but it was adapted by us.
* @return 0 on success, and non-zero otherwise
*/
int mouse_enable_data_reporting2(uint32_t cmd2);

/**
* Handles mouse interrutps.
*
* Reads the status register and the output buffer (OB).
*
* If there was some error, the byte read from the OB should be discarded.
* @param IH's take no arguments
* @return IH's return no values
*/
void (mouse_ih)();

/**
* Builds the packed that indicates what the mousse has done.
*/
void packet_make();

/** @} end of mouse */
