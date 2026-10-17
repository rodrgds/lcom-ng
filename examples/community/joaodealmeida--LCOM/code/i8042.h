#ifndef __I8042_H
#define __I8042_H


#define DELAY_US 20000

#define STAT_REG 0x64
#define DATA_REG 0x60

#define BIT(n) (0x01<<(n))

//use interrupt.h CLOCK_IRQ  /**< @brief Timer 0 IRQ line */
//use interrupt.h KEYBOARD_IRQ  /**< @brief Keyboard IRQ line */
//#define KBD_AUX_IRQ 12 /* AUX (PS/2 mouse) port in kbd controller */

/* STATUS REGISTERS */

#define OBF        BIT(0)
#define IBF        BIT(1)
#define AUX        BIT(5)
#define TO_ERR     BIT(6)
#define PAR_ERR    BIT(7)

/* KEYBOARD RESPONSES */

#define ACK 0xFA
#define RESEND 0xFE
#define ERROR 0xFC

/* KEYBOARD Commands */

#define RESET_KBD 0xFF
#define ENABLE_KBD 0xF6
#define DISABLE_KBD 0xF5
#define CLEAR_KBD 0xF4
#define RATE_KBD 0xF3
#define SET_LEDS_KBD 0xED

/* KBC Commands */
#define READ_CMD 0x20 //Returns Command Byte
#define WRITE_CMD 0x60

/* KEYBOARD-Related KBC Commands */

#define CHECK_KBC 0xAA //Returns 0x55 if ok , 0xFC if error
#define CHECKINT_KBC // Returns 0 if OK
#define DISABLEINT_KBD 0xAD //Disable KBD Interface
#define ENABLEINT_KBD 0xAE //Enable KBD Interface

/* Mouse-Related KBC Commands */
#define WRITEBYTE_2MOUSE_KBC 0xD4 //forward argument to mouse without any interpretation

//WRITE_BYTE_TO_MOUSE 0xD4 argument commands
#define MCMD_RESET 0xFF // Mouse reset = RESET KBD
//=RESEND ALREADY DEFINED ABOVE 0XFE.
#define MCMD_SET_DEFAULTS 0xF6 //Set default values
#define MCMD_DISABLE_STREAM_MODE 0xF5 //Disable Stream Mode (In stream mode, should be sent before any other command
#define MCMD_ENABLE_SEND_DATAPACKS 0xF4 //Enable Sending Data Packets (In stream mode only)
#define MCMD_SET_RATE 0xF3 //Sets state sampling rate
#define MCMD_SET_REMOTE_MODE 0xF0 //Send data on request only
#define MCMD_READ_DATA 0xEB //Send data packet request
#define MCMD_SET_STREAM_DATA 0xEA //Send data on events
#define MCMD_STATUS_REQUEST 0xE9 //Get mouse configuration (3 bytes)
#define MCMD_SET_RES 0xE8 //Set Resolution
#define MCMD ACCEL_MODE 0xE7 //acceleration mode
#define MCMD_LINEAR_MODE0xE6 //linear mode


#endif
