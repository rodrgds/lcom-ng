#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @file i8042.h
 * @brief file containing the constants for the i8042 keyboard/mouse controller
 */

/** @defgroup i8042 i8042
 * @{
 *
 * @brief Constants for programming the i8042 Keyboard/Mouse Controller.
 */

#define BIT(n) 		(0x01<<(n))		/**< @brief activate only bit n */


/* hook ids and irq lines */
#define MSE_IRQ			12			/**< @brief Mouse IRQ line */
#define MSE_HOOK_ID 	12			/**< @brief hook_id for Mouse interruptions */
#define KBD_IRQ			1			/**< @brief Keyboard IRQ line */
#define KBD_HOOK_ID 	1			/**< @brief hook_id for Keyboard interruptions */

/* KBC ports */
#define KBC_STAT_REG	0x64		/**< @brief KBC status register */
#define KBC_CMD_REG		0x60		/**< @brief KBC command register */
#define KBC_OUT_BUF		0x60		/**< @brief KBC out register */

/* KBC status register */
#define KBC_OBF			BIT(0)		/**< @brief KBC output buffer full indicator */
#define KBC_IBF			BIT(1)		/**< @brief KBC input buffer full indicator */
#define KBC_SYS			BIT(2)		/**< @brief KBC system flag: 0 if system in power-on reset, 1 if system already initialized */
#define KBC_A2			BIT(3)		/**< @brief KBC A2 input line: 0 data byte, 1 command byte */
#define KBC_INH			BIT(4)		/**< @brief KBC inhibit flag: 0 if keyboard is inhibited */
#define KBC_AUX			BIT(5)		/**< @brief KBC mouse data flag */
#define KBC_TO_ERR		BIT(6)		/**< @brief KBC timeout error indicator */
#define KBC_PAR_ERR		BIT(7)		/**< @brief KBC parity error indicator */

/* mouse related KBC commands */
#define KBC_WRT_MSE		0xD4		/**< @brief KBC wryte byte to mouse command */
#define KBC_CHC_INT		0xA9		/**< @brief KBC check mouse interface command */
#define KBC_ENB_MSE		0xA8		/**< @brief KBC enable mouse command */
#define KBC_DSB_MSE		0xA7		/**< @brief KBC disable mouse command */
#define KBC_WRT_BYT		0x60		/**< @brief KBC write command byte */
#define KBC_REA_BYT		0x20		/**< @brief KBC read command byte */

/* KBC command byte */
#define KBC_INT			BIT(0)		/**< @brief KBC enable interrupt on OBF from keyboard */
#define KBC_INT2		BIT(1)		/**< @brief KBC enable interrupt on OBF from mouse */
#define KBC_DIS			BIT(4)		/**< @brief KBC disable keyboard in byte command */
#define KBC_DIS2		BIT(5)		/**< @brief KBC disable mouse in byte command */

/* KBC arguments of command 0xD4 */
#define KBC_MSE_RESET	0xFF		/**< @brief KBC mouse reset argument */
#define KBC_MSE_RESND	0xFE		/**< @brief KBC mouse resend argument */
#define KBC_MSE_SETDF	0xF6		/**< @brief KBC set defaults argument */
#define KBC_MSE_DSBSM	0xF5		/**< @brief KBC disable stream mode argument */
#define KBC_MSE_ENBDP	0xF4		/**< @brief KBC enable sending data packets argument */
#define KBC_MSE_SETSR	0xF3		/**< @brief KBC set sample rates argument */
#define KBC_MSE_SETRM	0xF0		/**< @brief KBC set remote mode argument */
#define KBC_MSE_READD	0xEB		/**< @brief KBC read data argument */
#define KBC_MSE_SETSM	0xEA		/**< @brief KBC set stream mode argument */
#define KBC_MSE_STATR	0xE9		/**< @brief KBC status request argument */
#define KBC_MSE_SETRS	0xE8		/**< @brief KBC set resolution argument */
#define KBC_MSE_SETS2	0xE7		/**< @brief KBC set scalling 2:1 argument */
#define KBC_MSE_SETS1	0xE6		/**< @brief KBC set scalling 1:1 argument */

/* KBC mouse replys */
#define KBC_MSE_ACK		0xFA		/**< @brief KBC mouse reply acknowledgment OK */
#define KBC_MSE_NACK	0xFE		/**< @brief KBC mouse reply invalid byte */
#define KBC_MSE_ERROR	0xFC		/**< @brief KBC mouse reply second consecutive invalid byte */

