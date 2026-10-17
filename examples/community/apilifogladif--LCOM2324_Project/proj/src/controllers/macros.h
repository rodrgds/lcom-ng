#ifndef _LCOM_MACRO_H_
#define _LCOM_MACRO_H_

#define BIT(n) (1 << (n))

// i8254 (timer controller)

#define TIMER_FREQ 1193182 /**< @brief clock frequency for timer in PC and AT */
#define TIMER0_IRQ 0 /**< @brief Timer 0 IRQ line */

    /* I/O port addresses */
#define TIMER_0    0x40 /**< @brief Timer 0 count register */
#define TIMER_1    0x41 /**< @brief Timer 1 count register */
#define TIMER_2    0x42 /**< @brief Timer 2 count register */
#define TIMER_CTRL 0x43 /**< @brief Control register */

#define SPEAKER_CTRL 0x61 /**< @brief Register for speaker control  */

    /* Timer control */

    /* Timer selection: bits 7 and 6 */

#define TIMER_SEL0   0x00              /**< @brief Control Word for Timer 0 */
#define TIMER_SEL1   BIT(6)            /**< @brief Control Word for Timer 1 */
#define TIMER_SEL2   BIT(7)            /**< @brief Control Word for Timer 2 */
#define TIMER_RB_CMD (BIT(7) | BIT(6)) /**< @brief Read Back Command */
#define TIMER_LSB     BIT(4)                  /**< @brief Initialize Counter LSB only */
#define TIMER_MSB     BIT(5)                  /**< @brief Initialize Counter MSB only */
#define TIMER_LSB_MSB (TIMER_LSB | TIMER_MSB) /**< @brief Initialize LSB first and MSB afterwards */
#define TIMER_SQR_WAVE (BIT(2) | BIT(1)) /**< @brief Mode 3: square wave generator */
#define TIMER_RATE_GEN BIT(2)            /**< @brief Mode 2: rate generator */
#define TIMER_BCD 0x01 /**< @brief Count in BCD */
#define TIMER_BIN 0x00 /**< @brief Count in binary */

    /* READ-BACK COMMAND FORMAT */

#define TIMER_RB_COUNT_  BIT(5)
#define TIMER_RB_STATUS_ BIT(4)
#define TIMER_RB_SEL(n)  BIT((n) + 1)

// i8042 (keyboard controller)

    // adresses
#define IN_BUF_COMMANDS             0x64
#define IN_BUF_ARGUMENTS            0x60
#define OUT_BUF                     0x60
#define ESC_BREAKCODE               0x81
#define READ_COMMAND_BACK           0x20
#define WRITE_COMAND_BACK           0x60
#define ST_REG                      0x64

    // status-register bit definitions
#define OBF                         BIT(0)
#define IBF                         BIT(1)
#define AUX                         BIT(5)  // mouse data
#define TIMEOUT_ERROR               BIT(6)
#define PARITY_ERROR                BIT(7)

#define KEYBOARD_IRQ                1
#define KBD_SCANCODE_BRK            BIT(7)

#define MAX_ATTEMPS                 10

// mouse

#define MOUSE_IRQ                   12
#define MOUSE_LB                    BIT(0)
#define MOUSE_RB                    BIT(1)
#define MOUSE_MB                    BIT(2)
#define FIRST_BYTE                  BIT(3)
#define MSB_X_DELTA                 BIT(4)
#define MSB_Y_DELTA                 BIT(5)
#define X_OVFL                      BIT(6)
#define Y_OVFL                      BIT(7)
#define DISABLE_CMD                 0xF5
#define ENABLE_CMD                  0xF4
#define SET_STREAM_MODE             0xEA
#define READ_DATA                   0xEB
#define WRITE_BYTE_TO_AUX_DEVICE    0xD4

#define ACK                         0xFA    
#define NACK                        0xFE
#define ERROR                       0xFC

// graphics
#define SET_VBE_MODE 0x4F
#define RETURN_VBE_MODE_INFO 0X4F
#define RETURN_VBE_CTRL_INFO 0x4F

// rtc
#define HOUR 0x04
#define MONTH 0x08
#define REGISTER_INPUT 0x70
#define REGISTER_OUTPUT 0x71
#define REG_A 0x0A
#define REG_B 0x0B

// fifo
#define MAXSIZE 100

// serial port
#define COM1                  0x3F8
#define COM1_IRQ              4

// uart registers
#define RBR                   0x00
#define THR                   0x00
#define IER                   0x01
#define IIR                   0x02
#define FCR                   0x02
#define LCR                   0x03
#define MCR                   0x04
#define LSR                   0x05
#define MSR                   0x06
#define SCRATCHPAD_REG        0x07

#define DIVISOR_LATCH_LSB     0x00
#define DIVISOR_LATCH_MSB     0x01

#define LSR_RECEIVER_DATA                       BIT(0)
#define LSR_OVERRUN_ERROR                       BIT(1)
#define LSR_PARITY_ERROR                        BIT(2)
#define LSR_FRAMING_ERROR                       BIT(3)
#define LSR_BREAK_INT                           BIT(4)
#define LSR_TRANSMITTER_HOLDING_REG_EMPTY       BIT(5)
#define LSR_TRANSMITTER_EMPTY_REG               BIT(6)
#define LSR_FIFO_ERROR                          BIT(7)

#define IER_ENABLES_RECEIVED_DATA_AVAILABLE_INT BIT(0)
#define IER_ENABLES_TRANSMIT_HOLD_REG_EMPTY_INT BIT(1)
#define IER_ENABLES_RECEIVER_LINE_STATUS_INT    BIT(2)

#define IIR_NO_INT_PENDING                      BIT(0)
#define IIR_INT_PENDING                         BIT(1) | BIT(2) | BIT(3)
#define IIR_RECEIVER_LINE_STATUS                BIT(1) | BIT(2)
#define IIR_RECEIVED_DATA_AVAILABLE             BIT(2)
#define IIR_CHAR_TIMEOUT_FIFO                   BIT(3) | BIT(2)
#define IIR_TRANSMITTER_HOLDING_REG_EMPTY       BIT(1)
#define IIR_MODEM_STATUS                        0x00

#define FCR_ENABLE_BOTH_FIFO                    BIT(0)
#define FCR_CLEAR_RCVR_FIFO                     BIT(1)
#define FCR_CLEAR_XMIT_FIFO                     BIT(2)
#define FCR_ENABLE_64_BYTE_FIFO                 BIT(5)

#define LCR_DLAB                                BIT(7)

// game macros
#define LOCAL 0x26          // L
#define REMOTE 0x13         // R
#define BOAT_2P 0x12        // E
#define BOAT_3P_1 0x20      // D
#define BOAT_3P_2 0x2E      // C
#define BOAT_4P 0x30        // B
#define BOAT_5P 0x1E        // A
#define ENTER 0x1C          // Enter
#define ARROW 0xE0
#define UP 0x48
#define DOWN 0x50
#define LEFT 0x4b
#define RIGTH 0x4d
#define ROTATE 0x13         // R
#define NEXT 0x0f           // Tab
#define H 0x23              // H

#define MATRIX_SIZE 10
#define BIG_BOARD_X 117
#define BIG_BOARD_Y 140
#define BIG_BOARD_SQUARE 68
#define SMALL_BOARD_X 813
#define SMALL_BOARD_Y 482
#define SMALL_BOARD_SQUARE 33

#define TRANSPARENT 0xfffffa

#endif
