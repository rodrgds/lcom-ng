#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/**
 * @file i8042.h
 * @brief Constants for programming the i8042 Keyboard Controller (KBC) and PS/2 Mouse.
 */

/** @defgroup i8042 i8042 KBC/Mouse Interface
 * @{
 *
 * Constants for programming the i8042 Keyboard Controller (KBC) and PS/2 Mouse.
 */

/* IRQ LINES */

#define IRQ_KBC             1   /**< @brief Keyboard Controller (KBC) IRQ line. */
#define IRQ_MOUSE           12  /**< @brief PS/2 Mouse IRQ line. */

/* KBC COMMANDS AND STATUS REGISTER BITS */

#define MAKE_CODE           BIT(7)  /**< @brief Scancode type bit (BIT(7)). If set, it's a break code. If clear, it's a make code. */
#define TWO_BYTE_CODE       0xE0    /**< @brief First byte of a two-byte scancode (e.g., for arrow keys, multimedia keys). */
#define ERR_PARITY          BIT(7)  /**< @brief Parity error bit in KBC Status Register. */
#define ERR_TIMEOUT         BIT(6)  /**< @brief Timeout error bit in KBC Status Register. */
#define FULL_OUT_BUF        BIT(0)  /**< @brief Output Buffer Full bit in KBC Status Register (OBF). Set when data is available for reading by the CPU. */
#define FULL_IN_BUF         BIT(1)  /**< @brief Input Buffer Full bit in KBC Status Register (IBF). Set when data has been written and is waiting for KBC processing. */

#define KBC_STAT_REG        0x64    /**< @brief KBC Status Register port. */
#define KBC_IN_CMD          0x64    /**< @brief KBC Input Command port (for KBC commands like 0x20, 0x60). Also used as argument port for 0x60. */
#define KBC_OUT_CMD         0x60    /**< @brief KBC Output Buffer port (for scancodes, mouse data, command byte read). Also used as argument port for commands to keyboard/mouse. */
#define KBC_READ_CMD        0X20    /**< @brief KBC command to read the KBC's command byte. */
#define KBC_WRITE_CMD       0x60    /**< @brief KBC command to write to the KBC's command byte. */

/* Keys Scancodes */
/* Note: For break codes, the MAKE_CODE bit (BIT(7)) is set. */
/* Two-byte scancodes are typically preceded by TWO_BYTE_CODE (0xE0). */

#define LEFT_ARROW          0x4b    /**< @brief Left Arrow key scancode (make code, typically preceded by 0xE0). */
#define RIGHT_ARROW         0x4d    /**< @brief Right Arrow key scancode (make code, typically preceded by 0xE0). */
#define DOWN_ARROW          0x50    /**< @brief Down Arrow key scancode (make code, typically preceded by 0xE0). */
#define UP_ARROW            0x48    /**< @brief Up Arrow key scancode (make code, typically preceded by 0xE0). */

#define ENTER               0x1c    /**< @brief Enter key scancode (make code). */
#define SPACE               0x39    /**< @brief Space key scancode (make code). */
#define ESC_BREAK           0x81    /**< @brief ESC key break code (0x01 | MAKE_CODE). Make code is 0x01. */

#define ONE_T               0x82    /**< @brief '1' key break code (0x02 | MAKE_CODE). Make code is 0x02. */
#define TWO_T               0x83    /**< @brief '2' key break code (0x03 | MAKE_CODE). Make code is 0x03. */
#define THREE_T             0x84    /**< @brief '3' key break code (0x04 | MAKE_CODE). Make code is 0x04. */
#define FOUR_T              0x05    /**< @brief '4' key scancode (make code). */
#define FIVE_T              0x06    /**< @brief '5' key scancode (make code). */

/* MOUSE RELATED DEFINITIONS */

