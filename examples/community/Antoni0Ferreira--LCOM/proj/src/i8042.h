#define KBC_ST_REG 0x64
#define KBC_CMD_REG 0x64
#define KBC_OUT_BUF 0x60
#define KBC_ARG_REG 0x60

#define OBF BIT(0)
#define IBF BIT(1)
#define AUX_MOUSE BIT(5)
#define PAR_ERROR BIT(7)
#define TIME_ERROR BIT(6)

#define READ_COMMAND 0x20
#define WRITE_COMMAMD 0x60
#define MOUSE_WRITE 0xD4
#define KEYBOARD_IRQ 1
#define MOUSE_IRQ   12

#define ESC 0x81
#define W_BREAK_CODE 0x91
#define S_BREAK_CODE 0x9F
#define A_BREAK_CODE 0x9E
#define D_BREAK_CODE 0xA0
#define ENTER_BREAK_CODE 0x9C

#define W_MAKE_CODE 0x11
#define S_MAKE_CODE 0x1F
#define A_MAKE_CODE 0x1E
#define D_MAKE_CODE 0x20
#define SPACE_MAKE_CODE 0X39
