#ifndef __KBD_H
#define __KBD_H

#define KBD_SYS_IN_ERROR 0xFFE //Error Reading Code
#define KBD_IN_CMD_ERROR 0xFFF //Error Reading Command Byte

#define IRQ_KBD_SET_ERROR -16
#define IRQ_KBD_ENAB_ERROR -17
#define IRQ_KBD_DISAB_ERROR -18
#define IRQ_KBD_REM_ERROR -19

#define KBD_SYS_OUT_ERROR 20
#define KBD_POLLING_ERROR 21
#define KBD_TEST_SCAN_ERROR 22
#define KBD_SUB_ERROR 23
#define KBD_UNSUB_ERROR 24
#define KBD_WRITE_CMD_BYTE_ERROR 25

unsigned long globalCode;

/**
 * @brief The Keys Used by the Game
 */

typedef enum {
	KEY_ESC = 0x01,
	KEY_ENTER = 0x1C,
	KEY_W = 0x11,
	KEY_A = 0x1E,
	KEY_S = 0x1F,
	KEY_D = 0x20

} KEYS_USED;

/**
 * @brief Subscribes Keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_subscribe_int(void);

/**
 * @brief Unsubscribes Keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_unsubscribe_int(void);

/**
 * @brief Read Code from Keyboard with interrupts
 *
 * @return Return code upon success and KBD_SYS_IN_ERROR otherwise
 */
unsigned long readCode();

/**
 * @brief Keyboard interrupt handler
 *
 * Increments counter
 */
void kbd_handler();

/**
 * @brief Write CommandByte and Arguments to the port
 *
 * @param port the port to be sent the code
 * @param cmd the cmd or the argument
 * @return Return 0 upon success and non-zero otherwise
 */
int WriteCommandByte(unsigned long port, unsigned long cmd);

/**
 * @brief Read current CommandByte
 *
 * @return Return the Command Byte
 */
unsigned long ReadCommandByte();

/**
 * @brief Wait for Esc to Be Released
 *
 * @return Return 0 upon success and non-zero otherwise
 */

int waitForEscRelease();

#endif
