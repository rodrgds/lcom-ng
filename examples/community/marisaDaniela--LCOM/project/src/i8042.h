#ifndef _I8042_H_
#define _I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 
 */
#define BREAK_CODE_BIT 		      BIT(7)      /**< @brief break code bit  */

#define ESC					0x81  /**< @brief break code ESC key */

#define SNAKE_W				0x91  /**< @brief break code W key */
#define SNAKE_S				0x9f  /**< @brief break code S key */
#define SNAKE_A				0x9e  /**< @brief break code A key */
#define SNAKE_D				0xa0  /**< @brief break code D key */
#define SPACE 				0xb9  /**< @brief break code SPACE key */
#define ENTER			  	0x9c  /**< @brief break code ENTER key */
#define P				  	0x99  /**< @brief break code P key */


// KBC REGISTERS

#define STAT_REG 			      0x64  /**< @brief stat reg */
#define CMD_REG 			      0x64  /**< @brief command reg */
#define KBC_CMD_REG 		      0x64  /**< @brief kbc command reg */
#define IN_BUF 				0x60  /**< @brief Input buffer */
#define OUT_BUF		 		0x60  /**< @brief Output buffer */

#define KBD_IRQ				0x01 /**< @brief Keyboard IRQ  */

#define OUT_BUF_FULL 		      BIT(0) /**< @brief Output buffer full */
#define IN_BUF_FULL 		      BIT(1) /**< @brief Input buffer full */
#define AUX 				BIT(5) /**< @brief Aux  */

#define DELAY				20000 /**< Delay */

// MOUSE

/**
Stream Mode The mouse sends the data packet at a (programmable)
maximum fixed rate to the KBC, as determined by a mouse event
i.e. mouse movements and changes in buttons state

Remote Mode The mouse sends data packets only upon request of
the KBC*/

#define MOUSE_IRQ 			12

#define ACK					0xFA  /**< @brief if everything OK */ 
#define NACK 				0xFE 	/**< @brief if invalid */
#define ERROR 				0xFC 	/**<@brief second consecutive invalid byte */

// PS/2 Mouse Commands

#define STREAM_ON 			0xF4 /**< @brief Stream on*/ 
#define STREAM_MODE_DISABLE		0xF5 /**< @brief Disable stream mode */ 
#define STREAM_MODE_ENABLE		0xF4 /**< @brief Enable stream mode */ 
#define SET_STREAM_MODE			0xEA /**< @brief Set stream mode */ 
#define STATUS_REQUEST 			0xE9 /**< @brief Status request */ 

#define WRITE_TO_MOUSE 			0xD4 /**< @brief Write to mouse command */ 
#define MOUSE_ENABLE			0xA8 /**< @brief Enable mouse */ 
#define MOUSE_DISABLE 			0xA7 /**< @brief Disable mouse */ 

// Computer Labs: The PS/2 Mouse (slide 4)

#define MOUSE_LB 			      BIT(0) /**< @brief Mouse left button */ 
#define MOUSE_RB 			      BIT(1) /**< @brief Mouse right button */ 
#define MOUSE_MB 			      BIT(2) /**< @brief Mouse middle button */ 
#define MOUSE_UNUSED 			BIT(3) /**< @brief Mouse unused */ 
#define MOUSE_X_SIGN 			BIT(4) /**< @brief Mouse X sign */ 
#define MOUSE_Y_SIGN 			BIT(5) /**< @brief Mouse Y sign  */ 
#define MOUSE_X_OVF 			BIT(6) /**< @brief Mouse X overflow */ 
#define MOUSE_Y_OVF 			BIT(7) /**< @brief Mouse Y overflow  */ 

#endif /* _I8042_H_ */
