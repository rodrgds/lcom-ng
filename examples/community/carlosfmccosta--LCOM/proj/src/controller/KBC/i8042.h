/**
 * @file i8042.h
 * @brief Constants and macros for programming the i8042 Keyboard and Mouse controller.
 *
 * This header defines IRQ lines, register addresses, command bytes, status bits,
 * and command/response codes for interfacing with the i8042 controller used for
 * keyboard and mouse operations.
 *
 */
#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * 
 * @brief Constants for programming the i8042 Keyboard and Mouse
 * 
 * @{
 */

/* Keyboard Controller IRQ lines */
#define KBD_IRQ 1    /**< @brief Keyboard IRQ line */
#define MOUSE_IRQ 12 /**< @brief Mouse IRQ line */

/* Delay */
#define DELAY_US 20000 /**< @brief Keyboard delay: 20 milliseconds */

/* Keyboard registers */
#define KBC_OUT_BUF 0x60 /**< @brief Keyboard Output Buffer */
#define KBD_CMD_REG 0x64 /**< @brief Address to write all keyboard and mouse related commands */

/* Keyboard Keys */
#define ESC_KEY 0x81
#define BREAK_CODE_BIT BIT(7) /**< @brief Escape key */
#define KEY_A 0x1E
#define KEY_B 0x30
#define KEY_C 0x2E
#define KEY_D 0x20
#define KEY_E 0x12
#define KEY_F 0x21
#define KEY_G 0x22
#define KEY_H 0x23
#define KEY_I 0x17
#define KEY_J 0x24
#define KEY_K 0x25
#define KEY_L 0x26
#define KEY_M 0x32
#define KEY_N 0x31
#define KEY_O 0x18
#define KEY_P 0x19
#define KEY_Q 0x10
#define KEY_R 0x13
#define KEY_S 0x1F
#define KEY_T 0x14
#define KEY_U 0x16
#define KEY_V 0x2F
#define KEY_W 0x11
#define KEY_X 0x2D
#define KEY_Y 0x15
#define KEY_Z 0x2C
#define KEY_ENTER 0x1C
#define KEY_BACKSPACE 0x0E
#define ARROW_UP 0x48
#define ARROW_DOWN 0x50
#define FIRST_BYTE_SCANCODE 0xE0 /**< @brief first */

/* Status Register Bits */
#define ST_OUTB BIT(0) /**< @brief 0: Output buffer empty, don't read yet. 1: Output buffer full, can be read. This bit is cleared when port 0x60 is read. */
#define ST_INPB BIT(1) /**< @brief 0: Input buffer empty, can be written. 1: Input buffer full, don't write yet. */
#define ST_SYSF BIT(2) /**< @brief Set to 0 after power on reset. Set to 1 after successful completion of the keyboard controller self-test (Basic Assurance Test, BAT). */
#define ST_A2 BIT(3)   /**< @brief 0: Last write to input buffer was data (port 0x60). 1: Last write was a command (port 0x64). */
#define ST_KEYL BIT(4) /**< @brief 0: Locked. 1: Not locked. */
#define ST_AUXB BIT(5) /**< @brief 0: OK. 1: Timeout on transmission from keyboard controller to keyboard. */
#define ST_TIM BIT(6)  /**< @brief 0: OK. 1: Timeout on transmission from keyboard to keyboard controller. */
#define ST_PARE BIT(7) /**< @brief 0: OK; 1: Parity error with last byte. */

/* Keyboard Controller Command Byte Bits */
#define KBD_CMD_KIE BIT(0)   /**< @brief Enable keyboard interrupt */
#define KBD_CMD_MIE BIT(1)   /**< @brief Enable mouse interrupt */
#define KBD_CMD_SYSF BIT(2)  /**< @brief System flag */
#define KBD_CMD_IGNLK BIT(3) /**< @brief Ignore keyboard lock */
#define KBD_CMD_KE BIT(4)    /**< @brief Keyboard enable */
#define KBD_CMD_ME BIT(5)    /**< @brief Mouse enable */
#define KBD_CMD_XLATE BIT(6) /**< @brief Translate keyboard scancodes */
#define KBD_CMD_ZERO BIT(7)  /**< @brief Always 0 */

/* KBC Commands */
#define KBD_RD_CMD 0x20  /**< @brief Returns Command Byte */
#define KBD_WRT_CMD 0x60 /**< @brief Writes Command Byte */
#define CHECK_KBC 0xAA   /**< @brief Self test on KBC */

/* KBC Self-Test Return Values */
#define KBC_OK 0x55
#define KBC_ERROR 0xFC

/* Keyboard Interface Commands */
#define CHECK_KBD_INT 0xAB   /**< @brief Checks keyboard Interface. Returns 0 if OK */
#define DISABLE_KBD_INT 0xAD /**< @brief Disable keyboard interface */
#define ENABLE_KBD_INT 0xAE  /**< @brief Enable keyboard interface */

/* Mouse Commands and Responses */
#define MS_ACKNOWLEDGE 0xFA
#define MS_NACKNOWLEDGE 0xFE
#define MS_ENABLE_REPORTING 0xF4
#define MS_DISABLE_REPORTING 0xF5
#define KBC_WRITE_TO_MS 0xD4

/**@}*/

#endif /* _LCOM_I8042_H_ */
