#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 KBC.
 */

// I/O Ports
#define OUT_BUF 0x60    // Output buffer (read scancodes or mouse bytes)
#define IN_BUF_CMD 0x64 // Input buffer for commands
#define STAT_REG 0x64   // Status register

// Status Register Flags
#define PAR_ERR BIT(7) // Parity error
#define TO_ERR BIT(6)  // Timeout error
#define AUX BIT(5)     // Mouse data if set, keyboard data otherwise
#define INH BIT(4)     // Inhibit flag (keyboard interface inhibited if set)
#define A2 BIT(3)      // A2 input line (command/data distinction)
#define SYS BIT(2)     // System flag
#define IBF BIT(1)     // Input buffer full
#define OBF BIT(0)     // Output buffer full

// Mouse-related Commands
#define WRITE_BYTE_TO_MOUSE 0xD4
#define MOUSE_ENABLE_DATA 0xF4
#define MOUSE_DISABLE_DATA 0xF5
#define MOUSE_RESET 0xFF
#define MOUSE_ACK 0xFA
#define MOUSE_NACK 0xFE
#define MOUSE_ERROR 0xFC

// IRQ Lines
#define KBD_IRQ 1
#define MOUSE_IRQ 12

// Useful macros
#define DELAY_US 20000

#endif /* _LCOM_I8042_H_ */
