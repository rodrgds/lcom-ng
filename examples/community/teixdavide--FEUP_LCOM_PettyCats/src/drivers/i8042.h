#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define KEYBOARD_IRQ 1

#define MOUSE_IRQ 12

#define STATUS_PORT 0x64

#define KBD_OUT_BUF 0x60

#define ESC_BREAK 0x81

#define CODE_2_BYTES 0xE0

#define KEYBOARD_READ_COM 0x20

#define KEYBOARD_WRITE_COM 0x60

#define ACK 0xfa

#define MOUSE_WRITE_COM 0xd4

#define DISABLE_DATA_REP 0xf5

#define ENABLE_DATA_REP 0xf4


#endif /* _LCOM_I8042_H */
