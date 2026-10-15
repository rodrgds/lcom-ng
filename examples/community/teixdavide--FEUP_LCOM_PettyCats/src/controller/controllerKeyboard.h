/**
 * @file controllerKeyboard.h
 * @brief This file contains make and break code macros for the keys used in the project and a function to parse a scan code
 * @date 2024-06-02
 * 
 * 
 */

#ifndef _CONTROLLERKEYBOARD_H_
#define _CONTROLLERKEYBOARD_H_

#include "../models/Game.h"

#define W_MAKE 0x11
#define W_BREAK 0x91

#define S_MAKE 0x1f
#define S_BREAK 0x9f

#define A_MAKE 0x1e
#define A_BREAK 0x9e

#define D_MAKE 0x20
#define D_BREAK 0xa0

#define ESC_BREAK 0x81
#define E_BREAK 0x92
/**
 * @brief Parses a scan code and determines which key is currently pressed
 * 
 * @param scan_code 
 * @return always returns 0 
 */
int update_keys(uint8_t scan_code);



#endif
