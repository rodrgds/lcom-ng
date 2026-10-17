//
// Created by barba on 10/05/2022.
//

#ifndef LAB4_IMOUSE_H
#define LAB4_IMOUSE_H

/* Mouse Macros */

/* Parse the mouse packets */
#define MOUSE_LB_PRESSED BIT(0)   // if left button is pressed
#define MOUSE_RB_PRESSED BIT(1)   // if right button is pressed
#define MOUSE_MB_PRESSED BIT(2)   // if middle button is pressed
#define MOUSE_BIT_ID BIT(3)       // bit 3 of the first byte is always 1
#define MOUSE_X_DELTA_SIGN BIT(4)
#define MOUSE_Y_DELTA_SIGN BIT(5)
#define MOUSE_X_OVF BIT(6)        // if there was an overflow on X axis
#define MOUSE_Y_OVF BIT(7)        // if there was an overflow on Y axis

/* PS/2 Mouse commands */
#define MOUSE_CMD 0xD4 //Write Byte to Mouse
//---
#define MOUSE_RESET 0xFF // Reset the mouse
#define MOUSE_DISABLE_CMD 0xF5 //Disable Data Reporting
#define MOUSE_ENABLE_CMD 0xF4 //Enable Data Reporting, in stream mode only
#define MOUSE_READ 0xEB //Read Data - When in remote mode, request the packet.
#define MOUSE_STREAM 0xEA //Set Stream Mode

/* PS/2 Mouse commands - responses */
#define MOUSE_OK 0xFA // if everything OK
#define MOUSE_NACK 0xFE // if invalid byte (due to communication errors)
#define MOUSE_ACK_ERROR 0xFC //error - second consecutive invalid byte

/* Mouse status register */
#define MOUSE_PARE BIT(7)  /**< @brief Status register parity error */
#define MOUSE_TIMEO BIT(6) /**< @brief Status register timeout error */
#define MOUSE_AUXB BIT(5)  /**< @brief Status register mouse data */ // if 1 -> data from the mouse, if 2 -> data from keyboard
#define MOUSE_KEYL BIT(4)  /**< @brief Status register keylock/inhibit flag */ // 0 -> if keyboard is inhibited
#define MOUSE_CD BIT(3)    /**< @brief Status register command/A2 input line */ // 1 -> KBC or data, 0 -> device
#define MOUSE_SYSF BIT(2)  /**< @brief Status register system flag */ // 0 -> if in power-on reset, 1 -> if already initialized
#define MOUSE_INPB BIT(1)  /**< @brief Status register input buffer flag */ // if 1 -> Input buffer full, don’t write commands or arguments
#define MOUSE_OUTB BIT(0)  /**< @brief Status register output buffer flag*/ // if 1 -> Output buffer full - data available for reading

/* Mouse buffer - PORTS (same as kbd buffer)*/
#define MOUSE_STATUS_REG 0x64 // Sent to KBC by writing to port 0x64
#define MOUSE_OUT_BUF 0x60 // Arguments and return values are written/read to/from 0x60
#define MOUSE_IRQ 12

#endif // LAB4_IMOUSE_H
