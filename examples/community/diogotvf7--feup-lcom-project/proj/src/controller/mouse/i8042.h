#ifndef __I8042_H_
#define __I8042_H_


/*          CONSTANTS         */
#define KEYBOARD_KBD_IRQ  0x1
#define MOUSE_KBD_IRQ     12
#define DELAY_US          20000
#define ATTEMPTS          0xa0000

/*     I/O PORT ADDRESSES     */
#define KBC_STAT_REG      0x64
#define KBC_CMD_REG       0x64
#define KBC_IN_BUF        0x60
#define KBC_OUT_BUF       0x60

/*    STATUS REGISTER BITS    */
#define PARITY_ERROR      BIT(7)
#define TIMEOUT_ERROR     BIT(6)
#define AUX               BIT(5)
#define INH               BIT(4)
#define IBF               BIT(1)
#define OBF               BIT(0)

/*    AKNOWLEDGMENT BYTES     */
#define ACK               0xFA
#define NACK              0xFE
#define ERROR             0xFC

/*     KEYBOARD SCANCODES     */
#define SCANCODE_MSB      0x80
#define DOUBLE_SCANCODE   0xE0
#define ESC_MAKE          0x1
#define ESC_BREAK         0x81

/*      MOUSE FIRST BYTE      */
#define Y_OVFL            BIT(7)
#define X_OVFL            BIT(6)
#define MSB_Y_DELTA       BIT(5)
#define MSB_X_DELTA       BIT(4)
#define FIRST_BYTE        BIT(3)
#define MOUSE_MB          BIT(2)
#define MOUSE_RB          BIT(1)
#define MOUSE_LB          BIT(0)

/*          COMMANDS          */
#define READ_CMD_BYTE     0x20
#define WRITE_CMD_BYTE    0x60
#define DISABLE_MOUSE     0xA7
#define ENABLE_MOUSE      0xA8
#define CHECK_MI          0xA9
#define WRITE_BYTE_MOUSE  0xD4

/*         COMMAND BYTE        */
#define INT               BIT(0)
#define INT2              BIT(1)
#define DIS               BIT(4)
#define DIS2              BIT(5)

/*          ARGUMENTS         */
#define RESET             0xFF  // Mouse reset
#define RESEND            0xFE  // For serial communications errors
#define SET_DEFAULTS      0xF6  // Set default values
#define DISABLE_DATA_REP  0xF5  // In stream mode, should be sent before any other command
#define ENABLE_DATA_REP   0xF4  // In stream mode only
#define SET_SAMPLE_RATE   0xF3  // Sets state sampling rate
#define SET_REMOTE_MODE   0xF0  // Send data on request only
#define READ_DATA         0xEB  // Send data packet request
#define SET_STREAM_MODE   0xEA  // Set Stream Mode Send data on events
#define STATUS_REQUEST    0xE9  // Get mouse configuration (3 bytes)
#define SET_RESOLUTION    0xE8  // Set Resolution
#define SET_ACCEL_MODE    0xE7  // Set Scaling 2:1
#define SET_LINEAR_MODE   0xE6  // Set Scaling 1:1

#endif /* __I8254_H2 */

