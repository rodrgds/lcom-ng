#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define IRQ_MOUSE       12

#define MOUSE_LB                BIT(0)
#define MOUSE_RB                BIT(1)
#define MOUSE_MB                BIT(2)
#define FIRST_BYTE              BIT(3)
#define MOUSE_X_SIGNAL          BIT(4)
#define MOUSE_Y_SIGNAL          BIT(5)
#define MOUSE_X_OVERFLOW        BIT(6)
#define MOUSE_Y_OVERFLOW        BIT(7)

#define DISABLE_DATA_REPORTING  0xF5
#define ENABLE_DATA_REPORTING   0xF4
#define SET_STREAM_MODE         0xEA
#define MOUSE_READ_DATA         0xEB
#define WRITE_BYTE_MOUSE        0xD4
#define SET_SCALING_2_1         0xE7
#define SET_SCALING_1_1         0xE6

#define ACK 0xFA    
#define NACK 0xFE

#endif /* _LCOM_I8042_H_ */
