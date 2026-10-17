/**
 * @file macros.h
 * @brief Hardware-specific macros and constants
 * 
 * This header file contains various macros and constants for hardware
 * devices including timer, keyboard, mouse, and video card configurations.
 */

#ifndef _MACROS_H_
#define _MACROS_H_

#include <lcom/lcf.h>

// Timer-related constants
#define CONTROL_REG 0x43        // Timer control register address
#define TIMER_0 0x40            // Timer 0 register address
#define MIN_FREQ 19             // Minimum timer frequency
#define CLOCK_FREQ 1193182      // Timer clock frequency

// Keyboard-related constants
#define ESC_BREAK 0x01          // Escape key break code
#define KBD_IRQ 1               // Keyboard IRQ line
#define KBC_ST_REG 0x64         // Keyboard controller status register
#define KBC_IN_BUF 0x60         // Keyboard controller input buffer
#define KBC_CMD_REG 0x64        // Keyboard controller command register
#define KBC_OUT_BUF 0x60        // Keyboard controller output buffer
#define KBC_PAR_ERR BIT(7)      // Parity error bit
#define KBC_TO_ERR BIT(6)       // Timeout error bit
#define KBC_OBF BIT(0)          // Output buffer full bit
#define BREAK_CODE BIT(7)       // Break code identifier bit
#define KBC_IBF BIT(1)          // Input buffer full bit
#define RCB 0x20                // Read command byte
#define WCB 0x60                // Write command byte

// Mouse-related constants
#define MOUSE_IRQ 12            // Mouse IRQ line
#define WB_TO_MOUSE 0xD4        // Write byte to mouse command
#define ENABLE_DR 0xF4          // Enable data reporting command
#define DISABLE_DR 0xF5         // Disable data reporting command
#define MOUSE_DATA BIT(5)       // Mouse data bit identifier
#define ACK 0xFA                // Acknowledgment byte

// Video/Graphics-related constants
#define BIO_CALL 0x4F           // BIOS call number for VBE
#define VBE_MODE 0x02           // VBE mode function
#define LINEAR_FB BIT(14)       // Linear frame buffer bit
#define VBE_INTNO 0x10          // VBE interrupt number
#define INDEXED_MODE 0x105      // Indexed color mode
#define DIRECT_800X600_MODE 0X115 // Direct color 800x600 mode
#define START_OF_DISPLAY 0x07   // Start of display constant

#endif
