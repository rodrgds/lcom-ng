#define MOUSE_IRQ 12

//Commands to the mouse -> Written into the control register (Port 0x64)
#define READ_CMD_BYTE 0x20
#define WRITE_CMD_BYTE 0X60
#define WRITE_BYTE_TO_MOUSE 0XD4
#define SET_STREAM_MODE 0XEA
#define ENABLE_DATA_REPORTING 0xF4
#define DISABLE_DATA_REPORTING 0XF5

#define Y_OVFL BIT(7)
#define X_OVFL BIT(6)
#define Y_SIGN BIT(5)
#define X_SIGN BIT(4)
#define MMB    BIT(2)
#define RMB    BIT(1)
#define LMB    BIT(0)

#define FIRST_BYTE BIT(3)

//Mouse Controller command responses
#define ACK 0xFA   //Acknowledged command or argument
#define NACK 0xFE  //Invalid byte
#define ERROR 0xFC //Second consecutive invalid byte


