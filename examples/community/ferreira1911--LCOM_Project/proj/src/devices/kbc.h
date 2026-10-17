#ifndef _KBC_H_
#define _KBC_H_

#include <lcom/lcf.h>  
#include <stdint.h> 

/* I/O port addresses */

#define KBC_OUTPUT_BUFFER 0x60 /**< @brief Output buffer */
#define KBC_INPUT_BUFFER  0x60 /**< @brief Input buffer */
#define KBC_COMMAND_REGISTER 0x64 /**< @brief Command register */
#define KBC_STATUS_REGISTER  0x64 /**< @brief Status register */

/* Status register flags */

#define KBC_STATUS_OB_FULL BIT(0) /**< @brief Output buffer full */
#define KBC_STATUS_IB_FULL BIT(1) /**< @brief Input buffer full */
#define KBC_STATUS_D BIT(5) /**< @brief Mouse data (1 = mouse, 0 = keyboard) */
#define KBC_STATUS_TIMEOUT_ERR BIT(6) /**< @brief Timeout error */
#define KBC_STATUS_PARITY_ERR  BIT(7) /**< @brief Parity error */

/* KBC Commands */

#define KBC_READ_CMD  0x20 /**< @brief Read Command Byte */
#define KBC_WRITE_CMD 0x60 /**< @brief Write Command Byte */

/* Command Byte */

#define KBC_CB_EN_KBD_INT    BIT(0) /**< @brief Enable keyboard interrupts */
#define KBC_CB_EN_MOUSE_INT  BIT(1) /**< @brief Enable mouse interrupts */
#define KBC_CB_DISABLE_KBD   BIT(4) /**< @brief Disable keyboard interface */
#define KBC_CB_DISABLE_MOUSE BIT(5) /**< @brief Disable mouse interface */

/* Useful constants */

#define KBC_DELAY 20000 /**< @brief Delay in microseconds */

/* Function declarations */

int write_kbc_cmd(uint8_t cmd, uint8_t port);
int read_kbc_cmd(uint8_t *byte);

#endif 
