#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/*!< @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard and Mouse.
 */


#define KBC_CMD_REG   0x64    /*!< Command Register */
#define KBC_STAT_REG  0x64    /*!< Status Register  */
#define KBC_OUT_BUF   0x60    /*!< Output Buffer    */
#define KBC_IN_BUF    0x60    /*!< Input buffer     */


#define READ_COMMAND_BYTE 0X20      /*!<Returns Command Byte */
#define WRITE_COMMAND_BYTE 0x60     /*!<Takes A: Command Byte */
#define CHECK_KBC 0xAA              /*!<Returns 0x55, if OK || Returns 0xFC, if error */
#define CHECK_KEYBOARD_INTEFACE 0xAB/*!<Returns 0, if OK */
#define DISABLE_KBD_INTERFACE 0xAD  /*!<Disables KBD Interface */
#define ENABLE_KBD_INTERFACE 0xAE   /*!<Enables KBD Interface */


#define ESC_BREAK_CODE 0x81/*!<Escape Break Code */
#define KBC_IRQ 0x01       /*!<Sets policy for KBD interrupt request */
#define KBC_DELAY_US 20000 /*!<20 ms of timer delay */

#define KBC_PARITY BIT(7)  /*!<Parity error - invalid data */
#define KBC_TIMEOUT BIT(6) /*!<Timeout error - invalid data */
#define KBC_AUX BIT(5)     /*!<Mouse data */
#define KBC_INH BIT(4)     /*!<Inhibit flag: 0 if keyboard is inhibited */
#define KBC_A2 BIT(3)      /*!<A2 input line: 0 data byte 1 command byte */
#define KBC_SYS BIT(2)     /*!<System flag: 0 if system in power-on reset, 1 if system already initialized */
#define KBC_IBF BIT(1)     /*!<Input buffer full - Don’t write commands or arguments */
#define KBC_OBF BIT(0)     /*!<Output buffer full - data available for reading */

//PS/2 Mouse
#define MOUSE_IRQ 12      /*!<Mouse IRQ line */

//Byte 1
#define Y_OVERFLOW BIT(7)         /*!< X axis overflow */
#define X_OVERFLOW BIT(6)         /*!< Y axis overflow */
#define MSB_Y_DELTA BIT(5)        /*!< Mouse x axis movement */
#define MSB_X_DELTA BIT(4)        /*!< Mouse y axis movement */
#define ALWAYS_1 BIT(3)           /*!< This bit must always be 1 to prevent errors */
#define MIDDLE_MOUSE_BUTTON BIT(2)/*!< Press middle mouse button */
#define RIGHT_MOUSE_BUTTON BIT(1) /*!< Press right mouse button */
#define LEFT_MOUSE_BUTTON BIT(0)  /*!< Press left mouse button */

#define MOUSE_BYTE 0xD4           /*!< Mouse Byte Macro */
//Commands passed as arguments of commmand 0xD4
//Response to these commands, if any, are put in the KBC's OUT_BUF and should be read via port 0x60
#define MOUSE_RESET 0XFF                /*!<Mouse reset */
#define MOUSE_RESEND 0XFE               /*!<For serial communication errors */
#define MOUSE_SET_DEFAULTS 0XF6         /*!<Set default values */
#define MOUSE_DISABLE_REPORTING 0XF5    /*!<In stream mode, should be sent before any other command */
#define MOUSE_ENABLE_REPORTING 0XF4     /*!<In stream mode only */
#define MOUSE_SET_SAMPLE_RATE 0XF3      /*!<Sets state sampling rate */
#define MOUSE_SET_REMOTE_MODE 0XF0      /*!<Send data on request only */
#define MOUSE_READ_DATA 0XEB            /*!<Send data packet request */
#define MOUSE_SET_STREAM_MODE 0xEA      /*!<Send data on events */
#define MOUSE_STATUS_REQUEST 0XE9       /*!<Get mouse configuration(3 bytes) */
#define MOUSE_SET_RESOLUTION 0XE8       /*!<Sets mouse resolution */
#define MOUSE_SET_SCALING_21 0XE7       /*!<Acceleration mode */
#define MOUSE_SET_sACLING_11 0XE6       /*!<Linear mode */
#define MOUSE_ACK 0xFA                  /*!<Mouse Acknowledge */

