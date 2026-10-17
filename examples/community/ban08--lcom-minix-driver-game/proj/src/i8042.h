#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define KEYBOARD_IRQ            1
#define EN_INT                  BIT(0)
#define FULL_OUTPUT_BUF         BIT(0)

#define FULL_INPUT_BUF          BIT(1)
#define OUT_NOT_FOUND           BIT(5)
#define TIMEOUT_ERR             BIT(6)
#define PARITY_ERR              BIT(7)
#define MAKE_CODE               BIT(7)
#define BREAK_CODE_ESCAPE       0x81
#define TWO_BYTES_MASK          0xE0
#define KBC_WRITE_COMMAND       0x60
#define KBC_STATUS_REGISTER     0x64
#define KBC_OUT_COMMAND         0x60
#define KBC_IN_COMMAND          0x64
#define KBC_READ_COMMAND        0x20
#define WAIT_KBCONTROLLER       20000 //nome mais explicativo do que DELAY_US


//
//


#define MOUSE_IRQ       12

#define LB              BIT(0)
#define MB              BIT(2)
#define RB              BIT(1)

#define FST_BYTE        BIT(3)

#define M_X_SIG         BIT(4)
#define M_Y_SIG         BIT(5)

#define M_X_OVERFLOW    BIT(6)
#define M_Y_OVERFLOW    BIT(7)


#define EN_DATA_REP     0xF4
#define DIS_DATA_REP    0xF5
#define EN_STREAM_MODE  0xEA
#define M_READ_DATA     0xEB
#define WR_BYTE_M       0xD4

#define ACK 0xFA    
#define NACK 0xFE
//#define ERROR 0XFC

#endif
