/**
 * @file i8042.h
 * @brief Constants for programming the i8042 Keyboard Controller.
 *
 * This file contains the definitions of the constants used for interacting
 * with the i8042 Keyboard Controller. These constants include the status
 * register, input buffer, output buffer, and various status flags.
 *
 * @defgroup i8042 i8042
 * @{
 */

#define DELAY_US 20000
#define MAX_ATTEMPS 10
#define ERROR -1    
#define OK 0

#define BREAKCODE BIT(7)

// Breakcodes
#define ESC_BREAK 0x81
#define FLAG_BREAK 0xE0
#define ARROW_UP_CODE 0x48
#define ARROW_DOWN_CODE 0x50
#define ENTER_CODE 0x1C
#define BACKSPACE_CODE 0x8E 
#define SPACE_CODE 0xB9 
#define SHIFT_CODE_MAKE 0x2A
#define SHIFT_CODE_BREAK 0xAA

#define KBC_IRQ 1
#define MOUSE_IRQ 12

/** @brief Status Register port */
#define KBC_STAT_REG 0x64

/** @brief  Same as IN_BUF*/
#define KBC_NOT_NAMED 0X64

/** @brief Input Buffer port */
#define KBC_IN_BUF 0x60

/** @brief Output Buffer port */
#define KBC_OUT_BUF 0x60

/** @brief Parity error - invalid data */
#define STATUS_PARITY BIT(7)

/** @brief Timeout error - invalid data */
#define STATUS_TIMEOUT BIT(6)

/** @brief Mouse data */
#define STATUS_AUX BIT(5)

/** @brief Inhibit flag - 0 if keyboard is inhibited*/
#define STATUS_INH BIT(4)

/** @brief Input buffer full - do not write commands or arguments */
#define STATUS_IBF BIT(1)

/** @brief Output buffer full - data available for reading */
#define STATUS_OBF BIT(0)


/*
0x20
0x60
0xA7
0xA8
0xA9
0xD4
Meaning
Read Command Byte
Write Command Byte
Disable Mouse
Enable Mouse
Check Mouse Interface
Write Byte to Mouse
Args (A)/ Return (R)
Command byte (R)
Command byte (A)
Returns 0, if OK
Byte (A)
*/




//kbc commands
#define KBC_READ_CMD 0x20


/** @} end of i8042 */
