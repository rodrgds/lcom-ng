#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define IN_OUT_BUFFER_KBC_PORT          0x60
#define STATUS_COMMAND_KBC_PORT         0x64

#define KBC_STATUS_PARITY_ERR           BIT(7)
#define KBC_STATUS_TIMEOUT_ERR          BIT(6)
#define KBC_STATUS_AUX_MOUSE            BIT(5)
#define KBC_STATUS_INH_FLAG             BIT(4)
#define KBC_STATUS_INPUT_BUFFER_FULL    BIT(1)
#define KBC_STATUS_OUTPUT_BUFFER_FULL   1
#define KBC_STATUS_CHECK_ERRS           (KBC_STATUS_PARITY_ERR | KBC_STATUS_TIMEOUT_ERR)

#define ESC_KEY_BREAKCODE               0x81
#define SCANCODE_FIRST_BYTE             0xE0

//SCANCODES FOR KEY INPUTS
#define W_MAKE                          0x11
#define A_MAKE                          0x1E
#define S_MAKE                          0x1F
#define D_MAKE                          0x20

#define F_MAKE                          0x21
#define J_MAKE                          0x24
#define K_MAKE                          0x25

#define UP_ARROW_MAKE                   0x48
#define DOWN_ARROW_MAKE                 0x50
#define LEFT_ARROW_MAKE                 0x4B
#define RIGHT_ARROW_MAKE                0x4D
//-------------------------

#define CHECK_IF_IS_BREAKCODE           BIT(7)

#define READ_COMMAND                    0x20
#define WRITE_COMMAND                   0x60
#define CHECK_KBC_SELF                  0xAA
#define CHECK_KBD_IFACE                 0xAB
#define DISABLE_KBD_IFACE               0xAD
#define ENABLE_KBD_IFACE                0xAE

#define COMMAND_BYTE_DIS2               BIT(5)
#define COMMAND_BYTE_DIS                BIT(4)
#define COMMAND_BYTE_INT2               BIT(1)
#define COMMAND_BYTE_INT                1

#define DELAY_KB                        tickdelay(micros_to_ticks(20000))
#define MAX_RETRIES                     10

#define MOUSE_WRITE_TO_AUX_DEVICE       0xD4
#define ENABLE_MOUSE_DATA_REPORTING     0xF4
#define DISABLE_MOUSE_DATA_REPORTING    0xF5

#define MOUSE_ERROR_RESPONSE            0xFE
#define MOUSE_ERROR_RESEND              0xFC
#define MOUSE_ACKNOWLEDGMENT            0xFA

#define MOUSE_BYTE1_BIT7_Y_OF           BIT(7)
#define MOUSE_BYTE1_BIT6_X_OF           BIT(6)
#define MOUSE_BYTE1_BIT5_Y_SIGN         BIT(5)
#define MOUSE_BYTE1_BIT4_X_SIGN         BIT(4)
#define MOUSE_BYTE1_BIT3_1              BIT(3)
#define MOUSE_BYTE1_BIT2_MID_BTTN       BIT(2)
#define MOUSE_BYTE1_BIT1_RGT_BTTN       BIT(1)
#define MOUSE_BYTE1_BIT0_LFT_BTTN       BIT(0)
#define DELTA_NEG_SIGN_EXT              0xFF00

#define KBC_IRQ                         1
#define MOUSE_IRQ                       12

#endif /* _LCOM_I8042_H */
