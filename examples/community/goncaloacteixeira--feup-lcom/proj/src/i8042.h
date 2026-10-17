//
// Created by skidr on 14/10/2019.
//

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8254 KBC.
 */

#ifndef LAB3_I8042_H
#define LAB3_I8042_H

#define DELAY_US        10000       //!< Delay

#define KBD_IRQ         0x01        //!< IRQ Line for keyboard
#define ESC_MAKE        0x01        //!< ESC_MAKE Scancode
#define ESC_BREAK       0x81        //!< ESC_BREAK Scancode

#define LEFT_MAKE       0x4b        //!< LEFT_MAKE Scancode   
#define RIGHT_MAKE      0x4d        //!< RIGHT_MAKE Scancode
#define DOWN_MAKE       0x50        //!< DOWN_MAKE Scancode
#define A_MAKE          0x1e        //!< A_MAKE Scancode
#define D_MAKE          0xa0        //!< D_MAKE Scancode
#define UP_MAKE         0x48        //!< UP_MAKE Scancode
#define SPACE_MAKE      0x39        //!< SPACE_MAKE Scancode
#define ENTER_MAKE      0x1c        //!< ENTER_MAKE Scancode

#define BUFF_OUT        0x60        //!< Output buffer
#define ST_REG          0x64
#define MSB             0x80

#define TWO_BYTE_CODE   0xE0
#define BREAK_CODE      BIT(7)

// MOUSE
#define LB              BIT(0)
#define RB              BIT(1)
#define MB              BIT(2)
#define CHECK_BYTE1     BIT(3)
#define X_NEG           BIT(4)
#define Y_NEG           BIT(5)
#define X_OV            BIT(6)
#define Y_OV            BIT(7)

#define IRQ_MOUSE       12
#define IBF_FULL        BIT(1)
#define DIS_MOUSE       0xFD    // 1111 1101
#define EN_MOUSE        0x02    // 0000 0010

#define CMD_INIT        0xD4 	  // Write byte do mouse
#define CMD_REG         0x64    // Keyboard command register

#define ACK             0xFA	  // If everything OK
#define NACK            0xFE

#define REMOTE          0xF0
#define ENABLE          0xF4
#define DISABLE         0xF5

#define SET_STREAM      0xEA
#define READ            0xEB
#define ENABLE_KB_MOU   0xCC
#define BUF_OUT         0x60
#define BUF_IN	 	      0x60

// Status register macros
#define OB_FULL         BIT(0)  //!< Outbuffer full
#define IB_FULL         BIT(1)  //!< InBuffer full, if this is on you can't write commands into the buffer
#define TIMEOUT_ERROR   BIT(6)  //!< Data in outbuffer with timeout error
#define PRT_ERROR       BIT(7)  //!< Data in outbuffer with a parity error


#endif //LAB3_I8042_H
