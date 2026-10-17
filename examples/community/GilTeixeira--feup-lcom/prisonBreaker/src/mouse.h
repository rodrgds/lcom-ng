#ifndef __MOUSE_H
#define __MOUSE_H

#define IRQ_MOUSE_SET_ERROR 26
#define IRQ_MOUSE_ENAB_ERROR 27
#define IRQ_MOUSE_DISAB_ERROR 28
#define IRQ_MOUSE_REM_ERROR 29
#define MOUSE_WRITE_CMD_ERROR 30
#define MOUSE_SYS_OUT_ERROR 31
#define MOUSE_SUB_ERROR 32
#define MOUSE_UNSUB_ERROR 33
#define CLEAN_OUTBUFF_ERROR 34
#define ENAB_STREAM_MODE_ERROR 35

/**
 * @struct Mouse
 * @brief This structure holds the information about Mouse Movement
 */
typedef struct {
	long deltaX;
	long deltaY;
} Mouse;

/**
 * @brief Initializes the Mouse
 *
 * @return The new Mouse
 */
Mouse* initMouse();

/**
 * @brief Processes an interrupt
 *
 * @param mouse The Mouse to be handled
 */
void mouseHandler(Mouse* mouse);

/**
 * @brief Free the Mouse and all the resources used by it
 *
 * @param mouse The Mouse to be freed
 */
void stopMouse(Mouse* mouse);

/**
 * @brief Reset The Mouse Settings
 *
 * @param mouse The Mouse to be reset
 */
void resetMouse(Mouse* mouse);

/**
 * @brief Gets the Mouse Y movement
 *
 * @return The Mouse Y movement
 */
long getDeltaY();

/**
 * @brief Gets the Mouse X movement
 *
 * @return The Mouse X movement
 */
long getDeltaX();

/**
 * @brief Subscribes Mouse interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_subscribe_int(void);

/**
 * @brief Unsubscribes Mouse interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_unsubscribe_int();

/**
 * @brief Read a Packets Byte
 *
 * @return Return the Byte read
 */
unsigned long readByte();

/**
 * @brief Mouse interrupt handler
 */
void mouse_handler();

/**
 * @brief Subscribes Mouse interrupts in exclusive mode only
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_subscribe_Exc_int(void);

/**
 * @brief Shows Packet Read
 *
 * Displays in a human friendly way the packets
 */
int print_packets();

/**
 * @brief Enables Mouses Stream Mode
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int enable_stream_mode();

/**
 * @brief Write a Mouse CommandByte
 *
 * @param cmd the cmd
 * @return Return 0 upon success and non-zero otherwise
 */
int mouseWriteCommandByte(unsigned long cmd);

/**
 * @brief Read mouse Output
 *
 * @return Return the mouse Output
 */
unsigned long mouseReadOutput();

/**
 * @brief Converts number in Twos complement to signed number representation
 *
 * @param number the number without its most significant bit
 * @param msb the most significant bit
 *
 * @return Return the number in signed number representation
 */
long convertNumber(char number, unsigned long msb);

/**
 * @brief Compares 2 numbers and check if they have the same signal
 *
 * @param number1
 * @param number2
 *
 * @return Return 1 if true, 0 if false
 */
short hasSameSignal(long number1, long number2);

#endif

