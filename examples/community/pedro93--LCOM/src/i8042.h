#ifndef __I8042_H
#define __I8042_H

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 keyboard.
 */

#define KEYBOARDIRQ 0x01 //!< As defined in the Master PIC list

#define STAT_REG 0x64 //!< Adress value to which keyboard commands are sent
#define KBC_CMD_REG 0x64 //!< Same as STAT_REG
#define DATA_REG 0x60 //!< Data register to which the keyboard internal functions sends information
#define OUT_BUF 0x60 //!< Same as data register

#define LED_SWITCH_COMMAND 0xED
#define LED0_BITMASK = (1<<0) //!< Scroll Lock Indicator
#define LED1_BITMASK = (1<<1) //!< Numeric Lock Indicator
#define LED2_BITMASK = (1<<2) //!< Caps Lock Indicator
#define TWO_BYTE_BREAKCODE 0xE0

//KBD Responses after issuing a command
#define KBD_RESEND 0xFE  //!< Resend - The latest byte should be written again
#define KBD_ERROR 0xFC //!< Error - The entire sequence should be restarted
#define KBD_ACK 0xFA //!< ACK - Acknowledge OK response

#define ESC_BREAKCODE 0x81 //!< Escape key breakcode

#define BIT(n) (0x01<<(n))

#define DELAY_US 20000

//Keyboard Controller Status Register
#define OBF 	BIT(0)  //!< Output Buffer Full - data available for reading
#define IBF 	BIT(1)  //!< Input Buffer Full - don't write commands or arguments
#define SYS 	BIT(2)  //!< System Flag : 0 if system in power-on reset || 1 if system already initialized
#define A2 		BIT(3)  //!< A2 Input line : 0 data type || 1 command byte
#define INH 	BIT(4)  //!< Inhibit Flag : 0 if keyboard is inhibited
#define AUX 	BIT(5)  //!< Mouse Data
#define TO_ERR	BIT(6)  //!< Timeout Error
#define PAR_ERR BIT(7)  //!<  Parity Error

//PS/2 Mouse
#define MOUSE_IRQ 0x0C //!< IRQ 12

#define EnableMouse    0xA8
#define MouseReset     0xFF
#define MouseDefaults  0xF6
#define StreamDisable  0xF5
#define StreamEnable   0xF4
#define SampleRate     0xF3
#define ReadDevice     0xF2
#define RemoteMode     0xF0
#define MouseReadData  0xEB
#define EnterStream    0xEA
#define StatusRequest  0xE9
#define Resolution     0xE8
#define Scaling2       0xE7
#define Scaling1       0xE6
#define ReadCommand    0x20
#define WriteCommand   0x60
#define DisableMouse   0xA7
#define TestMouse      0xA9
#define WriteByteToMouse 0xD4

///PS2 Mouse Controller Status Register
#define YOVF BIT(7) //!< X overflow
#define XOVF BIT(6) //!< Y overflow
#define YSGN BIT(5)) //!< Sign of data packet byte 3 (y movement)
#define XSGN BIT(4) //!< Sign of data packet byte 2 (x movement)
#define MBT  BIT(2) //!< Middle button
#define RBT  BIT(1) //!< Right button
#define LBT  BIT(0) //!< Left button

#endif /* __I8042_H */
