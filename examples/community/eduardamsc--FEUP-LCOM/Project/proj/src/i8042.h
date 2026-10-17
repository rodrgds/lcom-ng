#define KBD_IRQ 1

#define STAT_REG 0x64
#define KBC_CMD_REG 0x64

#define READ_CMD_BYTE 0x20
#define WRITE_CMD_BYTE 0x60

#define IN_BUF 0x60
#define OUT_BUF 0x60

#define IBF BIT(1)
#define OBF BIT(0)

#define ESC 0x81
#define KEY_A 0x1E
#define KEY_S 0x1F
#define KEY_W 0x11

#define NTRIES 5

//#define BIT(n) (0x01<<(n))

#define DELAY_US    20000
#define TIMEOUT BIT(6)
#define PARITY BIT(7)

#define TWOBYTE 0xe0

//Mouse
#define MOUSE_IRQ 12
#define AUX BIT(5)
#define ACK 0xFA //if everything OK
#define NACK 0xFE //if invalid byte (may be because of a serial communication error)
#define RESEND 0xFE
#define MOUSE_EN_DATA	0xF4
#define MOUSE_WRITE	0xD4
#define ERROR 0xFC //second consecutive invalid byte
#define WRITE_BYTE_MOUSE 0xD4
#define ENABLE_DATA_REPORT 0xF4
#define DISABLE_DATA_REPORT 0xF5
#define ENABLE_STREAM 0xEA
#define ENABLE_REMOTE 0xF0
#define NOT_BIT1 0xFD
#define READ_DATA 0xEB

#define YOVFL BIT(7)
#define XOVFL BIT(6)
#define YSIGN BIT(5)
#define XSIGN BIT(4)
#define MIDB BIT(2)
#define RIGHTB BIT(1)
#define LEFTB BIT(0)
