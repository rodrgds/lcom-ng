#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"
#include "i8254.h"

/**
 * @brief Subscribes and enables KBD's interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int kbd_unsubscribe_int();
/**
 * @brief Unsubscribes KBD's interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_subscribe_int(void);
/**
 * @brief reads the key that was pressed
 *
 * @param apt the key that was pressed
 *
 * @return 0 on success or 1 on non-success
 */
int kbd_scan_c(int *apt);
/**
 * @brief Read KBC responses from Output Buffer
 *
 * @param data Stores KBC Response (ACK, RESEND, ERROR)
 *
 * @return Return 0 upon success or non-zero otherwise
 */
unsigned short read_data_OUTBUF_from_KBC(unsigned long* data);
/**
 * @brief Issues mouse command to KBC
 *
 * @param command Stores mouse command
 *
 * @return Return 0 upon success and non-zero otherwise
 */
unsigned short KBC_issue_command_mouse(unsigned char command);
/**
 * @brief Issues argument of any command to KBC
 *
 * @param argument Stores KBC argument from any command
 *
 * @return Return 0 upon success and non-zero otherwise
 */
unsigned short issue_argument_KBC(unsigned char argument);

