#ifndef _PROJ_I8042_
#define _PROJ_I8042_

// IRQ numbers for Keyboard and Mouse
#define MOUSE_IRQ 12
#define KBD_IRQ 1

// Geral
#define ST_REG 0x64
#define IN_COM_REG 0x64
#define IN_ARG_REG 0x60
#define OUT_REG 0x60

// Status Byte
#define PAR_ERR BIT(7)
#define TIME_ERR BIT(6)
#define MOUSE_AUX BIT(5)
#define KEYBOARD_INH BIT(4)
#define IN_BUF_FULL BIT(1)
#define OUT_BUF_FULL BIT(0)

// Comandos gerais para o Input Buffer 0x64
#define READ_BYTE 0x20
#define WRITE_BYTE 0x60

// Comandos de Keyboard para o Input Buffer 0x64
#define CHECK_KBC 0xAA
    #define KBC_OK 0x55
    #define KBC_ERR 0xFC
#define CHECK_KB_INTER 0xAB // retorna 0 se OK
#define DIS_KBD_INTER 0xAD
#define EN_KBD_INTER 0xAE

// Comandos de Mouse para o Input Buffer 0x64
#define WR_MOUSE_BYTE 0xD4
#define DIS_MOUSE 0xA7
#define EN_MOUSE 0xA8
#define CHECK_MOUSE_Inter 0xA9 // retorna 0 se OK

// Bytes escritos diretamente para o Mouse depois do comando 0xD4 (WR_MOUSE_BYTE)
#define RESET_MOUSE 0xFF
#define SET_DEFAULT 0xF6
#define DIS_DATA_REP 0xF5
#define EN_DATA_REP 0xF4
#define SET_FAST_SCALE 0xE7
#define SET_NORMAL_SCALE 0xE6
    // Possiveis respostas para o Byte escrito (recebidas no OUT_BUF)
    #define BYTE_ACK 0xFA
    #define BYTE_NO_ACK 0xFE
    #define BYTE_ERR 0xFC

// Representação dos bits no mouse packet 1º byte
#define MOUSE_LB BIT(0)
#define MOUSE_RB BIT(1)
#define MOUSE_MB BIT(2)
#define FRST_BYTE_CTRL BIT(3)
#define X_SIGN BIT(4)
#define Y_SIGN BIT(5)
#define X_OVR_FLOW BIT(6)
#define Y_OVR_FLOW BIT(7)

#endif // _PROJ_I8042_
