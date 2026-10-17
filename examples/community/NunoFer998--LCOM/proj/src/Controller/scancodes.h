/**
 * @file scancodes.h
 * @brief Keyboard scan code definitions
 * 
 * This header file defines the scan codes for various keyboard keys
 * used throughout the minesweeper game for input handling.
 */

#ifndef _SCANCODE_H_
#define _SCANCODE_H_

// Special scan codes
#define TWO_BYTES 0xE0    // Indicates a two-byte scan code sequence

// Arrow key scan codes (second byte of two-byte sequences)
#define M_UP 0X48         // Up arrow key scan code
#define M_DOWN 0x50       // Down arrow key scan code

// Letter key scan codes
#define M_W 0x11          // W key scan code (alternative up movement)
#define M_S 0x1f          // S key scan code (alternative down movement)

// Control key scan codes
#define B_ENTER 0X1c      // Enter key scan code
#define B_ESC 0x81        // Escape key scan code (break code)

#endif