/* mouse packet byte 1*/
#define MSE_Y_OVFL		BIT(7)		/**< @brief mouse Y shift overflow indicator */
#define MSE_X_OVFL		BIT(6)		/**< @brief mouse X shift overflow indicator */
#define MSE_Y_SIGN		BIT(5)		/**< @brief mouse Y negative shift indicator */
#define MSE_X_SIGN		BIT(4)		/**< @brief mouse X negative shift indicator */
#define MSE_MB			BIT(2)		/**< @brief mouse middle button indicator */
#define MSE_RB			BIT(1)		/**< @brief mouse right button indicator */
#define MSE_LB			BIT(0)		/**< @brief mouse left button indicator */

#define DELAY_KBC    	20000		/**< @brief Delay for Keyboard/Mouse output */

#define DB_BYTE_CODE	0xE0		/**< @brief First byte of code with two bytes */
#define ESC_BREAK_CODE	0x81		/**< @brief Break code for ESC */
#define SPACE_MAKE_CODE 0x39		/**< @brief Make code for SPACE */
#define LSHFT_MAKE_CODE 0x2A		/**< @brief Make code for LEFT SHIFT */
#define RSHFT_MAKE_CODE 0x36		/**< @brief Make code for RIGHT SHIFT */
#define ALT_MAKE_CODE   0x38		/**< @brief Make code for ALT, right alt is the same as left but with dual code */
#define CTRL_MAKE_CODE	0x1D		/**< @brief Make code for CTRL, right ctrl is the same as left but with dual code */
#define ENTER_MAKE_CODE 0x1C		/**< @brief Make code for ENTER */
#define A_MAKE_CODE		0x1E		/**< @brief Make code for A */
#define B_MAKE_CODE		0x30		/**< @brief Make code for B */
#define C_MAKE_CODE		0x2E		/**< @brief Make code for C */
#define D_MAKE_CODE		0x20		/**< @brief Make code for D */
#define E_MAKE_CODE		0x12		/**< @brief Make code for E */
#define F_MAKE_CODE		0x21		/**< @brief Make code for F */
#define G_MAKE_CODE		0x22		/**< @brief Make code for G */
#define H_MAKE_CODE		0x23		/**< @brief Make code for H */
#define I_MAKE_CODE		0x17		/**< @brief Make code for I */
#define J_MAKE_CODE		0x24		/**< @brief Make code for J */
#define K_MAKE_CODE		0x25		/**< @brief Make code for K */
#define L_MAKE_CODE		0x26		/**< @brief Make code for L */
#define M_MAKE_CODE		0x32		/**< @brief Make code for M */
#define N_MAKE_CODE		0x31		/**< @brief Make code for N */
#define O_MAKE_CODE		0x18		/**< @brief Make code for O */
#define P_MAKE_CODE		0x19		/**< @brief Make code for P */
#define Q_MAKE_CODE		0x10		/**< @brief Make code for Q */
#define R_MAKE_CODE		0x13		/**< @brief Make code for R */
#define S_MAKE_CODE		0x1F		/**< @brief Make code for S */
#define T_MAKE_CODE		0x14		/**< @brief Make code for T */
#define U_MAKE_CODE		0x16		/**< @brief Make code for U */
#define V_MAKE_CODE		0x2F		/**< @brief Make code for V */
#define W_MAKE_CODE		0x11		/**< @brief Make code for W */
#define X_MAKE_CODE		0x2D		/**< @brief Make code for X */
#define Y_MAKE_CODE		0x15		/**< @brief Make code for Y */
#define Z_MAKE_CODE		0x2C		/**< @brief Make code for Z */
#define N1_MAKE_CODE	0x02		/**< @brief Make code for 1 */
#define N2_MAKE_CODE	0x03		/**< @brief Make code for 2 */
#define N3_MAKE_CODE	0x04		/**< @brief Make code for 3 */
#define N4_MAKE_CODE	0x05		/**< @brief Make code for 4 */
#define N5_MAKE_CODE	0x06		/**< @brief Make code for 5 */
#define N6_MAKE_CODE	0x07		/**< @brief Make code for 6 */
#define N7_MAKE_CODE	0x08		/**< @brief Make code for 7 */
#define N8_MAKE_CODE	0x09		/**< @brief Make code for 8 */
#define N9_MAKE_CODE	0x01		/**< @brief Make code for 9 */

#define MSE_ATTEMPTS	3			/**< @brief Number of attempts in case of mouse error */


/**@}*/

#endif /* _LCOM_I8042_H */
