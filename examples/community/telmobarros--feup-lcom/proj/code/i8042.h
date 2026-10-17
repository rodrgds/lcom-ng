#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for i8042 KEYBOARD CONTROLLER
 */

#define BIT(n) (0x01<<(n))

//KEYBOARD VARIABLES
#define KBD_IRQ	        	1    /**< @brief Keyboard 0 IRQ line */
#define STAT_REG 			0x64
#define OUT_BUF 			0x60
#define OBF 				BIT(0)
#define PAR_ERR				BIT(7)
#define TO_ERR 				BIT(5)
#define DELAY_US    		20000
#define BREAKCODE 			0x81
#define CODETYPE 			BIT(7)
#define CHANGELEDS 			0xED
#define UPKEY_MC			0xE048
#define UPKEY_BC			0XE0C8
#define DOWNKEY_MC			0XE050
#define DOWNKEY_BC			0XE0D0
#define LEFTKEY_MC			0XE04B
#define LEFTKEY_BC			0XE0CB
#define RIGHTKEY_MC			0XE04D
#define RIGHTKEY_BC			0XE0CD
#define SPACEBAR_MC			0X39
#define SPACEBAR_BC			0XB9
#define A_MC				0X1E
#define A_BC				0X9E
#define W_MC				0X11
#define W_BC				0X91
#define S_MC				0X1F
#define S_BC				0X9F
#define D_MC				0X20
#define D_BC				0XA0

//MOUSE VARIABLES
#define MOUSE_IRQ 			12
#define IBF 				BIT(0)
#define IN_BUF	 			0x64
#define BYTETOMOUSE 		0xD4
#define ENABLE_PACKETS 		0xF4
#define LB					BIT(0)
#define RB					BIT(1)
#define MB					BIT(2)
#define ISFIRSTPACKET		BIT(3)
#define XSIGN				BIT(4)
#define YSIGN				BIT(5)
#define XOV					BIT(6)
#define YOV					BIT(7)
#define STATUS_REQUEST		0xE9
#define REMOTE				BIT(6)
#define ENABLE				BIT(5)
#define SCALING				BIT(4)
#define LEFT				BIT(2)
#define MIDDLE				BIT(1)
#define RIGHT				BIT(0)
#define RESOLUTION 			BIT(0) | BIT(1)

#define ACK 0xFA
#define NACK 0xFE
#define ERROR 0xFC

/**@}*/

#endif /* _LCOM_I8042_H */