//Macros para todo o alfabeto
//Makecodes
#define Q_MAKECODE 0X10 /*!< Makecode for letter Q*/
#define W_MAKECODE 0X11 /*!< Makecode for letter W*/
#define E_MAKECODE 0X12 /*!< Makecode for letter E*/
#define R_MAKECODE 0X13 /*!< Makecode for letter R*/
#define T_MAKECODE 0X14 /*!< Makecode for letter T*/
#define Y_MAKECODE 0X15 /*!< Makecode for letter Y*/
#define U_MAKECODE 0X16 /*!< Makecode for letter U*/
#define I_MAKECODE 0X17 /*!< Makecode for letter I*/
#define O_MAKECODE 0X18 /*!< Makecode for letter O*/
#define P_MAKECODE 0X19 /*!< Makecode for letter P*/
#define A_MAKECODE 0X1E /*!< Makecode for letter A*/
#define S_MAKECODE 0x1F /*!< Makecode for letter S*/
#define D_MAKECODE 0x20 /*!< Makecode for letter D*/
#define F_MAKECODE 0x21 /*!< Makecode for letter F*/
#define G_MAKECODE 0x22 /*!< Makecode for letter G*/
#define H_MAKECODE 0x23 /*!< Makecode for letter H*/
#define J_MAKECODE 0X24 /*!< Makecode for letter J*/
#define K_MAKECODE 0X25 /*!< Makecode for letter K*/
#define L_MAKECODE 0X26 /*!< Makecode for letter L*/
#define Z_MAKECODE 0X2C /*!< Makecode for letter Z*/
#define X_MAKECODE 0X2D /*!< Makecode for letter X*/
#define C_MAKECODE 0X2E /*!< Makecode for letter C*/
#define V_MAKECODE 0X2F /*!< Makecode for letter V*/
#define B_MAKECODE 0X30 /*!< Makecode for letter B*/
#define N_MAKECODE 0X31 /*!< Makecode for letter N*/
#define M_MAKECODE 0X32 /*!< Makecode for letter M*/

//BREAKCODES
#define Q_BREAKCODE 0X90 /*!< Breakcode for letter Q*/
#define W_BREAKCODE 0X91 /*!< Breakcode for letter W*/
#define E_BREAKCODE 0X92 /*!< Breakcode for letter E*/
#define R_BREAKCODE 0X93 /*!< Breakcode for letter R*/
#define T_BREAKCODE 0X94 /*!< Breakcode for letter T*/
#define Y_BREAKCODE 0X95 /*!< Breakcode for letter Y*/
#define U_BREAKCODE 0X96 /*!< Breakcode for letter U*/
#define I_BREAKCODE 0X97 /*!< Breakcode for letter I*/
#define O_BREAKCODE 0X98 /*!< Breakcode for letter O*/
#define P_BREAKCODE 0X99 /*!< Breakcode for letter P*/
#define A_BREAKCODE 0X9E /*!< Breakcode for letter A*/
#define S_BREAKCODE 0x9F /*!< Breakcode for letter S*/
#define D_BREAKCODE 0xA0 /*!< Breakcode for letter D*/
#define F_BREAKCODE 0xA1 /*!< Breakcode for letter F*/
#define G_BREAKCODE 0xA2 /*!< Breakcode for letter G*/
#define H_BREAKCODE 0xA3 /*!< Breakcode for letter H*/
#define J_BREAKCODE 0XA4 /*!< Breakcode for letter J*/
#define K_BREAKCODE 0XA5 /*!< Breakcode for letter K*/
#define L_BREAKCODE 0XA6 /*!< Breakcode for letter L*/
#define Z_BREAKCODE 0XAC /*!< Breakcode for letter Z*/
#define X_BREAKCODE 0XAD /*!< Breakcode for letter X*/
#define C_BREAKCODE 0XAE /*!< Breakcode for letter C*/
#define V_BREAKCODE 0XAF /*!< Breakcode for letter V*/
#define B_BREAKCODE 0XB0 /*!< Breakcode for letter B*/
#define N_BREAKCODE 0XB1 /*!< Breakcode for letter N*/
#define M_BREAKCODE 0XB2 /*!< Breakcode for letter M*/

/*!<@}*/

#endif /* _LCOM_I8042_H */
