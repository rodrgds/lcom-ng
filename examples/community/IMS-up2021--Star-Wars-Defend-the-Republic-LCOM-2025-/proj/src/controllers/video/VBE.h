#ifndef __VBE_MACROS_H
#define __VBE_MACROS_H

#include <lcom/lcf.h> // It's good practice to include this if BIT() or other lcf macros might be used, though not strictly necessary for this specific set of defines if BIT() is self-defined or not used. Given BIT(14) is used, it's necessary.

/**
 * @file vbe_macros.h
 * @brief Constants and macros related to VBE (VESA BIOS Extensions) for graphics mode programming.
 */

/** @defgroup vbe VBE Graphics Interface
 * @{
 *
 * Constants and macros related to VBE (VESA BIOS Extensions) for graphics mode programming.
 * This includes VBE mode numbers, function calls, memory models,
 * and other relevant constants for setting up and using VBE graphics modes.
 */

/* VBE Graphics Mode Numbers */
/* Predefined VBE mode numbers for common resolutions and color depths. */
#define VBE_768p_INDEXED    0x105   /**< @brief VBE Mode: 1024x768, 8-bit Indexed Color. */
#define VBE_480p_DC         0x110   /**< @brief VBE Mode: 640x480, Direct Color. */
#define VBE_600p_DC         0x115   /**< @brief VBE Mode: 800x600, Direct Color. */
#define VBE_1024p_DC        0x11A   /**< @brief VBE Mode: 1280x1024, Direct Color. */
#define VBE_864p_DC         0x14C   /**< @brief VBE Mode: 1152x864, Direct Color (e.g., 24-bit). */

/* VBE Function Calls and Flags */
/* Macros for VBE function calls and mode setting flags. */
#define VBE_MODE_SET        0x4F02  /**< @brief VBE Function 02h: Set VBE Mode (AH=4Fh, AL=02h). BX contains mode. */
#define VBE_LINEAR_FB       BIT(14) /**< @brief Bit 14 for VBE mode number in BX: Use Linear Frame Buffer model. */

/* VBE Memory Models (as per VBE ModeInfoBlock MemoryModel field) */
#define DIRECT_COLOR        0x06    /**< @brief VBE Memory Model: Direct Color (RGB). Value for ModeInfoBlock.MemoryModel. */
#define INDEXED_COLOR       0x105   /**< @brief VBE Memory Model Identifier: This is actually mode 0x105, which is an indexed color mode. A more general identifier for indexed memory model is 0x04 (Packed Pixel). */

/* BIOS Video Services (INT 0x10) */
#define BIOS_SET_VIDEOMODE  0x00    /**< @brief BIOS Video Service (INT 0x10), Subfunction 00h: Set Video Mode (AH=00h). */
#define BIOS_VIDEOCARD_SERV 0x10    /**< @brief BIOS Interrupt number for Video Card Services (INT 0x10). */

/* Color and XPM Definitions */
#define TRANSPARENT_COLOR   0x000000/**< @brief Color value often used for transparency (black in direct color). In indexed mode, it's an index. */
#define XPM_DIRECT          1       /**< @brief Flag or identifier possibly indicating an XPM image uses direct color or is for a direct color mode. */

/**@}*/

#endif /* __VBE_MACROS_H */
