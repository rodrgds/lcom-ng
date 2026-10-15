/**
 * @file i8042
 * @brief Constants for programming the i8042 Keyboard
 */

#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042_Keyboard i8042_Keyboard
 * @{
 *
 * Constants for programming the i8042 Keyboard
 */

#define KBC_IRQ 1 /**< @brief Keyboard IRQ line */

#define KBC_ARG_REG 0x60 /**< @brief Address to pass the arguments of a command */
#define KBC_OUT_BUF 0x60 /**< @brief Keyboard output buffer register */
#define KBC_ST_REG  0x64 /**< @brief Keyboard status register */
#define KBC_CMD_REG 0x64 /**< @brief Register for write keyboard commands */

#define FIRST_B_SCODE   0xE0 /**< @brief First byte from the keyboard scancode with two bytes */

#define ESC_MAKECODE    0x01 /**< @brief Esc keyboard makecode */
#define ESC_BREAKCODE   0x81 /**< @brief Esc keyboard breakcode */
#define ENTER_MAKECODE  0x1C /**< @brief Enter keyboard makecode */
#define ENTER_BREAKCODE 0x9C /**< @brief Enter keyboard breakcode */
#define SPACE_MAKECODE  0x39 /**< @brief Space bar keyboard makecode */
#define SPACE_BREAKCODE 0xB9 /**< @brief Space bar keyboard breakcode */
#define DEL_MAKECODE    0x0E /**< @brief Delete keyboard makecode */
#define DEL_BREAKCODE   0x8E /**< @brief Delete keyboard breakcode */

#define UP_MAKECODE     0x48 /**< @brief Up keyboard makecode */
#define UP_BREAKCODE    0xC8 /**< @brief Up keyboard breakcode */
#define DOWN_MAKECODE   0x50 /**< @brief Down keyboard makecode */
#define DOWN_BREAKCODE  0xD0 /**< @brief Down keyboard breakcode */
#define RIGHT_MAKECODE  0x4D /**< @brief Right keyboard makecode */
#define RIGHT_BREAKCODE 0xCD /**< @brief Right keyboard breakcode */
#define LEFT_MAKECODE   0x4B /**< @brief Left keyboard makecode */
#define LEFT_BREAKCODE  0xCB /**< @brief Left keyboard breakcode */

#define ONE_MAKECODE    0x02 /**< @brief One keyboard number makecode */
#define ONE_BREAKCODE   0x82 /**< @brief One keyboard number breakcode */
#define TWO_MAKECODE    0x03 /**< @brief Two keyboard number makecode */
#define TWO_BREAKCODE   0x83 /**< @brief Two keyboard number breakcode */

#define A_MAKECODE      0x1E /**< @brief Letter A keyboard makecode */
#define A_BREAKCODE     0x9E /**< @brief Letter A keyboard breakcode */
#define B_MAKECODE      0x30 /**< @brief Letter B keyboard makecode */
#define B_BREAKCODE     0xB0 /**< @brief Letter B keyboard breakcode */
#define C_MAKECODE      0x2E /**< @brief Letter C keyboard makecode */
#define C_BREAKCODE     0xAE /**< @brief Letter C keyboard breakcode */
#define D_MAKECODE      0x20 /**< @brief Letter D keyboard makecode */
#define D_BREAKCODE     0xA0 /**< @brief Letter D keyboard breakcode */
#define E_MAKECODE      0x12 /**< @brief Letter E keyboard makecode */
#define E_BREAKCODE     0x92 /**< @brief Letter E keyboard breakcode */
#define F_MAKECODE      0x21 /**< @brief Letter F keyboard makecode */
#define F_BREAKCODE     0xA1 /**< @brief Letter F keyboard breakcode */
#define G_MAKECODE      0x22 /**< @brief Letter G keyboard makecode */
#define G_BREAKCODE     0xA2 /**< @brief Letter G keyboard breakcode */
#define H_MAKECODE      0x23 /**< @brief Letter H keyboard makecode */
#define H_BREAKCODE     0xA3 /**< @brief Letter H keyboard breakcode */
#define I_MAKECODE      0x17 /**< @brief Letter I keyboard makecode */
#define I_BREAKCODE     0x97 /**< @brief Letter I keyboard breakcode */
#define J_MAKECODE      0x24 /**< @brief Letter J keyboard makecode */
#define J_BREAKCODE     0xA4 /**< @brief Letter J keyboard breakcode */
#define K_MAKECODE      0x25 /**< @brief Letter K keyboard makecode */
#define K_BREAKCODE     0xA5 /**< @brief Letter K keyboard breakcode */
#define L_MAKECODE      0x26 /**< @brief Letter L keyboard makecode */
#define L_BREAKCODE     0xA6 /**< @brief Letter L keyboard breakcode */
#define M_MAKECODE      0x32 /**< @brief Letter M keyboard makecode */
#define M_BREAKCODE     0xB2 /**< @brief Letter M keyboard breakcode */
#define N_MAKECODE      0x31 /**< @brief Letter N keyboard makecode */
#define N_BREAKCODE     0xB1 /**< @brief Letter N keyboard breakcode */
#define O_MAKECODE      0x18 /**< @brief Letter O keyboard makecode */
#define O_BREAKCODE     0x98 /**< @brief Letter O keyboard breakcode */
#define P_MAKECODE      0x19 /**< @brief Letter P keyboard makecode */
#define P_BREAKCODE     0x99 /**< @brief Letter P keyboard breakcode */
#define Q_MAKECODE      0x10 /**< @brief Letter Q keyboard makecode */
#define Q_BREAKCODE     0x90 /**< @brief Letter Q keyboard breakcode */
#define R_MAKECODE      0x13 /**< @brief Letter R keyboard makecode */
#define R_BREAKCODE     0x93 /**< @brief Letter R keyboard breakcode */
#define S_MAKECODE      0x1F /**< @brief Letter S keyboard makecode */
#define S_BREAKCODE     0x9F /**< @brief Letter S keyboard breakcode */
#define T_MAKECODE      0x14 /**< @brief Letter T keyboard makecode */
#define T_BREAKCODE     0x94 /**< @brief Letter T keyboard breakcode */
#define U_MAKECODE      0x16 /**< @brief Letter U keyboard makecode */
#define U_BREAKCODE     0x96 /**< @brief Letter U keyboard breakcode */
#define V_MAKECODE      0x2F /**< @brief Letter V keyboard makecode */
#define V_BREAKCODE     0xAF /**< @brief Letter V keyboard breakcode */
#define X_MAKECODE      0x2D /**< @brief Letter X keyboard makecode */
#define X_BREAKCODE     0xAD /**< @brief Letter X keyboard breakcode */
#define W_MAKECODE      0x11 /**< @brief Letter W keyboard makecode */
#define W_BREAKCODE     0x91 /**< @brief Letter W keyboard breakcode */
#define Y_MAKECODE      0x15 /**< @brief Letter Y keyboard makecode */
#define Y_BREAKCODE     0x95 /**< @brief Letter Y keyboard breakcode */
#define Z_MAKECODE      0x2C /**< @brief Letter Z keyboard makecode */
#define Z_BREAKCODE     0xAC /**< @brief Letter Z keyboard breakcode */

#define MAKE_BREAK_CODE BIT(7) /**< @brief Bit distinguish makecode and breakcode */

#define KBC_ST_IBF BIT(1) /**< @brief Bit indicates input buffer full */
#define KBC_ST_OBF BIT(0) /**< @brief Bit indicates output buffer full */
#define KBC_ST_AUX BIT(5) /**< @brief Bit indicates mouse data */

#define KBC_PAR_ERR BIT(7) /**< @brief Bit indicates parity error */
#define KBC_TO_ERR  BIT(6) /**< @brief Bit indicates timeout error */

#define ENABLE_KBC_INT   BIT(0) /**< @brief Bit indicates if keyboard interrupts are enabled on OBF */
#define DISABLE_KBC_ITFC BIT(4) /**< @brief Bit indicates if keyboard iterface is disabled */

#define WAIT_KBC    20000 /**< @brief Tick delay for keyboard */

#define READ_CMDB           0x20 /**< @brief Command to read command byte */
#define WRITE_CMDB          0x60 /**< @brief Command to write command byte */
#define CHKC_KBC_CMDB       0xAA /**< @brief Command to check KBC */
#define CHCK_KBD_ITFC_CMDB  0xAB /**< @brief Command to check keyboard interface */
#define DIS_KBD_ITFC_CMDB   0xAD /**< @brief Command to disable keyboard interface */
#define ENB_KBD_ITFC_CMDB   0xAE /**< @brief Command to enable keyboard interface */

/**@}*/

#endif
