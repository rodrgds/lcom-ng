#ifndef __I8042_H
#define __I8042_H
/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard.
 */
#define OUT_BUF_FULL 0x01
#define DELAY_US 20000 /**< @brief given delay to get keyboard data */
#define ESC 0x81	/**< @brief ESC key code*/
#define OUT_BUF 0x60	/**< @brief control word for OUT_BUF*/
#define IN_BUF 0x60		/**< @brief control word for IN_BUF*/
#define STAT_REG 0x64	/**< @brief control word for STAT_REG*/
#define KBC_CMD_REG 0x64	/**< @brief control word for KBC_CMD_REG*/
#define BIT(n) (0x01<<(n))
#define PAR_ERR BIT(7)
#define TO_ERR  BIT(5)
#define OBF BIT(0)
#define TWO_BYTES 0xe0 /**< @brief control word for TWO_BYTES data*/
#define IBF BIT(0)
#define ACK 0xFA	/**< @brief control word for ACK*/
#define RESEND 0xFE	/**< @brief control word for RESEND*/
#define ERROR 0xFC	/**< @brief control word for ERROR*/

#endif

/**@}*/
