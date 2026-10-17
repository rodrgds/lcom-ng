
#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8042
 *  @{
 *
 *  Constants for programming the i8042 Timer.
 */

/** @brief Get bit in n position. */
#define BIT(n) 			(0x01<<(n))

/** @brief Keyboard's IRQ */
#define KBD_IRQ			1

/** @brief Mouse's IRQ. */
#define MC_IRQ			12

/** @brief KBC port (output buffer). */
#define KBC				0x60

/** @brief KBC's status port. */
#define KBC_STATUS		0x64

/** @brief Escape key code (ESC). */
#define ESC				0x81

/** @brief Q key code. */
#define Q				0x10

/** @brief W key code. */
#define W				0x11

/** @brief E key code. */
#define E				0x12

/** @brief R key code. */
#define R				0x13

/** @brief X key code. */
#define X				0x2D

/** @brief Z key code (it's actually the '<' key). */
#define Z				0x56

/** @brief Break key code. */
#define BREAK			0x80

/** @brief Two byte key code. */
#define TWOBYTE			0xE0

/** @brief Parity error. */
#define PAR_ERR			BIT(7)

/** @brief Timeout error. */
#define TO_ERR			BIT(6)

/** @brief Status' AUX. */
#define STATUS_AUX	 	BIT(5)

/** @brief Status' IBF. */
#define STATUS_IBF		BIT(1)

/** @brief Status' OBF. */
#define STATUS_OBF		BIT(0)

/** @brief Enable keyboard interrupts. */
#define KBD_INT			BIT(0)

/** @brief Enable mouse interrupts. */
#define M_INT			BIT(1)

/** @brief Keyboard's read command (KBC). */
#define READ_COMMAND	0x20

/** @brief Keyboard's write command (KBC). */
#define WRITE_COMMAND	0x60

/** @brief Mouse's write command (KBC). */
#define WRITE_MOUSE		0xD4

/** @brief Stream mode. */
#define STREAM_MODE		0xEA

/** @brief Remote mode. */
#define REMOTE_MODE		0xF0

/** @brief Enable data reporting. */
#define ENABLE_DATA		0xF4

/** @brief Disable data reporting. */
#define DISABLE_DATA	0xF5

/** @brief Request data. */
#define REQUEST_DATA	0xEB

/** @brief Mouse's acknowledge byte. */
#define ACK				0xFA

/** @brief Delay (in milliseconds). */
#define DELAY_US 		20000

/**
 *  @brief Reads from the output buffer.
 *  @param response Variable to save the response from the buffer.
 *  @return Returns 0 if successful and 1 otherwise.
 */
int getResponse(unsigned long *response);

/**
 *  @brief Reads the KBC's status.
 *  @param status Variable to save the status of the buffer.
 *  @return Returns 0 if successful and 1 otherwise.
 */
int getStatus(unsigned long *status);

/**
 *  @brief Sends write command to port.
 *  @param command Argument to write.
 *  @return Returns 0 if successful and 1 otherwise.
 */
int writeCommand(unsigned long command);

/**
 *  @brief Reads command from port.
 *  @return Returns the response if successful and 1 otherwise.
 */
int readCommand();

/**
 *  @brief Reads from the output buffer if the data is coming from the mouse.
 *  @param response Variable to save the response from the buffer.
 *  @return Returns 0 if successful and 1 otherwise.
 */
int M_getResponse(unsigned long *response);

/**
 *  @brief Sends mouse's write command to port
 *  @param command Argument to write.
 *  @return Returns 0 if successful and 1 otherwise.
 */
int M_writeCommand(unsigned long command);

/**@} end of i8042 */

#endif /* _LCOM_I8042_H */
