#include <lcom/lcf.h>


#include <stdbool.h>
#include <stdint.h>


//KBC commands
#define IRQ1                      1
#define IRQ12                     12
#define STATUS_REG                0x64
#define OUT_BUF                   0x60
#define IN_BUF                    0X60
#define DELAY_US                  2000


#define ESC_BREAKCODE             0x81

#define FIRST_BYTE                0xE0

//STATUS REGISTER
#define PARITY_ERROR              BIT(7)
#define TIMEOUT_ERROR             BIT(6)
#define INH                       BIT(4)
#define AUX                       BIT(3)  
#define IBF                       BIT(1) //IF BIT 1=1 CANNOT WRITE
#define OBF                       BIT(0)

//KEYBOARD-RELATED KBC COMMANDS
#define READ_COMMAND_BYTE         0X20
#define WRITE_COMMAND_BYTE        0X60
#define CHECK_KBC                 0XAA
#define CHECK_KEYBOARD_INTERFACE  0XAB
#define DISABLE_KBD_INTERFACE     0XAD
#define ENABLE_KBD_INTERFACE      0XAE


//KBC COMMAND BYTE
#define DIS2                      BIT(5)
#define DIS                       BIT(4)
#define INIT2                     BIT(1)
#define INIT                      BIT(0)

//MOUSE PACKET
#define Y_OVFL                    BIT(7)
#define X_OVFL                    BIT(6)
#define MSB_Y_DELTA               BIT(5)
#define MSB_X_DELTA               BIT(4)
#define MIDDLE_BIT                BIT(3)
#define MB                        BIT(2)
#define RB                        BIT(1)
#define LB                        BIT(0)

//MOUSE RELATED KBC COMMANDS
#define DISABLE_MOUSE             0XF5
#define ENABLE_MOUSE              0XF4
#define CHECK_MOUSE_INTERFACE     0XA9
#define WRITE_BYTE_TO_MOUSE       0XD4

//MOUSE COMMANDS
#define RESET                     0XFF
#define RESEND                    0XFE
#define SET_DEFAULTS              0XF6
#define DISABLE_DATA_REP          0XF5
#define ENABLE_DATA_REP           0XF4
#define SET_SAMPLE_RATE           0XF3
#define SET_REMOTE_MODE           0XF0
#define READ_DATA                 0XEB
#define SET_STREAM_MODE           0XEA
#define STATUS_REQUEST            0XE9
#define SET_RESOLUTION            0XE8
#define SET_SCALING_21            0XE7
#define SET_SCALING_11            0XE6
#define ACK                       0XFA
#define NACK                      0XFE
#define ERROR                     0XFC
