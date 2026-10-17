#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define IRQ_KEYBOARD          1
#define MOUSE_IRQ             12

#define BREAK_ESC             0x81
#define MAKE_CODE             BIT(7) // break bit
#define TWO_BYTES             0xE0
#define ENABLE_INT            BIT(0) // int bit, keyboard enabled

#define PARITY_ERROR          BIT(7)
#define TIMEOUT_ERROR         BIT(6)
#define MOUSE_CMP             BIT(5)
#define FULL_IN_BUFFER        BIT(1)
#define FULL_OUT_BUFFER       BIT(0)

#define KBC_STATUS_REG        0x64
#define KBC_IN_CMD            0x64
#define KBC_IN_CMD2           0x60
#define KBC_OUT_CMD           0x60
#define KBC_READ_CMD          0x20
#define KBC_WRITE_CMD         0x60

#define WAITING_TIME          20000
#define MAX_ATTEMPS           10

#define WRITE_BYTE_CMD_M      0XD4
#define ENABLE_DATA_M         0xF4
#define MOUSE_ACK             0xFA
#define DISABLE_DATA_M        0xF5

#define PRESSED_W             17  
#define PRESSED_A             30
#define PRESSED_S             31 
#define PRESSED_D             32
#define PRESSED_Q             16
#define PRESSED_E             18
#define PRESSED_B             48
#define PRESSED_N             49
#define PRESSED_M             50
#define PRESSED_H             35    
#define PRESSED_P             25
#define PRESSED_ENTER         28  


#endif /* _LCOM_I8042_H_ */