/* KBC Command Byte Bits (for enabling/disabling KBD/Mouse and interrupts) */
/* These bits are part of the KBC command byte, read by KBC_READ_CMD and written by KBC_WRITE_CMD. */
#define DIS2                BIT(5)  /**< @brief KBC Command Byte: Disable Mouse Interface (set to 1 to disable mouse). */
#define DIS1                BIT(4)  /**< @brief KBC Command Byte: Disable Keyboard Interface (set to 1 to disable keyboard). */
#define INT2                BIT(1)  /**< @brief KBC Command Byte: Enable Interrupt on OBF from Mouse (set to 1 to enable mouse interrupts). */
#define INT1                BIT(0)  /**< @brief KBC Command Byte: Enable Interrupt on OBF from Keyboard (set to 1 to enable keyboard interrupts). */

/* Generic enable interrupt bit (often refers to keyboard interrupt enable) */
#define ENABLE_INT          BIT(0)  /**< @brief Generic enable interrupt bit, same as INT1 for keyboard interrupt enable in KBC command byte. */

/* Mouse Controller Responses */
#define MOUSE_ACK           0xFA    /**< @brief Mouse Acknowledgement: Command accepted by the mouse. */
#define MOUSE_NACK          0xFE    /**< @brief Mouse Negative Acknowledgement: Command not accepted or error occurred. Second NACK may indicate error. */

/* Mouse Packet Byte 1 Bits (Structure of the first byte of a standard 3-byte PS/2 mouse data packet) */
#define MOUSE_LB            BIT(0)  /**< @brief Mouse Packet Byte 1: Left Button status (1 if pressed). */
#define MOUSE_RB            BIT(1)  /**< @brief Mouse Packet Byte 1: Right Button status (1 if pressed). */
#define MOUSE_MB            BIT(2)  /**< @brief Mouse Packet Byte 1: Middle Button status (1 if pressed). */
#define FIRST_BYTE          BIT(3)  /**< @brief Mouse Packet Byte 1: Identifier bit, should always be 1 for a valid data packet. */
#define MOUSE_X_DELTA       BIT(4)  /**< @brief Mouse Packet Byte 1: X delta sign bit (1 for negative X movement). */
#define MOUSE_Y_DELTA       BIT(5)  /**< @brief Mouse Packet Byte 1: Y delta sign bit (1 for negative Y movement). */
#define MOUSE_X_OVERFLOW    BIT(6)  /**< @brief Mouse Packet Byte 1: X overflow bit (1 if X movement exceeded representable range). */
#define MOUSE_Y_OVERFLOW    BIT(7)  /**< @brief Mouse Packet Byte 1: Y overflow bit (1 if Y movement exceeded representable range). */

/* KBC Timing and Retry Constants */
#define WAIT_KBC            20000   /**< @brief Delay in microseconds to wait for KBC response or buffer status. */
#define MAX_ATTEMPS         10      /**< @brief Maximum number of attempts for KBC operations (e.g., reading status, writing commands). */

/* PS/2 Mouse Commands (to be sent to the mouse, usually after writing 0xD4 to KBC_IN_CMD) */
#define MOUSE_RESET         0xFF    /**< @brief Mouse Command: Reset the mouse. Mouse performs self-test and resets to default state. */
#define MOUSE_RESEND        0xFE    /**< @brief Mouse Command: Resend last packet. Used if a communication error is suspected. */
#define MOUSE_DEFAULT       0xF6    /**< @brief Mouse Command: Set default mouse settings. */
#define DIS_DATA_REP        0xF5    /**< @brief Mouse Command: Disable data reporting. Mouse stops sending data packets. */
#define ENBL_DATA_REP       0xF4    /**< @brief Mouse Command: Enable data reporting. Mouse starts sending data packets in stream mode. */
#define SET_SAMPLE          0xF3    /**< @brief Mouse Command: Set sample rate. Followed by a byte argument for the rate. */
#define SET_REMOTE          0xF0    /**< @brief Mouse Command: Set mouse to remote mode. Mouse sends data only upon READ_DATA command. */
#define READ_DATA           0xEB    /**< @brief Mouse Command: Request a single data packet from the mouse (remote mode only). */
#define SET_STREAM          0xEA    /**< @brief Mouse Command: Set mouse to stream mode. Mouse sends data automatically on event. */
#define MOUSE_CFG           0xE9    /**< @brief Mouse Command: Status Request. Mouse returns a 3-byte status packet. */

/**@}*/

#endif /* _LCOM_I8042_H_ */
