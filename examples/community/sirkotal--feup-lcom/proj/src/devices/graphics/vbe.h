#ifndef _LCOM_VBE_H_
#define _LCOM_VBE_H_

#define BIOS_VIDEO_SERVICES           0x10 /**< @brief BIOS video services */

/* VBE Functions */
#define VBE_FUNCTION_CALL             0x4F /**< @brief VBE function call */
#define VBE_FUNCTION_SUCCESS          0x00 /**< @brief VBE function succeeded */
#define VBE_FUNCTION_FAIL             0x01 /**< @brief VBE function failed */
#define VBE_FUNCTION_NOT_SUPPORTED    0x02 /**< @brief VBE function not supported in current HW configuration */
#define VBE_FUNCTION_INVALID_MODE     0x03 /**< @brief VBE function invalid in current video mode */

/* VBE Modes */
#define VBE_LINEAR_FRAME_BUFFER       BIT(14) /**< @brief VBE linear frame buffer bit */
#define VBE_CONTROLLER_INFO           0x00 /**< @brief VBE get controller info */
#define VBE_MODE_INFO                 0x01 /**< @brief VBE get mode info */
#define VBE_SET_MODE                  0x02 /**< @brief VBE set mode */
#define VBE_STANDART_CGA              0x03 /**< @brief VBE standart CGA mode */

/* VBE Submodes */
#define VBE_768p_INDEXED              0x105 /**< @brief VBE 768p indexed mode */
#define VBE_480p_DIRECT_COLOR         0x110 /**< @brief VBE 480p direct color mode */
#define VBE_600p_DIRECT_COLOR         0x115 /**< @brief VBE 600p direct color mode */
#define VBE_1024p_DIRECT_COLOR        0x11A /**< @brief VBE 1024p direct color mode */
#define VBE_864p_DIRECT_COLOR         0x14C /**< @brief VBE 864p direct color mode */

#define VBE_DIRECT_COLOR_MODEL        0x06 /**< @brief VBE direct color model */
// TODO: confirm
#define VBE_INDEXED_COLOR_MODEL       0x105 /**< @brief VBE indexed color model */

#endif // _LCOM_VBE_H
