#ifndef LAB4_I8042_H

#define LAB4_I8042_H

#define KBD_IRQ 1

#define MOUSE_IRQ 12

#define Mask_make_brake 0x80

#define ESCKEY 0x81

#define SPACE_UP 0xB9

#define ENTER_KEY 0x9c

#define KBC_OUT_BUFF 0X60

#define KBC_CMD_REGISTER 0X64

#define STAT_REG 0x64

#define DELAY_US 20000

#define PAR_ERR BIT(7)

#define TO_ERR BIT(6)

#define MOUSE BIT(5)

#define OBF 1

#define IBF BIT(1)

#define TRIES 10

#define KBC_READ_COMBYTE 0x20

#define KBC_WRITE_COMBYTE 0x60

#define KBC_CB_INT BIT(0)

#define KBC_IN_BUFF 0x60

#define ACK 0xFA

#define NACK 0xFE

#define MOUSE_READ_DATA 0xEB

#define STREAM_MODE 0xEA     

#define MOUSE_CNTRL_ERROR 0xFC 

#define WRITE_BYTE_TO_MOUSE 0xD4 

#define ENABLE_DATA_REPORT 0xF4
#define DISABLE_DATA_REPORT 0xF5

#define DISABLED false
#define ENABLED true

#define LEFT_BUTTON BIT(0) 
#define RIGHT_BUTTON BIT(1)
#define MIDDLE_BUTTON BIT(2)
#define X_OVERFLOW BIT(6)
#define Y_OVERFLOW BIT(7)
#define X_SIGN BIT(4)
#define Y_SIGN BIT(5)

#endif //LAB4_I8042_H
