#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define KEYBOARD_IRQ 1 // Keyboard IRQ line
#define MOUSE_IRQ (BIT(2) | BIT(3)) // Mouse IRQ line

// PS/2 scan codes set 1
#define ADDITIONAL_BYTE 0xE0
#define IS_BREAK_CODE(C) (((C) & 0x80) >> 7)
#define IS_MAKE_CODE(C) (!IS_BREAK_CODE(C))
#define CREATE_MAKE_CODE(BC) ((BC) & 0x7F)
#define CREATE_BREAK_CODE(MC) ((MC) | 0x80)

// One byte scan codes
// Function Keys
#define MAKE_F1       0x3B
#define MAKE_F2       0x3C
#define MAKE_F3       0x3D
#define MAKE_F4       0x3E
#define MAKE_F5       0x3F
#define MAKE_F6       0x40
#define MAKE_F7       0x41
#define MAKE_F8       0x42
#define MAKE_F9       0x43
#define MAKE_F10      0x44
#define MAKE_F11      0x57
#define MAKE_F12      0x58

// Numbers keys
#define MAKE_1        0x02
#define MAKE_2        0x03
#define MAKE_3        0x04
#define MAKE_4        0x05
#define MAKE_5        0x06
#define MAKE_6        0x07
#define MAKE_7        0x08
#define MAKE_8        0x09
#define MAKE_9        0x0A
#define MAKE_0        0x0B

// Character keys
#define MAKE_Q        0x10
#define MAKE_W        0x11
#define MAKE_E        0x12
#define MAKE_R        0x13
#define MAKE_T        0x14
#define MAKE_Y        0x15
#define MAKE_U        0x16
#define MAKE_I        0x17
#define MAKE_O        0x18
#define MAKE_P        0x19
#define MAKE_A        0x1E
#define MAKE_S        0x1F
#define MAKE_D        0x20
#define MAKE_F        0x21
#define MAKE_G        0x22
#define MAKE_H        0x23
#define MAKE_J        0x24
#define MAKE_K        0x25
#define MAKE_L        0x26
#define MAKE_Z        0x2C
#define MAKE_X        0x2D
#define MAKE_C        0x2E
#define MAKE_V        0x2F
#define MAKE_B        0x30
#define MAKE_N        0x31
#define MAKE_M        0x32

// Special Keys
#define MAKE_ESC            0x01
#define MAKE_BKSP           0x0E  
#define MAKE_TAB            0x0F
#define MAKE_ENTER          0x1C
#define MAKE_LCTRL          0x1D
#define MAKE_LSHIFT         0x2A
#define MAKE_RSHIFT         0x36
#define MAKE_LALT           0x38
#define MAKE_SPACE          0x39
#define MAKE_CAPSLOCK       0x3A
#define MAKE_NUMLOCK        0x45
#define MAKE_SCROLLLOCK     0x46

// Number Pad
#define MAKE_NP_0           0x52 
#define MAKE_NP_1           0x4F 
#define MAKE_NP_2           0x50 
#define MAKE_NP_3           0x51 
#define MAKE_NP_4           0x4B 
#define MAKE_NP_5           0x4C
#define MAKE_NP_6           0x4D 
#define MAKE_NP_7           0x47 
#define MAKE_NP_8           0x48  
#define MAKE_NP_9           0x49 
#define MAKE_NP_MINUS       0x4A
#define MAKE_NP_PLUS        0x4E
#define MAKE_NP_PERIOD      0x53
#define MAKE_NP_ASTERISK    0x37

// Other
#define MAKE_MINUS      0x0C
#define MAKE_EQUAL      0x0D
#define MAKE_LBRACK     0x1A 
#define MAKE_RBRACK     0x1B 
#define MAKE_SEMI       0x27 
#define MAKE_QUOTE      0x28 
#define MAKE_BACKTICK   0x29 
#define MAKE_BKSLASH    0x2B 
#define MAKE_COMMA      0x33
#define MAKE_PERIOD     0x34
#define MAKE_SLASH      0x35

// Two Bytes scan codes, 0xE0
// Special Keys
#define MAKE_PRTSC      0x37
#define MAKE_INSERT     0x52
#define MAKE_HOME       0x47
#define MAKE_PAGEU      0x49
#define MAKE_DELETE     0x53
#define MAKE_END        0x4F
#define MAKE_PAGED      0x51
#define MAKE_RARROW     0x4D
#define MAKE_LARROW     0x4B
#define MAKE_DARROW     0x50
#define MAKE_UARROW     0x48

// Number Pad
#define MAKE_NP_SLASH   0x35
#define MAKE_NP_ENTER   0x1C

// Break Scan Code
// BREAK_FIRST = ADDITIONAL_BYTE
#define BREAK_SECOND    0x46
#define BREAK_THIRD     0xE0
#define BREAK_FOURTH    0xC6

// Pause Scan Code
#define PAUSE_FIRST     0xE1
#define PAUSE_SECOND    0x1D
#define PAUSE_THIRD     0x45
#define PAUSE_FOURTH    0xE1
#define PAUSE_FIFTH     0x9D
#define PAUSE_SIXTH     0xC5

// Mouse
#define MOUSE_LEFT          BIT(0)
#define MOUSE_RIGHT         BIT(1)
#define MOUSE_MIDDLE        BIT(2)
#define MOUSE_CTRL_BYTE     BIT(3)
#define MOUSE_X_DELTA       BIT(4)
#define MOUSE_Y_DELTA       BIT(5)
#define MOUSE_X_OVERFLOW    BIT(6)
#define MOUSE_Y_OVERFLOW    BIT(7)

// Mouse Commands
#define DISABLE_DATA    0xF5
#define ENABLE_DATA     0xF4
#define ENABLE_STREAM   0xEA
#define MOUSE_DATA      0xEB
#define WRITE_MOUSE     0xD4
#define MOUSE_ACK       0xFA
#define MOUSE_NACK      0xFE

// KBC Registers
#define KBC_STATUS_PORT 0x64
#define KBC_STAT_REG   0x64
#define KBC_CMD_IN_BUF 0x64
#define KBC_ARG_IN_BUF 0x60
#define KBC_OUT_BUF    0x60
#define SCAN_CODE_HEADER 0xE0 // FOR SCANCODES THAT USE TWO BYTES 

// KBC Commands
#define KBC_READ_CMD    0x20
#define KBC_WRITE_CMD   0x60
#define KBC_CHECK_KBC   0xAA
#define KBC_CHECK_INT   0xAB
#define KBC_DISABLE_INT 0xAD
#define KBC_ENABLE_INT  0xAE

// KBC Command Byte
#define KBC_DIS2    BIT(5)
#define KBC_DIS     BIT(4)
#define KBC_INT2    BIT(1)
#define KBC_INT     BIT(0)
#define MAKE_BIT    BIT(7)

#define KBC_PAR_ERR BIT(7)
#define KBC_TO_ERR  BIT(6)


// KBC Status
#define KBC_PARITY  BIT(7)
#define KBC_TIMEOUT BIT(6)
#define KBC_AUX     BIT(5)
#define KBC_INH     BIT(4)
#define KBC_A2      BIT(3)
#define KBC_SYS     BIT(2)
#define KBC_IBF     BIT(1)
#define KBC_OBF     BIT(0)

#define KBC_WAIT_TIME 20000

#endif
