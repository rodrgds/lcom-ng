#ifndef _LCOM_MACROS_H_
#define _LCOM_MACROS_H_

#include <lcom/lcf.h>


/** @defgroup SettingsMacros SettingsMacros
 * @{
 *
 * Constants for the Settings.
 */

#define FPS 60                      /**< @brief FPS for the project*/
#define visitedEdgeColor 0x90EE90   /**< @brief Color for a visited edge the display*/
#define unvisitedEdgeColor 0xBABABA /**< @brief Color for a not visited edge*/
#define maxGraphNodes 10            /**< @brief Max number of nodes in graph*/
#define lastMission 4               /**< @brief Last mission avaliable in the game*/
/**@}*/
/** @defgroup KBCboardMacros KBCMacros
 * @{
 *
 * Constants for the KBC.
 */
#define ESC 0x81 /**< @brief MakeCode for the Esc Key */
#define ARR_UP 0x48 /**< @brief MakeCode for the Arrow Up Key */
#define ARR_RIGHT 0x4D /**< @brief MakeCode for the Arrow Right Key */
#define ARR_DOWN 0x50  /**< @brief MakeCode for the Arrow Down Key */
#define ARR_LEFT 0x4B  /**< @brief MakeCode for the Arrow Left Key */
#define ENTER 0x1C     /**< @brief MakeCode for the Enter Key */
#define M 0x32         /**< @brief MakeCode for the M Key */
#define R 0x13         /**< @brief MakeCode for the R Key */
#define FIRST_BYTE_SCAN 0xE0 /**< @brief MakeCode for the firsy byte scan */

#define KEYBOARD_IRQ 1 /**< @brief Keyboard IRQ Line Value */
#define MOUSE_IRQ 12   /**< @brief Mouse IRQ Line Value */
#define MOUSE_IRQ_PIC2 4 /**< @brief Mouse PIC2 IRQ Line Value */

#define KBC_STAT_REG 0x64 /**< @brief Status register Port */
#define KBC_CMD_REG 0x64  /**< @brief Command register Port */
#define KBC_OUT_BUF 0x60  /**< @brief Output buffer Port */
#define KBC_IN_BUF 0x60   /**< @brief Input buffer Port */

#define PAR_ERR BIT(7) /**< @brief Parity Error Bit */
#define TO_ERR BIT(6)  /**< @brief Timeout Error Bit */
#define OBF BIT(0)     /**< @brief Output buffer full Bit */
#define IBF BIT(1)     /**< @brief Input buffer full Bit */
#define AUX BIT(5)     /**< @brief Auxiliary Bit */

#define READ_COM_BYTE 0x20 /**< @brief Read command  */
#define WRITE_COM_BYTE 0x60 /**< @brief Write command */
#define WRITE_MOUSE_BYTE 0xD4 /**< @brief Write mouse byte command */
#define CHECK_KBC 0xAA      /**< @brief Check KBC command */
#define CHECK_KBD_INTERFACE 0xAB /**< @brief Check Keyboard interface command */
#define DIS_KBD_INTERFACE 0xAD   /**< @brief Disable Keyboard interface command */
#define EN_KBD_INTERFACE 0xAE    /**< @brief Enable Keyboard interface command */

#define POLL_DELAY 20000 /**< @brief Polling delay*/
#define KBC_N_TRIES 2    /**< @brief Number of tries for the KBC */

#define BREAK_CODE BIT(7) /**< @brief Break Code Bit */
#define ENABLE_INT_KBD BIT(0) /**< @brief Enable Keyboard interface Bit */
#define ENABLE_INT_MOUSE BIT(1) /**< @brief Enable mouse interrupts Bit */
#define DISABLE_MOUSE BIT(5)    /**< @brief Disable mouse Bit */
#define DISABLE_KBD_INTERFACE BIT(4) /**< @brief disable keyboard interface Bit */

#define READ_DATA 0xEB /**< @brief Read mouse data command */
#define STREAM_MODE 0xEA /**< @brief Mouse to stream mode command */
#define ENABLE_DATA_REPORT 0xF4 /**< @brief Mouse enable data report command */
#define DISABLE_DATA_REPORT 0xF5 /**< @brief Mouse disable data report command */
#define REMOTE_MODE 0xF0         /**< @brief Mouse to remote mode command */

#define LB BIT(0) /**< @brief Left mouse button Bit */
#define RB BIT(1) /**< @brief Right mouse button Bit */
#define MB BIT(2) /**< @brief Middle mouse button Bit */
#define X_OV BIT(6) /**< @brief X Overflow Bit */
#define Y_OV BIT(7) /**< @brief Y Overflow Bit */
#define X_MSB BIT(4) /**< @brief X MSB Bit */
#define Y_MSB BIT(5) /**< @brief Y MSB Bit */

