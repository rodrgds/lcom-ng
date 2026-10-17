#ifndef _LCOM_VBE_H_
#define _LCOM_VBE_H_

#include <lcom/lcf.h>

/* VBE Functions */
#define VIDEO_SERVICES 0X10 /** @brief BIOS Services */
#define SET_VBE_MODE 0x4F02 /** @brief Set VBE Mode */
#define SET_VBE_DISPLAY_START 0x4F07 /** @brief Set VBE Display Start */
#define VERTICAL_RETRACE 0x80 /** @brief Vertical Retrace */

/* Auxiliary MACROS */
#define LINEAR_BIT BIT(14) /** @brief Linear Bit */
#define DIRECT 0x06 /** @brief Direct Color Mode */

/* Graphic Modes */
#define DIRECT_COLOR_WITH_32BITS 0x14C /** @brief Direct Color Mode with 32 bits */

#endif /* _LCOM_VBE_H */
