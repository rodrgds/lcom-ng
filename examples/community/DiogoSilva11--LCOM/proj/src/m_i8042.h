#ifndef _LCOM_M_I8042_H_
#define _LCOM_M_I8042_H_

#include <lcom/lcf.h>

/** @defgroup m_i8042 m_i8042
 * @{
 * 
 * Constants for programming the KBC controller (i8042).
 */

#define MOUSE_IRQ        12      /**< @brief Keyboard IRQ line */
#define DELAY_US       25000  /**< @brief Keyboard responds to a command in 20 ms */

/* KBC Port Adresses */

#define KBC_ST_REG  0x64 /**< @brief Status register (for reading the KBC state) */
#define KBC_CMD_REG 0x64 /**< @brief Address used to write KBC commands */
#define KBC_CMD_ARG 0x60 /**< @brief Address used to pass arguments of KBC commands */
#define KBC_OUT_BUF 0x60 /**< @brief Output buffer register (packet bytes or commands return values) */

/* Status Register */

#define KBC_ST_PAR     BIT(7)            /**< @brief Parity error - invalid data */
#define KBC_ST_TIME    BIT(6)            /**< @brief Timeout error - invalid data */
#define KBC_ST_ERROR   (BIT(7) | BIT(6)) /**< @brief Error in communication */
#define KBC_ST_AUX     BIT(5)            /**< @brief Mouse data (1-TRUE)(0-FALSE)*/
#define KBC_ST_INH     BIT(4)            /**< @brief Inhibit flag: 0 if keyboard is inhibited */
#define KBC_ST_IBF     BIT(1)            /**< @brief Input buffer full */
#define KBC_ST_OBF     BIT(0)            /**< @brief Output buffer full */

/* KBC Commands for PC-AT/PS2 */

#define READ_CMD_BYTE 0x20           /**< @brief Read command byte (returns command byte) */        
#define WRITE_CMD_BYTE 0x60          /**< @brief Write command byte (takes arguments: command byte) */               
#define CHECK_MOUSE_INTERFACE 0xA9       /**< @brief return 0 if Ok */
#define MOUSE_WRITE_CMD 0xD4         /**< @brief Write byte to mouse (takes arguments: byte to write) */
                      

/* KBC Command Byte */

#define KBC_CMD_DIS_MOUSE BIT(5) /**< @brief Disable mouse interface */
#define KBC_CMD_DIS_KBD  BIT(4) /**< @brief Disable keyboard interface */
#define KBC_CMD_INT_MOUSE BIT(1) /**< @brief Enable interrupt on OBF, from mouse */
#define KBC_CMD_INT_KBD  BIT(0) /**< @brief Enable interrupt on OBF, from keyboard */

/* Mouse packet */

#define MOUSE_LEFT BIT(0) /**< @brief Left button set */
#define MOUSE_RIGHT BIT(1)  /**< @brief Left button set */
#define MOUSE_MIDDLE BIT(2) /**< @brief Left button set */

/* PS/2 Mouse commands */

#define ENABLE_DATA_REPORTING   0XF4 /**< @brief Command byte to enable data */
#define MOUSE_DIS_DR   0XF5  /**< @brief Command byte to disable data */
#define SET_STREAM_MODE    0xEA /**< @brief Command byte to set stream mode */

/* S/2 Mouse commands - responses */
#define MOUSE_ACK 0XFA /**< @brief  Everything ok */
#define MOUSE_NACK 0XFE /**< @brief Invalid byte */
#define MOUSE_ERROR 0XFC /**< @brief Second consecutive invalid byte */

/**@}*/

#endif /* _LCOM_M_I8042_H */