#define LB_BIT 0    /**< @brief Left mouse button Bit N */
#define RB_BIT 1    /**< @brief Right mouse button Bit N */
#define MB_BIT 2     /**< @brief Middle mouse button Bit N */
#define X_OV_BIT 6   /**< @brief X Overflow Bit N */
#define Y_OV_BIT 7   /**< @brief Y Overflow Bit N */
#define X_MSB_BIT 4  /**< @brief X MSB Bit N */
#define Y_MSB_BIT 5  /**< @brief Y MSB Bit N */

#define ACK 0xFA /**< @brief Acknowlegde code */
#define NACK 0xFE /**< @brief Not acknowlegde code */
#define ERROR 0xFC /**< @brief Error code */

#define DISABLE_IRQ_LINE 0 /**< @brief Disable IRQ Line */
#define ENABLE_IRQ_LINE 1  /**< @brief Enable IRQ Line */
/**@}*/


/** @defgroup TimerMacros TimerMacros
 * @{
 *
 * Constants for programming the Timer.
 */

#define TIMER_FREQ 1193182 /**< @brief clock frequency for timer in PC and AT */
#define TIMER0_IRQ 0       /**< @brief Timer 0 IRQ line */
#define TIMER_FREQ_MIN 19  /**< @brief Timer minimum clock */
#define TIMER_DFLT_FREQ 60 /**< @brief Timer default clock */

#define TIMER_0 0x40    /**< @brief Timer 0 count register */
#define TIMER_1 0x41    /**< @brief Timer 1 count register */
#define TIMER_2 0x42    /**< @brief Timer 2 count register */
#define TIMER_CTRL 0x43 /**< @brief Control register */
#define SPEAKER_CTRL 0x61 /**< @brief Register for speaker control  */

#define TIMER_SEL0 0x00                /**< @brief Control Word for Timer 0 */
#define TIMER_SEL1 BIT(6)              /**< @brief Control Word for Timer 1 */
#define TIMER_SEL2 BIT(7)              /**< @brief Control Word for Timer 2 */
#define TIMER_RB_CMD (BIT(7) | BIT(6)) /**< @brief Read Back Command */

#define TIMER_LSB BIT(4)                      /**< @brief Initialize Counter LSB only */
#define TIMER_MSB BIT(5)                      /**< @brief Initialize Counter MSB only */
#define TIMER_LSB_MSB (TIMER_LSB | TIMER_MSB) /**< @brief Initialize LSB first and MSB afterwards */

#define TIMER_SQR_WAVE (BIT(2) | BIT(1)) /**< @brief Mode 3: square wave generator */
#define TIMER_RATE_GEN BIT(2)            /**< @brief Mode 2: rate generator */

#define TIMER_BCD 0x01 /**< @brief Count in BCD */
#define TIMER_BIN 0x00 /**< @brief Count in binary */

#define TIMER_RB_COUNT_ BIT(5) /**< @brief RB Count bit */
#define TIMER_RB_STATUS_ BIT(4) /**< @brief RB Status bit */
#define TIMER_RB_SEL(n) BIT((n) + 1) /**< @brief Select RB Bit */

/**@}*/

/** @defgroup GraphicsMacros GraphicsMacros
 * @{
 *
 * Constants for programming the Graphics.
 */
#define VBE_CALL 0x4F /**< @brief VBE call*/
#define VBE_CONTROL_INFO 0x00 /**< @brief VBE function that returns the control info*/
#define VBE_MODE_INFO 0x01    /**< @brief VBE function that returns the mode info*/
#define VBE_SET_MODE 0x02     /**< @brief VBE function that sets the color mode*/
#define VBE_INT 0x10          /**< @brief Video card interrupt*/
#define VBE_LINEAR_FRAME BIT(14)    /**< @brief Linear Frame bit*/
#define DIRECT_COLOR_888_MODE 0x115 /**< @brief Indexed color mode value*/
#define SIZEOF_BYTE 8               /**< @brief Size of a byte*/
#define SIZEOF_UINT32 8 * sizeof(uint32_t) /**< @brief Size of a uint32*/
#define CHROMA_KEY_GREEN_888 0x00b140      /**< @brief 888 Indexed mode none color*/
#define WIDTH 600                          /**< @brief Width of the display*/
#define HEIGHT 800                         /**< @brief Height the display*/
/**@}*/

#endif /* _LCOM_MACROS_H_ */
