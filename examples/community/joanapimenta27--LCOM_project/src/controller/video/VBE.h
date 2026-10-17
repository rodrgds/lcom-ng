/**
 * @file VBE.h
 * @brief Header file containing VBE macros and constants.
 */

#ifndef __VBE_MACROS_H
#define __VBE_MACROS_H

#define VBE_768p_INDEXED 0x105 /**< VBE video mode for 768p indexed color */
#define VBE_480p_DC 0x110 /**< VBE video mode for 480p direct color */
#define VBE_600p_DC 0x115 /**< VBE video mode for 600p direct color */
#define VBE_1024p_DC 0x11A /**< VBE video mode for 1024p direct color */
#define VBE_864p_DC 0x14C /**< VBE video mode for 864p direct color */

#define VBE_MODE_SET 0x4F02 /**< VBE mode set function */
#define VBE_LINEAR_FB BIT(14) /**< VBE linear framebuffer */

#define DIRECT_COLOR 0x06 /**< Direct color mode */
#define INDEXED_COLOR 0x105 /**< Indexed color mode */

#define BIOS_SET_VIDEOMODE 0x00 /**< BIOS video mode set service */
#define BIOS_VIDEOCARD_SERV 0x10 /**< BIOS video card service */

#endif /* __VBE_MACROS_H */
