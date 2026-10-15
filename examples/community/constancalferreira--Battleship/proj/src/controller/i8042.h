#ifndef I8042_H
#define I8042_H

#include <lcom/lcf.h>

//== GENERAL CONSTANTS ==//

#define DELAY_US 20000           ///< Delay for KBC operations (in microseconds)
#define MOUSE_SPEED 0.7          ///< Mouse movement speed multiplier

#define KBD_IRQ 1                ///< IRQ line for the keyboard
#define MOUSE_IRQ 12             ///< IRQ line for the mouse


//== I/O PORTS ==//

#define STATUS_REG 0x64          ///< KBC status register (read)
#define OUT_BUF 0x60             ///< Output buffer (read from KBC/mouse)
#define KBC_ARGS 0x60            ///< Input buffer for arguments to KBC
#define KBC_CMD_REG 0x64         ///< Register to write commands to KBC


//== KBC COMMANDS ==//

// General KBC commands
#define KBC_READ_CMD 0x20        ///< Read command byte from KBC
#define KBC_WRITE_CMD 0x60       ///< Write command byte to KBC
#define KBC_CHECK 0xAA           ///< Self-test command for KBC

// Keyboard interface commands
#define KBC_CHECK_KBD 0xAB       ///< Check keyboard interface
#define KBC_DISABLE_KBD 0xAD     ///< Disable keyboard interface
#define KBC_ENABLE_KBD 0xAE      ///< Enable keyboard interface

// Mouse interface commands
#define KBC_CHECK_MOUSE 0xA9     ///< Check mouse interface
#define KBC_ENABLE_MOUSE 0xA8    ///< Enable mouse interface
#define KBC_DISABLE_MOUSE 0xA7   ///< Disable mouse interface
#define KBC_WRITE_MOUSE 0xD4     ///< Write byte to mouse via KBC


//== KBC COMMAND BYTE BIT MASKS ==//

#define INT BIT(0)               ///< Enable keyboard interrupts
#define INT2 BIT(1)              ///< Enable mouse interrupts
#define DIS BIT(4)               ///< Disable keyboard
#define DIS2 BIT(5)              ///< Disable mouse


//== STATUS REGISTER BIT MASKS ==//

#define PARITY BIT(7)            ///< Parity error
#define TIMEOUT BIT(6)           ///< Timeout error
#define AUX BIT(5)               ///< 1 if data is from mouse
#define INH BIT(4)               ///< Inhibit flag
#define A2 BIT(3)                ///< Command/data distinction
#define SYS BIT(2)               ///< System flag
#define IBF BIT(1)               ///< Input buffer full
#define OBF BIT(0)               ///< Output buffer full


//== KEYBOARD CONSTANTS ==//

#define ESC_MAKE 0x01            ///< Make code for ESC key
#define ESC_BREAK 0x81           ///< Break code for ESC key
#define IS_BREAK BIT(7)          ///< Check if scancode is a break code


//== MOUSE COMMANDS ==//

#define MOUSE_RESET 0xFF         ///< Reset mouse
#define MOUSE_RESEND 0xFE        ///< Resend last command
#define MOUSE_SET_DEFAULT 0xF6   ///< Set default settings
#define MOUSE_DISABLE 0xF5       ///< Disable data reporting (stream mode)
#define MOUSE_ENABLE 0xF4        ///< Enable data reporting (stream mode)
#define MOUSE_SET_SAMPLE 0xF3    ///< Set sample rate
#define MOUSE_SET_REMOTE 0xF0    ///< Set remote mode
#define MOUSE_READ_DATA 0xEB     ///< Request data packet
#define MOUSE_SET_STREAM 0xEA    ///< Set stream mode
#define MOUSE_STATUS_REQ 0xE9    ///< Request status bytes
#define MOUSE_SET_RES 0xE8       ///< Set resolution
#define MOUSE_SET_SCALE_2 0xE7   ///< Set scaling 2:1
#define MOUSE_SET_SCALE 0xE6     ///< Set scaling 1:1


//== MOUSE PACKET BIT MASKS ==//

#define MOUSE_LB BIT(0)              ///< Left button
#define MOUSE_RB BIT(1)              ///< Right button
#define MOUSE_MB BIT(2)              ///< Middle button
#define IS_FIRST_BYTE BIT(3)         ///< First byte of mouse packet
#define MOUSE_MSB_X_DELTA BIT(4)     ///< MSB of X delta
#define MOUSE_MSB_Y_DELTA BIT(5)     ///< MSB of Y delta
#define MOUSE_X_OVERFLOW BIT(6)      ///< X overflow
#define MOUSE_Y_OVERFLOW BIT(7)      ///< Y overflow


//== MOUSE RESPONSE BYTES ==//

#define MOUSE_ACK 0xFA           ///< Command acknowledged
#define MOUSE_NACK 0xFE          ///< Command needs to be resent
#define MOUSE_ERROR 0xFC         ///< Error with command

#endif /* _LCOM_I8042_H */
