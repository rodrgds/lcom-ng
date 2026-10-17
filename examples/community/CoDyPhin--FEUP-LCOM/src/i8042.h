#ifndef LAB3_TEST_I8042_H
#define LAB3_TEST_I8042_H

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the Keyboard and Mouse
 */

//#  define BIT(n) (0x01 << (n)) //selects bit n

#define ESC_BREAKCODE           0x81 // breakcode of ESC key
#define TWO_BYTE_LSB            0xE0 //first byte of 2-byte scancode
#define MAKE_CODE               BIT(7)

#define MOUSE_IRQ               12 // mouse IRQ line

#define KEYBOARD_IRQ            1 // keyboard IRQ line
#define MSB                     0x80 // compares with the most significant byte



#define OUT_BUF                 0x60 // output buffer's address
#define IN_BUF                  0X60 // input buffer's address

#define STAT_REG                0x64 // status register
#define KBC_CMD_REG             0x64 // keyboard command register



    /*      Status Register commands        */

#define PAR_ERR                 BIT(7) // set if parity error - invalid data
#define TO_ERR                  BIT(6) // set if timeout error - invalid data
#define STAT_AUX                BIT(5) // set if data comes from mouse, not keyboard
#define STAT_INH                BIT(4) // set to 0 if keyboard is inhibited
#define STAT_A2                 BIT(3) // if 0, writes to data; if 1, writes in command
#define STAT_SYS                BIT(2) // 0 if system in power-on reset / 1 if system already initialized
#define IBF                     BIT(1) // if set, input buffer is full (don't write commands/arguments)
#define OBF                     BIT(0) // if set, output buffer is full (data is available to read, returns to 0 if data is read)

#define enableINT             BIT(0)

    /*      KBC Command Register        */

#define KBC_CMD_ARGS            0x60 // arguments for KBC commands must be passed from this address
#define KBC_READ_COMMAND_BYTE   0x20 // returns command byte
#define WRITE_COMMAND_BYTE      0x60 // writes command to kbc command register
#define KBC_CHECK               0xAA // returns 0x55 if OK, if an error occurs returns 0xFC
#define KBC_KB_INT              0xAB // if OK, returns 0
#define KBD_DISABLE_INT         0xAD // disables KBD interface
#define KBD_ENABLE_INT          0xAE // enables KBD interface

/*      Mouse KBC commands        */

#define KBC_DIS_MOUSE           0xA7 // disables mouse
#define KBC_EN_MOUSE            0xA8 // enables mouse
#define KBC_CHECK_MOUSE_INT     0xA9 // checks mouse interface (returns 0 if OK)
#define KBC_WRITE_MOUSE         0xD4 // Argument goes to mouse w/o interpretation



#define WAIT_KBC                20000



    /*      Mouse KBC commands        */

#define MOUSE_RESET             0xFF // resets mouse
#define MOUSE_RESEND            0xFE // when a serial communication error occurs
#define MOUSE_DEFAULT           0xF6 // sets default values
#define MOUSE_DISABLE           0xF5 // if in stream mode this should always be sent first
#define MOUSE_ENABLE            0xF4 // only in stream mode
#define MOUSE_SAMPLE            0xF3 // sets state sampling rate
#define MOUSE_REMOTE            0xF0 // sets remote mode (sends data only on request)
#define MOUSE_READ_DATA         0xEB // sends data packet request
#define MOUSE_STREAM            0xEA // sets stream mode (sends data on events)
#define MOUSE_STATUS            0xE9 // returns mouse configuration
#define MOUSE_RESOLUTION        0xE8 // sets resolution
#define MOUSE_21                0xE7 // sets acceleration mode
#define MOUSE_11                0xE6 // sets linear mode
#define ACK                     0xFA // if OK
#define NACK                    0xFE // if byte is invalid (maybe serial communication error)
#define ERROR                   0xFC // after 2nd consecutive NACK

#define BYTE1_CHECK             BIT(3) // bit 3 of first byte of a packet is always set
#define RB                      BIT(1)
#define LB                      BIT(0)
#define MB                      BIT(2)
#define Y_SIGN                  BIT(5)
#define X_SIGN                  BIT(4)
#define Y_OV                    BIT(7)
#define X_OV                    BIT(6)

    /*      RTC MACROS      */

#define RTC_ADDR_REG    0x70
#define RTC_DATA_REG    0x71
#define UIP             BIT(7) //set UIP of reg A before update and reset after
                               //so if  UIP is set, dont access time/date
#define DM              BIT(2) // set to get time and date in binary
#define REG_A           10
#define REG_B           11
#define REG_C           12
#define REG_D           13

#define SECONDS         0
#define MINUTES         2
#define HOURS           4
#define DAYS            7
#define MONTH           8
#define YEAR            9

/**@}*/

#endif /* _LCOM_I8042_H */
