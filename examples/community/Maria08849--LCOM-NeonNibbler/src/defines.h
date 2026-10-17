#ifndef _LCOM_DEFINES_H_
#define _LCOM_DEFINES_H_

#include <lcom/lcf.h>

/**
 *
 * TIMER RELATED MACROS
 *
 */

#define TIMER_FREQ 1193182                    /**< @brief clock frequency for timer in PC and AT */
#define TIMER0_IRQ 0                          /**< @brief Timer 0 IRQ line */

#define TIMER_0 0x40                          /**< @brief Timer 0 count register */
#define TIMER_1 0x41                          /**< @brief Timer 1 count register */
#define TIMER_2 0x42                          /**< @brief Timer 2 count register */
#define TIMER_CTRL 0x43                       /**< @brief Control register */

#define SPEAKER_CTRL 0x61                     /**< @brief Register for speaker control  */

#define TIMER_SEL0 0x00                       /**< @brief Control Word for Timer 0 */
#define TIMER_SEL1 BIT(6)                     /**< @brief Control Word for Timer 1 */
#define TIMER_SEL2 BIT(7)                     /**< @brief Control Word for Timer 2 */
#define TIMER_RB_CMD (BIT(7) | BIT(6))        /**< @brief Read Back Command */

#define TIMER_LSB BIT(4)                      /**< @brief Initialize Counter LSB only */
#define TIMER_MSB BIT(5)                      /**< @brief Initialize Counter MSB only */
#define TIMER_LSB_MSB (TIMER_LSB | TIMER_MSB) /**< @brief Initialize LSB first and MSB afterwards */

#define TIMER_SQR_WAVE (BIT(2) | BIT(1))      /**< @brief Mode 3: square wave generator */
#define TIMER_RATE_GEN BIT(2)                 /**< @brief Mode 2: rate generator */

#define TIMER_BCD 0x01                        /**< @brief Count in BCD */
#define TIMER_BIN 0x00                        /**< @brief Count in binary */

#define TIMER_RB_COUNT_ BIT(5)
#define TIMER_RB_STATUS_ BIT(4)
#define TIMER_RB_SEL(n) BIT((n) + 1)

/**
 *
 * KEYBOARD RELATED MACROS
 *
 */

#define IRQ_KEYBOARD 1

#define BREAK_ESC 0x81
#define MAKE_CODE BIT(7)
#define TWO_BYTES 0xE0
#define ENABLE_INT BIT(0)

#define W_MAKECODE 0x11
#define W_BREAKCODE 0x91
#define A_MAKECODE 0x1E
#define A_BREAKCODE 0x9E
#define S_MAKECODE 0x1F
#define S_BREAKCODE 0x9F
#define D_MAKECODE 0x20
#define D_BREAKCODE 0xA0

#define UP_MAKECODE 0x48
#define UP_BREAKCODE 0xC8
#define LEFT_MAKECODE 0x4B
#define LEFT_BREAKCODE 0xCB
#define DOWN_MAKECODE 0x50
#define DOWN_BREAKCODE 0xD0
#define RIGHT_MAKECODE 0x4D
#define RIGHT_BREAKCODE 0xCD

#define UP_MAKECODE 0x48
#define UP_BREAKCODE 0xC8
#define LEFT_MAKECODE 0x4B
#define LEFT_BREAKCODE 0xCB
#define DOWN_MAKECODE 0x50
#define DOWN_BREAKCODE 0xD0
#define RIGHT_MAKECODE 0x4D
#define RIGHT_BREAKCODE 0xCD

#define ENTER_MAKECODE 0x1C
#define ENTER_BREAKCODE 0x9C

#define TIMEOUT_ERROR BIT(6)
#define PARITY_ERROR BIT(7)
#define FULL_OUT_BUFFER BIT(0)
#define FULL_IN_BUFFER BIT(1)
#define MOUSE_DATA BIT(5)

#define KBC_STATUS_REG 0x64
#define KBC_IN_CMD 0x64
#define KBC_OUT_CMD 0x60
#define KBC_READ_CMD 0x20
#define KBC_WRITE_CMD 0x60

#define DELAY_US 20000
#define MAX_ATTEMPS 10

/**
 *
 * MOUSE RELATED MACROS
 *
 */

#define IRQ_MOUSE 12

#define ENABLE_INT BIT(0)
#define OK_MOUSE_DATA 0xFA
#define INVALID 0xFE
#define ERROR 0xFC

#define MOUSE_LEFT_MAKE 0x01
#define MOUSE_LEFT_BREAK 0x81
#define MOUSE_RIGHT_MAKE 0x02
#define MOUSE_RIGHT_BREAK 0x82

#define LB BIT(0)
#define RB BIT(1)
#define MB BIT(2)
#define MSB_X_DELTA BIT(4)
#define MSB_Y_DELTA BIT(5)
#define X_OVERFLOW BIT(6)
#define Y_OVERFLOW BIT(7)

#define WRITE_TO_MOUSE 0xD4
#define READ_DATA 0xEB
#define DISABLE_DATA 0xF5
#define ENABLE_DATA 0xF4
#define SET_STREAM_MODE 0xEA
#define RESEND 0xFE

/**
 *
 * VBE RELATED MACROS
 *
 */

#define TRANSPARENCY_COLOR 0x000000

#define FUNCTION_FAILED 0x01
#define FUNCTION_NOT_SUPPORTED 0x02
#define FUNCTION_INVALID 0x03

#define INDEXED_1024x768 0x105
#define DIRECT_COLOR_640x480 0x110
#define DIRECT_COLOR_800x600 0x115
#define DIRECT_COLOR_1280x1024 0x11A
#define DIRECT_COLOR_1152x864 0x14C

#define SET_VBE_MODE 0x02
#define RETURN_VBE_MODE 0x01
#define RETURN_VBE_CTRL_INFO 0x00

/**
 *
 * GAME_STATE RELATED MACROS
 *
 */

#define EXIT 0
#define MENU_SPLAY 1
#define MENU_NPLAY 2
#define GAME 3
#define POWERUP 4

#define STATIONARY 0
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

/**
 *
 * RTC RELATED MACROS
 *
 */

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define RTC_IH 8
#define RTC_MASK 5

#define RTC_REG_SEC 0
#define RTC_REG_MIN 2
#define RTC_REG_HOUR 4
#define RTC_REG_DAY 7
#define RTC_REG_MONTH 8
#define RTC_REG_YEAR 9

#define BINARY BIT(2)
#define UPDATING BIT(7)
#define RTC_REG_CTRL 10
#define RTC_REG_STA 11

#endif /* _LCOM_DEFINES_H_ */
