#ifndef _LCOM_IVBE_H_
#define _LCOM_IVBE_H_

#include <lcom/lcf.h>

/**
 * @defgroup ivbe iVBE
 * @ingroup graphics
 * @{
 *
 * @brief Constants for the VESA BIOS Extensions (VBE).
 */

/** @brief VBE video mode identifier for the graphics mode used in the project. */
#define VG_MODE 0x14c

/** @brief VBE function call failure status code. */
#define AH_FAIL 0x01

/** @brief VBE function call unsupported status code. */
#define AH_NOSUPPORT 0x02

/** @brief VBE function call invalid status code. */
#define AH_INVALID 0x03


/** @brief VBE function identifier. */
#define AH_PORT 0x4F

/** @brief Identifier for setting text mode in VBE. */
#define AL_TEXTMODE 0x03

/** @brief Identifier for setting VBE mode. */
#define AL_VBEMODE_SET 0x02

/** @brief Flag for enabling linear framebuffer in VBE. */
#define BX_LINEAR_FRAME_BUFF BIT(14)

/** @brief VBE function identifier for display start control. */
#define AL_DISPLAY_START_CONTROL 0x07

/** @brief Sub-function identifier for setting display start in VBE. */
#define BL_SET_DISPLAY_START 0x00

/** @brief VBE mode identifier for direct color mode. */
#define DIRECT_COLOR 0x06

/** @brief VBE mode identifier for indexed color mode. */
#define INDEXED_COLOR 0x105

/** @} */

#endif /* _LCOM_IVBE_H */
