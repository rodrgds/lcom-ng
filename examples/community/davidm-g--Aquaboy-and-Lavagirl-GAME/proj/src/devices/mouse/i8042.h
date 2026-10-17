#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define OBF BIT(0)
#define ENABLE_INTERRUPT BIT(0)
#define IBF BIT(1)
#define INH BIT(4)
#define AUX BIT(5)
#define TIM_ERR BIT(6)
#define PAR_ERR BIT(7)
#define LB BIT(0)
#define RB BIT(1)
#define MB BIT(2)
#define FIRST_BYTE BIT(3)
#define X_MSB BIT(4)
#define Y_MSB BIT(5)
#define X_OV BIT(6)
#define Y_OV BIT(7)

#define ST_REGISTER 0x64
#define READ_CMD_BYTE 0x20
#define WRITE_CMD_BYTE 0x60
#define OUT_BUF 0x60
#define KBC_CMD_REG 0x64
#define ESC_BREAK 0x81
#define ACK 0XFA
#define NACK 0XFE
#define ERROR 0XFC
#define MOUSE_IRQ 12
#define REQUEST_MOUSE 0XD4
#define KEYBOARD_IRQ 1
#define ENABLE_DATA 0XF4
#define DISABLE_DATA 0XF5
#define SET_STREAM_MODE 0XEA
#endif /* _LCOM_I8042_H_ */
