#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/* --- IRQ LINES --- */
#define KBC_IRQ          1   // Keyboard IRQ
#define MOUSE_IRQ        12  // Mouse IRQ

/* --- KBC PORTS --- */
#define KBC_STATUS_REG   0x64 // Status Register (read)
#define KBC_CMD_REG      0x64 // Command Register (write)
#define KBC_OUT_BUF      0x60 // Output Buffer (read)
#define KBC_IN_BUF       0x60 // Input Buffer (write)

/* --- KBC STATUS BITS --- */
#define KBC_OBF          BIT(0) // Output Buffer Full
#define KBC_IBF          BIT(1) // Input Buffer Full
#define KBC_AUX          BIT(5) // Mouse Data (set when byte is from mouse)
#define KBC_TO_ERR       BIT(6) // Timeout Error
#define KBC_PAR_ERR      BIT(7) // Parity Error

/* --- KBC COMMANDS --- */
#define KBC_READ_CMD     0x20 // Read Command Byte
#define KBC_WRITE_CMD    0x60 // Write Command Byte

/* --- KEYBOARD SCANCODES --- */
#define TWO_BYTE_SCANCODE  0xE0 // Prefix for 2-byte scancodes
#define ESC_BREAKCODE      0x81 // Escape key break code

/* --- MOUSE COMMANDS --- */
#define WRITE_TO_MOUSE   0xD4 // KBC command to forward next byte to mouse
#define MOUSE_EN_DATA    0xF4 // Enable data reporting
#define MOUSE_DIS_DATA   0xF5 // Disable data reporting

/* --- MOUSE ACKNOWLEDGMENT --- */
#define MOUSE_ACK        0xFA // Command acknowledged
#define MOUSE_NACK       0xFE // Resend
#define MOUSE_ERR        0xFC // Error

/* --- MOUSE PACKET BITS --- */
#define MOUSE_LB         BIT(0) // Left Button
#define MOUSE_RB         BIT(1) // Right Button
#define MOUSE_MB         BIT(2) // Middle Button
#define MOUSE_SYNC_BIT   BIT(3) // Always 1 in first byte
#define MOUSE_X_SIGN     BIT(4) // X delta sign
#define MOUSE_Y_SIGN     BIT(5) // Y delta sign
#define MOUSE_X_OVF      BIT(6) // X overflow
#define MOUSE_Y_OVF      BIT(7) // Y overflow

/* --- HELPER MACROS --- */
#define DELAY_US            20000
#define MAX_ATTEMPTS        10
#define SIGN_EXTEND(val)    (0xFF00 | (val))

/* --- KBC DATA SOURCE FLAGS (used in kbc_read_data) --- */
#define KBC_SOURCE_KEYBOARD 0  // expect data from keyboard
#define KBC_SOURCE_MOUSE    1  // expect data from mouse

#endif /* _LCOM_I8042_H_ */
