#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define IRQ_KEYBOARD    1       //< Keyboard IRQ line

#define BREAK_ESC       0x81    //< ESC key break code
#define BREAK_CODE      0x80    //< Base break code value
#define MAKE_CODE       BIT(7)  //< Bit mask to check if it's a make code
#define TWO_BYTES       0xE0    //< Prefix for two-byte scancodes
#define FULL_IN_BUFFER  BIT(1)  //< Input buffer is full
#define FULL_OUT_BUFFER BIT(0)  //< Output buffer is full
#define PARITY_ERROR    BIT(7)  //< Parity error flag in status register
#define TIMEOUT_ERROR   BIT(6)  //< Timeout error flag in status register
#define OK              0       //< Generic OK return value
#define FIRST_BYTE_OF_TWO 0xE0  //< First byte indicator of a two-byte scancode

#define KBC_STATUS_REG  0x64    //< KBC status register port
#define KBC_IN_CMD      0x64    //< KBC input command port
#define KBC_OUT_CMD     0x60    //< KBC output command port (for arguments)
#define KBC_READ_CMD    0x20    //< Command to read the command byte
#define KBC_WRITE_CMD   0x60    //< Command to write the command byte

#define STATUS_REG 0x64         //< Status register port
#define OUT_BUF 0x60            //< Output buffer port
#define KBC_CMD_REG 0x64        //< KBC command register port

#define OBF BIT(0)              //< Output buffer full flag
#define PAR_ERR BIT(7)          //< Parity error flag
#define TO_ERR BIT(6)           //< Timeout error flag

#define WAIT_KBC        20000   //< Delay in microseconds for KBC operations
#define MAX_ATTEMPS     10      //< Maximum number of retry attempts

#endif /* _LCOM_I8042_H_ */
