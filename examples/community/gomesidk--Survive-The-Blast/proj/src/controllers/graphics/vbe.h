#ifndef _VBE_H
#define _VBE_H

#include <lcom/lcf.h>

/**
 * @file vbe.h
 * @brief VBE (VESA BIOS Extensions) management
 *  
 * This module provides functions to interact with the VBE, including setting video modes, getting mode information, and handling frame buffers.
 */

/* VBE video mode */
#define VBE_1024p_INDEXED 0x107
#define VBE_768p_INDEXED 0x105
#define VBE_480p_DC 0x110
#define VBE_600p_DC 0x115
#define VBE_1024p_DC 0x11B
#define VBE_768p_DC 0x118
#define VBE_864p_DC 0x14C

/* Frame buffer*/
#define VBE_LINEAR_FB BIT(14)

/* VBE Color Mode*/
#define DIRECT_MODE 0x06
#define INDEXED_MODE 0x04

/* BIOS*/
#define BIOS_SET_VIDEOMODE 0x00
#define BIOS_VIDEOCARD_SERV 0x10

/**@}*/

#endif
