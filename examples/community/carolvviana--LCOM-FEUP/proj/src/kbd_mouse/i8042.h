#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define IRQ_KBD 1 /*!< @brief IRQLine for keyboard */
#define IRQ_MOUSE 12 /*!< @brief IRQLine for mouse */

#define ESC_KEY 0x81 /*!< @brief ESC key scancode */
#define UP_KEY 0x48 /*!< @brief UP key scancode */
#define DOWN_KEY 0x50 /*!< @brief DOWN key scancode */
#define LEFT_KEY 0x4B /*!< @brief LEFT key scancode */
#define RIGHT_KEY 0x4D /*!< @brief RIGHT key scancode */
#define ENTER_KEY 0x1C /*!< @brief ENTER key scancode */

#define KBD_OUT_CMD 0x60 /*!< @brief KBD output buffer */
#define KBD_IN_CMD 0x64 /*!< @brief KBD input buffer */

#define KBC_STATUS_REG 0x64 /*!< @brief KBC Status Register */
#define KBC_WRITE_CMD 0x60 /*!< @brief KBC Write Command byte */
#define KBC_READ_CMD 0x20 /*!< @brief KBC Read Command Byte */
#define KBC_INPUTB_FULL BIT(1) /*!< @brief Input buffer full bit */
#define KBC_OUTPUTB_FULL BIT(0) /*!< @brief Output buffer full bit */

#define PARITY_ERR BIT(7) /*!< @brief Parity Error bit */
#define TIMEOUT_ERR BIT(6) /*!< @brief Timeout Error bit */

#define ENABLE_KBD_INT BIT(0) /*!< @brief Enable keyboard intrrupts bit */

#endif
//adicionar macros?????????? que macros? ass:carol 
