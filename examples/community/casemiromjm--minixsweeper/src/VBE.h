/// @file VBE.h
/// @brief Macros related to VBE (VESA BIOS Extensions) video modes and operations.

#ifndef __VBE_MACROS_H
#define __VBE_MACROS_H

/// @brief VBE mode: 1024x768, 8-bit indexed color
#define VBE_768p_INDEXED 0x105

/// @brief VBE mode: 640x480, direct color
#define VBE_480p_DC 0x110

/// @brief VBE mode: 800x600, direct color
#define VBE_600p_DC 0x115

/// @brief VBE mode: 1280x1024, direct color
#define VBE_1024p_DC 0x11A

/// @brief VBE mode: 1152x864, direct color
#define VBE_864p_DC 0x14C

/// @brief VBE function: set video mode
#define VBE_MODE_SET 0x4F02

/// @brief Bit to enable linear framebuffer
#define VBE_LINEAR_FB BIT(14)

/// @brief VBE memory model: direct color
#define DIRECT_COLOR 0x06

/// @brief VBE memory model: indexed color
#define INDEXED_COLOR 0x105

/// @brief BIOS interrupt function to set video mode
#define BIOS_SET_VIDEOMODE 0x00

/// @brief BIOS video services interrupt number
#define BIOS_VIDEOCARD_SERV 0x10

#endif /* __VBE_MACROS_H */
