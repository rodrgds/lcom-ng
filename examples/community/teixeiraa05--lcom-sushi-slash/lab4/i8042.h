#ifndef I8042_H
#define I8042_H

#define KBC_IRQ 1
#define MOUSE_IRQ 12

#define KBC_OUT_BUF 0x60
#define KBC_IN_BUF 0x60

#define KBC_STAT_REG 0x64
#define KBC_CMD_REG 0x64

#define PARITY_ERR BIT(7)
#define TIMEOUT_ERR BIT(6)
#define MOUSE_DATA BIT(5)

#define ESC_BREAK_CODE 0x81

#define KBC_MAX_ATTEMPTS 10
//===================== MOUSE =====================

#define WRITE_TO_AUX 0xD4

//mouse responses
#define ACK_BYTE 0xFA 
#define NACK_BYTE 0xFE //if the mouse responds with this, resend the entire command
#define ERROR_BYTE 0xFC //if the mouse responds with this, resend the entire command


#define ENABLE_DATA_REPORTING 0xF4
#define DISABLE_DATA_REPORTING 0xF5

//mouse data package byte1
#define LB BIT(0)
#define RB BIT(1)
#define MB BIT(2)
#define ALWAYS_1 BIT(3)
#define X_SIGN BIT(4)
#define Y_SIGN BIT(5)
#define X_OVF BIT(6)
#define Y_OVF BIT(7)

#define IBF BIT(1) //input buffer full
#define OBF BIT(0) //output buffer full

#define MOUSE_RESPONSE_DELAY 20000
#define PACKET_SIZE 3

#endif //I8042_H
