#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard.
 */

#define DELAY          20000     // 20ms


/* KBC */

#define STATUS_REG     0x64
#define KBC_CMD        0x64
#define IN_BUF         0x60
#define OUT_BUF        0x60

#define READ_COMM_BYTE          0x20    // Read back command
#define WRITE_COMM_BYTE         0x60

#define OBF            BIT(0)   // Output buffer
#define IBF            BIT(1)   // Input buffer
#define INH            BIT(4)   // Inhibit flag
#define AUX            BIT(5)   // Mouse data
#define TIMEOUT        BIT(6)   // Timeout error - invalid data
#define PARITY         BIT(7)   // Parity error - invalid data



/* KEYBOARD */

#define KEYBOARD_IRQ       1

#define MAKE_OR_BREAK      BIT(7) 
#define SCAN_BREAK_ESC     0x81
#define SCAN_FIRST_TWO     0xE0    // Scancode of first byte of two bytes

#define SCAN_MAKE_X        0x2D
#define SCAN_MAKE_Z        0x2C

#define ENABLE_INT_KEYBOARD     BIT(0)

#define ENTER_MK        0x1C 
#define A_KEY_MK        0x1e
#define A_KEY_BRK       0x9e
#define S_KEY_MK        0x1F
#define S_KEY_BRK       0x9f
#define D_KEY_MK        0x20
#define D_KEY_BRK       0xa0
#define W_KEY_MK        0x11
#define W_KEY_BRK       0x91

#define G_KEY_BRK       0xA2
#define MK_1            0x02
#define MK_2            0x03
#define MK_3            0x04
#define BK_1            0x82
#define BK_2            0x83
#define BK_3            0x84

/* MOUSE */

#define MOUSE_IRQ          12

#define WRITE_BYTE_MOUSE   0xD4
#define ENABLE_DATA_MODE   0xF4
#define DISABLE_DATA_MODE  0xF5


#define ACK                0xFA
#define NACK               0xFE
#define ERROR              0xFC
#define END                0x40


#define M_LB            BIT(0)  // Mouse left button
#define M_RB            BIT(1)  // Mouse right button
#define M_MB            BIT(2)  // Mouse middle button

#define M_BYTE_1        BIT(3)  // Byte 

#define M_MSB_X_DELTA   BIT(4)  // Relative displacement MSBit X axis
#define M_MSB_Y_DELTA   BIT(5)  // Relative displacement MSBit Y axis

#define M_X_OVFL        BIT(6)  // Flag that the mouse displacement is to large
#define M_Y_OVFL        BIT(7)  // Flag that the mouse displacement is to large


#endif /* _LCOM_I8042_H_ */
