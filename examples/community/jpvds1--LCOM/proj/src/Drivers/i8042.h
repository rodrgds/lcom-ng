/**
@file i8042.h
@brief Constants and macros for programming the i8042 Keyboard Controller. */

#ifndef LAB3_I8042_H
#define LAB3_I8042_H

/** 
@defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard Controller (and useful i8254 Timer constants).
 * */

#define KBC_BRK_ESC_KEY 0x81 /**< @brief scancode (breakcode) of ESC key*/
#define KBC_W_MAKE_KEY 0x11 /**< @brief Scan code for the 'W' key pressed. */
#define KBC_W_BREAK_KEY 0x91 /**< @brief Scan code for the 'W' key released. */
#define KBC_D_MAKE_KEY 0x20 /**< @brief Scan code for the 'D' key pressed. */
#define KBC_D_BREAK_KEY 0xa0 /**< @brief Scan code for the 'D' key released. */
#define KBC_A_MAKE_KEY 0x1e /**< @brief Scan code for the 'A' key pressed. */
#define KBC_A_BREAK_KEY 0x9e /**< @brief Scan code for the 'A' key released. */
#define KBC_S_MAKE_KEY 0x1f /**< @brief Scan code for the 'S' key pressed. */
#define KBC_S_BREAK_KEY 0x9f /**< @brief Scan code for the 'S' key released. */
#define KBC_B_MAKE_KEY 0x30 /**< @brief Scan code for the 'B' key pressed. */
#define KBC_B_BREAK_KEY 0xb0 /**<@brief Scan code for the 'B' key released. */
#define KBC_E_MAKE_KEY 0x12 /**<@brief Scan code for the 'E' key pressed. */
#define KBC_E_BREAK_KEY 0x92 /**<@brief Scan code for the 'E' key released. */
#define KBC_Q_MAKE_KEY 0x10 /**<@brief Scan code for the 'Q' key pressed. */
#define KBC_Q_BREAK_KEY 0x90 /**<@brief Scan code for the 'Q' key released. */
#define KBC_SPACE_MAKE_KEY 0x39 /**<@brief Scan code for the 'SPACE' key pressed. */
#define KBC_SPACE_BREAK_KEY 0xb9 /**<@brief Scan code for the 'SPACE' key released. */
#define KBC_ENTER_MAKE_KEY 0x1c /**<@brief Scan code for the 'ENTER' key pressed. */
#define KBC_ENTER_BREAK_KEY 0x9c /**<@brief Scan code for the 'ENTER' key released. */
#define KBC_MSB_SCNCD BIT(7) /**< @brief scancode diff (make or break) in BIT 7*/
#define TIMER_FREQ 1193182 /**< @brief clock frequency for timer in PC and AT */
#define TIMER0_IRQ 0 /**< @brief Timer 0 IRQ line */
#define KBC_IRQ 1 /**< @brief KBC IRQ line */

/* I/O port addresses */

#define TIMER_0    0x40 /**< @brief Timer 0 count register */
#define KBC_STAT_REG 0x64 /**< @brief KBC status register */
#define KBC_IN_BUF_CMD 0x64 /**< @brief KBC input buffer for commands */
#define KBC_IN_BUF_ARG 0x60 /**< @brief KBC input buffer for arguments */
#define KBC_OUT_BUF 0x60 /**< @brief KBC output buffer */

/* KBC Control */

/* Status Register Specification */

#define KBC_PARITY BIT(7) /**< @brief KBC parity error bit */
#define KBC_TIMEOUT BIT(6) /**< @brief KBC timeout error bit */
#define KBC_AUX BIT(5) /**< @brief KBC mouse data bit */
#define KBC_INH BIT(4) /**< @brief KBC inhibit flag bit */
#define KBC_A2 BIT(3) /**< @brief KBC a2 input line bit */
#define KBC_SYS BIT(2) /**< @brief KBC system flag bit */
#define KBC_IBF BIT(1) /**< @brief KBC input buffer full bit */
#define KBC_OBF BIT(0) /**< @brief KBC output buffer full bit */

/* KBC Commands for PC-AT/PS2 */

#define KBC_R_CMD_B 0x20 /**< @brief KBC command to read command byte */
#define KBC_W_CMD_B 0x60 /**< @brief KBC command to write command byte */
#define KBC_CHK 0xAA /**< @brief KBC command to self-check */
#define KBC_CHK_KBD_I 0xAB /**< @brief KBC command to check keyboard interface */
#define KBC_DIS_KBD_I 0XAD /**< @brief KBC command to disable KBD interface */
#define KBC_EN_KBD_I 0XAE /**< @brief KBC command to enable KBD interface */

/* KBC Command Byte */

#define KBC_DIS2 BIT(5) /**< @brief disable mouse interface */
#define KBC_DIS BIT(4) /**< @brief disable keyboard interface */
#define KBC_INT2 BIT(1) /**< @brief enable interrupt on OBF, from mouse */
#define KBC_INT BIT(0) /**< @brief enable interrupt on OBF, from keyboard */

/* KBC scancode type */

#define KBC_2BYTE_CODE 0xE0 /**< @brief the first byte of a 2 byte scancode*/

#endif //LAB3_I8042_H




