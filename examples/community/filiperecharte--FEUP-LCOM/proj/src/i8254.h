#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_

#include <lcom/lcf.h>

/** @defgroup i8254 i8254
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

#define TIMER_FREQ 1193182 /**< @brief clock frequency for timer in PC and AT */
#define TIMER0_IRQ 0 /**< @brief Timer 0 IRQ line */
#define KEYBOARD_IRQ 1
#define MOUSE_IRQ 12


/* I/O port addresses */

#define TIMER_0    0x40 /**< @brief Timer 0 count register */
#define TIMER_1    0x41 /**< @brief Timer 1 count register */
#define TIMER_2    0x42 /**< @brief Timer 2 count register */
#define TIMER_CTRL 0x43 /**< @brief Control register */

#define SPEAKER_CTRL 0x61 /**< @brief Register for speaker control  */

/* Timer control */

/* Timer selection: bits 7 and 6 */

#define TIMER_SEL0   0x00              /**< @brief Control Word for Timer 0 */
#define TIMER_SEL1   BIT(6)            /**< @brief Control Word for Timer 1 */
#define TIMER_SEL2   BIT(7)            /**< @brief Control Word for Timer 2 */
#define TIMER_RB_CMD (BIT(7) | BIT(6)) /**< @brief Read Back Command */

/* Register selection: bits 5 and 4 */

#define TIMER_LSB     BIT(4)                  /**< @brief Initialize Counter LSB only */
#define TIMER_MSB     BIT(5)                  /**< @brief Initialize Counter MSB only */
#define TIMER_LSB_MSB (TIMER_LSB | TIMER_MSB) /**< @brief Initialize LSB first and MSB afterwards */

/* Operating mode: bits 3, 2 and 1 */

#define TIMER_SQR_WAVE (BIT(2) | BIT(1)) /**< @brief Mode 3: square wave generator */
#define TIMER_RATE_GEN BIT(2)            /**< @brief Mode 2: rate generator */

/* Counting mode: bit 0 */

#define TIMER_BCD 0x01 /**< @brief Count in BCD */
#define TIMER_BIN 0x00 /**< @brief Count in binary */

/* READ-BACK COMMAND FORMAT */

#define TIMER_RB_COUNT_  BIT(5)
#define TIMER_RB_STATUS_ BIT(4)
#define TIMER_RB_SEL(n)  BIT((n) + 1)

#define OUT_BUF 0x60
#define IN_BUF 0x60
#define ESC_KEY 0x81
#define UP_ARROW 0x48
#define DOWN_ARROW 0x50
#define LEFT_ARROW 0x4B
#define RIGHT_ARROW 0x4D

#define COMMAND_REGISTER 0x64

#define DELAY_US 20000

#define PARITY_ERROR BIT(7)
#define TIMEOUT_ERROR BIT(6)
#define OBF BIT(0)
#define IBF BIT(0)

/////* Mouse buttons and things */////

#define Y_OVFL              BIT(7)    
#define X_OVFL              BIT(6)    
#define Y_SIGN              BIT(5)    
#define X_SIGN              BIT(4)    
#define MIDDLE_BUTTON       BIT(2)    
#define RIGHT_BUTTON        BIT(1)    
#define LEFT_BUTTON         BIT(0)    

#define TWO_BYTES 0xE0


/////* Alphabet and importante keys*/////
#define A_KEY   0x1E
#define B_KEY   0x30
#define C_KEY   0x2E
#define D_KEY   0x20
#define E_KEY   0x12
#define F_KEY   0x21
#define G_KEY   0x22
#define H_KEY   0x23
#define I_KEY   0x17
#define J_KEY   0x24
#define K_KEY   0x25
#define L_KEY   0x26
#define M_KEY   0x32
#define N_KEY   0x31
#define O_KEY   0x18
#define P_KEY   0x19
#define Q_KEY   0x10
#define R_KEY   0x13
#define S_KEY   0x1F
#define T_KEY   0x14
#define U_KEY   0x16
#define V_KEY   0x2F
#define W_KEY   0x11
#define X_KEY   0x2D
#define Y_KEY   0x15
#define Z_KEY   0x2C
#define ENTER_KEY 0x1C
#define SPACE_KEY 0x39
#define BACKSPACE_KEY 0x0E

/**@}*/

#endif /* _LCOM_I8254_H */
