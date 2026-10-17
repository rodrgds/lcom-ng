#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define IRQ_KEYBOARD    1
#define IRQ_MOUSE       12
#define BREAK_ESC       0x81
#define MAKE_CODE       BIT(7)
#define TWO_BYTES       0xE0
#define ENABLE_INT      BIT(0)

#define TIMEOUT_ERROR   BIT(6)
#define PARITY_ERROR    BIT(7)
#define FULL_OUT_BUFFER BIT(0)
#define FULL_IN_BUFFER  BIT(1)

#define KBC_STATUS_REG  0x64 /* Status Register*/
#define KBC_IN_CMD      0x64 /*para onde é escrito um kbcCommand */
#define KBC_OUT_CMD     0x60/*Output Buffer: de onde é lido o commandByte*/
#define KBC_READ_CMD    0x20/* Vai escrever no 0x64 um KBCCommand que notifica o i8042 da leitura de um commandByte*/
#define KBC_WRITE_CMD   0x60/* Vai escrever no 0x64 um KBCCommand que notifica o i8042 da escrita de um commandByte*/

#define WAIT_KBC        20000
#define MAX_ATTEMPS     10
/* Rato */
#define MOUSE_LB BIT(0)
#define MOUSE_RB BIT(1)
#define MOUSE_MB BIT(2)
#define FIRST_BYTE BIT(3)
#define MOUSE_X_DELTA BIT(4)
#define MOUSE_Y_DELTA BIT(5)
#define MOUSE_X_OVERFLOW BIT(6)
#define MOUSE_Y_OVERFLOW BIT(7)

#define DISABLE_DATA_REPORTING 0xF5
#define ENABLE_DATA_REPORTING 0xF4
#define ENABLE_STREAM_MODE 0xEA
#define WRITE_BYTE_TO_MOUSE 0xD4
#define MOUSE_READ_DATA 0xEB



#define ACK 0xFA    
#define NACK 0xFE
#define ERROR 0xFC


#define MAKE_1          0x02
#define MAKE_2          0x03
#define MAKE_3          0x04
#define MAKE_4          0x05
#define MAKE_5          0x06
#define MAKE_6          0x07
#define MAKE_7          0x08
#define MAKE_8          0x09
#define MAKE_9          0x0A
#define MAKE_b          0x30
#define MAKE_p          0x99
#define MAKE_m          0xb2
#define MAKE_r          0x93
#define MAKE_l          0xa6
#define MAKE_n          0xb1
#define MAKE_k          0xa5 
#define ESC             0x81
#define UP              0x48
#define DOWN            0x50

#endif /* _LCOM_I8042_H_ */
