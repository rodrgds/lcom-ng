#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/* LAB3  */
#define KBO_IRQ       1
#define BIT(n) (0x01<<(n))
#define DELAY_US 20000

//initializing the 8042 Keyboard Controller
#define MSB 0x80;
#define STATUS_PORT 0x64
#define KBD_OUT_BUF 0x60
#define KBD_INPUT_BUF 0x60
#define KBD_CMD_BUF 0x64
#define KBD_DATA_BUF 0x60
#define PORT_A 0x60
#define PORT_B 0x61
#define OUT_BUF_FULL 0x01
#define INPT_BUF_FULL 0x02
#define SYS_FLAG 0x04
#define CMD_DATA 0x08
#define KEYBOARD_INH 0x10
#define TRANS_TMOUT 0x20
#define RCV_TMOUT 0x40
#define PARITY_EVEN 0x80
#define INH_KEYBOARD 0x10
#define KB_ENA 0xAE
#define KB_DIS 0xAD
#define TIME_OUT BIT(5)
#define PARITY_ERROR BIT(7)


//Keyboard commands
#define KB_MENU 0xF1
#define KB_ENABLE 0xF4
#define KB_MAKEBREAK 0xF7
#define KB_ECHO 0xFE
#define KB_RESET 0xFF
#define KB_LED_CMD 0xED
#define READ_TEST_INPUTS 0xE0

//Keyboard responses
#define KB_OK 0xAA
#define KB_ACK 0xFA
#define KB_OVERRUN 0xFF
#define KB_RESEND 0xFE
#define KB_BREAK 0xF5
#define KB_FA 0x10
#define KB_FE 0x20
#define KB_PR_LED 0x40

/*    LAB4    */

#define IRQ_MOUSE 12
#define READ_BACK_COMMAND 0x20
#define WRITE_COMMAND_BYTE 0x60
#define DISABLE_MOUSE 0xA7
#define ENABLE_MOUSE 0xA8
#define CHECK_MOUSE_INTERFACE 0xA9
#define WRITE_MOUSE_BYTE 0xD4
#define SET_STREAM_MODE 0xEA
#define ENABLE_DATA 0xF4
#define STATUS_REQUEST 0xE9
#define READ_DATA 0xEB
#define MOUSE_ACK 0xFA
#define DISABLE_STREAM_MODE 0xF5

#endif /* _LCOM_I8042_H */


