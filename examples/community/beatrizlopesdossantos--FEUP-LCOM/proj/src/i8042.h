#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/**
 * @defgroup i8042
 * @{
 * @brief Macros for Keyboard and Mouse
 */

#define KBC_ST_REG          0x64        /**< @brief KBC Status Register */
#define KBC_CMD_REG         0x64        /**< @brief KBC Command Register */
#define KBC_OUT_BUF         0x60        /**< @brief KBC Output Buffer */

#define PARITY_ERROR        BIT(7)      /**< @brief Parity Error */           
#define TIMEOUT_ERROR       BIT(6)      /**< @brief Timeout Error */           
#define AUX                 BIT(5)      /**< @brief Mouse Data */         
#define IBFULL              BIT(1)      /**< @brief Input Buffer Full */
#define OBFULL              BIT(0)      /**< @brief Output Buffer Full */

#define KBD_IRQ             1           /**< @brief KBD IRQ Line */

#define MOUSE_IRQ           12          /**< @brief Mouse IRQ Line */

#define ESQ                 0x81        /**< @brief Esc key make code */
#define LEFT_ARROW_MAKE     0x4B        /**< @brief Left arrow key make code */
#define LEFT_ARROW_BREAK     0xCB        /**< @brief Left arrow key break code */
#define RIGHT_ARROW_MAKE    0x4D        /**< @brief Right arrow key make code */
#define RIGHT_ARROW_BREAK   0xCD        /**< @brief Right arrow key break code */
#define A_MAKE              0X1E        /**< @brief A key make code */ 
#define A_BREAK             0X9E        /**< @brief A key break code */
#define D_MAKE              0X20        /**< @brief D key make code */
#define D_BREAK             0XA0        /**< @brief D key break code */

#define FIRST_OF_TWO_BYTES  0xE0        /**< @brief Fist of a two byte code */
#define WRITE_BYTE_MOUSE    0xD4        /**< @brief Writes byte to mouse */

#define Y_OVFL              BIT(7)      /**< @brief Overflow of Y */
#define X_OVFL              BIT(6)      /**< @brief Overflow of X */         
#define MSB_Y               BIT(5)      /**< @brief Y delta sign */           
#define MSB_X               BIT(4)      /**< @brief X delta sign */           
#define CHECK               BIT(3)      /**< @brief Always set on the fist byte */         
#define MB                  BIT(2)      /**< @brief Middle button bit */
#define RB                  BIT(1)      /**< @brief Right button bit */
#define LB                  BIT(0)      /**< @brief Left button bit */

#define ENABLE              0xF4        /**< @brief Enables Mouse data reporting */
#define DISABLE             0xF5        /**< @brief Disables Mouse data reporting */

#define ACK                 0xFA        /**< @brief Acknowledge Command */
#define NACK                0xFE        /**< @brief Invalide Byte */
#define ERROR               0xFC        /**< @brief Second consecutive invalid byte */

#define DELAY_US            20000       /**< @brief Time to delay */

/** @} end of i8042 */
#endif //I8042_H
