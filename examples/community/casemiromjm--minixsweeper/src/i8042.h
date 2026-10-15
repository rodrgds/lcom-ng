#ifndef _I8042_H_
#define _I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard Controller and PS/2 Mouse.
 *
 * This group contains macros and definitions for handling the i8042 controller,
 * which manages keyboard and mouse input on the PC. It includes I/O port addresses,
 * status register bits, command bytes, mouse/keyboard commands, and useful macros
 * for communication and data interpretation.
 */

#define KBD_IRQ 1
#define MOUSE_IRQ 12

#define TWO_BYTES 0xE0
#define BREAK_CODE BIT(7)   // If set, it is a Break code, if not it is a Make code
#define ESC_BREAKCODE 0x81

#define INT BIT(0) //Interrupt enable for keyboard in Command Byte

// I/O Ports for the PS/2 Controller
#define KBC_CMD_REG     0x64  // Keyboard controller command register
#define KBC_STATUS_REG  0x64  // Keyboard controller status register
#define KBC_IN_BUF      0x60  // Input buffer (keyboard)
#define KBC_OUT_BUF     0x60  // Output buffer (keyboard/mouse)

// Status Register Bits
#define KBC_OB_FULL         BIT(0)  // Output buffer full
#define KBC_IB_FULL         BIT(1)  // Input buffer full
#define KBC_SYS         BIT(2)  // System flag
#define KBC_A2          BIT(3)  // Command/Data flag (0 = Data, 1 = Command)
#define KBC_INH         BIT(4)  // Keyboard inhibit
#define KBC_AUX         BIT(5)  // Mouse data
#define KBC_TIMEOUT_ERROR     BIT(6)  // Timeout error
#define KBC_PARITY_ERROR      BIT(7)  // Parity error

// Keyboard Commands
#define KBC_READ_CMD    0x20  // Read command byte
#define KBC_WRITE_CMD   0x60  // Write command byte
// #define KBC_IN_CMD      0x64  // Sending commands to the KBC
// #define KBC_OUT_CMD     0x60  // Reading KBC data
#define KBC_SELF_TEST   0xAA  // Self-test
#define KBC_INTERFACE_TEST 0xAB  // Interface test
#define KBC_DISABLE_KBD 0xAD  // Disable keyboard
#define KBC_ENABLE_KBD  0xAE  // Enable keyboard

// Mouse Commands
#define MOUSE_DATA_REPORT_ON  0xF4    // Activate Mouse data report
#define MOUSE_DATA_REPORT_OFF  0xF5    // Deactivate Mouse data report

// Responses
#define KBC_ACK         0xFA  // Acknowledge
#define KBC_RESEND      0xFE  // Resend last byte
#define KBC_ERROR       0xFC  // Error

// Useful macros
#define DELAY_US        20000  // Delay for controller operations
#define MAX_ATTEMPTS    10     // Max attempts of communication

// Mouse Macros
#define CONTROL_BYTE    BIT(3)  // LCF assures that BIT 3 is active in every mouse control byte
#define MOUSE_LB        BIT(0)  // Mouse Left Button pressed
#define MOUSE_MB        BIT(2)  // Mouse Middle Button pressed
#define MOUSE_RB        BIT(1)  // Mouse Right Button pressed
#define MOUSE_X_OVERFLOW  BIT(6)  // Detects X position overflow
#define MOUSE_Y_OVERFLOW  BIT(7)  // Detects Y position overflow
#define MOUSE_X_SIGNAL    BIT(4)  // X movement direction (1 = negative, 0 = positive)
#define MOUSE_Y_SIGNAL    BIT(5)  // Y movement direction (1 = negative, 0 = positive)

#endif /* _I8042_H_ */
/** @} */
