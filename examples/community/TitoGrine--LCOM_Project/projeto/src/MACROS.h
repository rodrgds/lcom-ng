#ifndef MACROS_H
#define MACROS_H


// GENERAL MACROS

#define BIT(n)            (0x01<<(n))
#define DELAY             20000
#define STAT_REG          0x64
#define OUT_BUF           0x60


// STATUS REGISTER BITS

#define ST_PARITY         BIT(7)
#define ST_TIMEOUT        BIT(6)
#define ST_AUX            BIT(5)
#define ST_IBF            BIT(1)
#define ST_OBF            BIT(0)


// ACKNOWLEDGEMENT BYTES

#define ACK               0xFA
#define NACK              0xFE
#define ERROR             0xFC


// COMMAND BYTE

#define INT2              BIT(1)
#define INT               BIT(0)


// KEYBOARD MACROS

#define READ_CMD_BYTE     0x20
#define WRITE_CMD_BYTE    0x60
#define ENABLE_KBD_INT    0xAE
#define WRITE_BYTE_MOUSE  0xD4

#define KBD_IRQ           1
#define KBC_CMD_REG       0x64
#define ESC_BREAK         0x81

#define W_KEY_MAKE        0x11     
#define D_KEY_MAKE        0x20     
#define S_KEY_MAKE        0x1F     
#define A_KEY_MAKE        0x1E 

#define W_KEY_BREAK       0x91     
#define D_KEY_BREAK       0xA0     
#define S_KEY_BREAK       0x9F     
#define A_KEY_BREAK       0x9E 


// MOUSE MACROS

#define MOUSE_RESET       0xFF
#define DISABLE_DT_RPT    0xF5
#define ENABLE_DT_RPT     0xF4
#define SET_REMOTE_MODE   0xF0
#define READ_DATA         0xEB
#define SET_STREAM_MODE   0xEA

#define Y_OVF             BIT(7)
#define X_OVF             BIT(6)
#define Y_SIGNAL          BIT(5)
#define X_SIGNAL          BIT(4)
#define MMB               BIT(2)
#define RMB               BIT(1)
#define LMB               BIT(0)

#define MOUSE_IRQ         12


// GRAPHICS MACROS

#define TEXT_MODE         0x03
#define MODE_115          0x115
#define TRANSPARENT       0x00
#define VBE_MODE_INFO     0x4F01
#define VBE_MODE          0x10
#define VBE_CALL_SUCCESS  0x00
#define RED_COMPONENT     3
#define GREEN_COMPONENT   2
#define BLUE_COMPONENT    1

#define BLACK             0x00
#define WHITE             0xFFFFFF


// RTC COMMANDS

#define RTC_IRQ             8

#define RTC_ADDR_REG        0x70
#define RTC_DATA_REG        0x71

#define REG_A               0x0A
#define REG_B               0x0B
#define REG_C               0x0C

#define REG_SECONDS         0x01
#define REG_MINUTES         0x03
#define REG_HOURS           0x05

#define SECONDS             0
#define MINUTES             2
#define HOURS               4
#define DAYS                7
#define MONTHS              8
#define YEARS               9                 

#define UIP                 BIT(7)
#define DM                  BIT(2)

#define ALARM_INTERRUPT     BIT(5)
#define ENABLE_ALARM        BIT(5)
#define DISABLE_ALARM       0xEF



#endif
