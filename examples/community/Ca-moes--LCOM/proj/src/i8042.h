#ifndef _LCOM_I8042_H
#define _LCOM_I8042_H

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Timer. Needs to be completed.
 */

#define KBC_IRQ        1      /**<@brief Keyboard Controller IRQ line - 1*/
#define MOUSE_IRQ      12     /**<@brief PS/2 Mouse IRQ line - 1*/

//make codes and break codes

#define MC_ESC         0x01   /**<@brief ESC key makecode*/
#define MC_CAPS        0x3A /**<@brief CAPS key makecode*/
#define MC_SHIFT       0x36   /**<@brief SHIFT key makecode*/
#define MC_SHIFT2      0x2A   /**<@brief SHIFT key makecode*/
#define MC_BACK        0x0E   /**<@brief BACK key makecode*/
#define MC_DOT         0x34   /**<@brief DOT key makecode*/

#define BC_ESC         0x81   /**<@brief ESC key breakcode*/
#define BC_CAPS        0xBA  /**<@brief CAPS key breakcode*/
#define BC_SHIFT       0xB6   /**<@brief SHIFT key breakcode*/
#define BC_SHIFT2      0xAA   /**<@brief SHIFT key breakcode*/
#define BC_BACK        0x8E   /**<@brief BACK key breakcode*/

#define SCANCODE_2B_FB 0xE0   /**<@brief first byte of 2 byte scancodes*/

#define DELAY_US               20000   //delay used in the delay function

// Registers

#define OUT_BUF        0X60   /**<@brief Output Buffer */
#define STAT_REG       0x64   /**<@brief Satus Register */
#define KBC_CMD_REG    0x64   /**<@brief Where to Write Keyboard Command  */
#define KBC_CMD_ARGS   0x60   /**<@brief Where to Write the KBC arguments */

// KBC Commands

#define KBC_READ_CB        0x20   /**<@brief Read Command Byte - R: CB*/
#define KBC_WRITE_CB       0x60   /**<@brief Write Command Byte  - A: CB*/
#define KBC_SELF_TEST      0xAA   /**<@brief Self Test - R_OK: 0x55; R_ERROR: 0xFC*/
#define KBC_CHECK_KBI      0xAB   /**<@brief Check Keyboard Interface - R_OK: 0;*/
#define KBC_DIS_KBI        0xAD   /**<@brief Disable Keyboard Interface*/
#define KBC_EN_KBI         0xAE   /**<@brief Enable Keyboard Interface*/
#define KBC_DIS_MOUSE      0xA7   /**<@brief Disable Mouse*/
#define KBC_EN_MOUSE       0xA8   /**<@brief Enable Mouse*/
#define KBC_CHECK_MOUSEI   0xA9   /**<@brief Check Mouse Interface - R_OK: 0*/
#define KBC_WRITE_MOUSE 0xD4   /**<@brief  Write Byte to Mouse - A: B*/

// KBC Command Byte

#define KBC_CB_DIS2    BIT(5)  /**<@brief Disable Mouse */
#define KBC_CB_DIS     BIT(4)  /**<@brief Disable Keyboard Interface (1)*/
#define KBC_CB_INT2    BIT(1)  /**<@brief Enable OUT_BUF Interrupt,from mouse (1)*/
#define KBC_CB_INT     BIT(0)  /**<@brief Enable OUT_BUF Interrupt, from keyboard (1)*/

// Status Register

#define KB_SR_PARITY   BIT(7)  /**<@brief Parity Error (1) */
#define KB_SR_TIMEOUT  BIT(6)  /**<@brief Time Out Error (1) */
#define KB_SR_AUX      BIT(5)  /**<@brief Mode in which the controller doesn't check parity nor converts scan codes*/
#define KB_SR_INH      BIT(4)  /**<@brief Inhibit Flag (0) */
#define KB_SR_A2       BIT(3)  /**<@brief Command(1) or Data(0) byte */
#define KB_SR_SYS      BIT(2)  /**<@breif System Flag - power/reset (0), initialized (1)*/
#define KB_SR_IBF      BIT(1)  /**<@brief IN_BUF Full(1) - don't write args/commands! 0 after KBC reads it*/
#define KB_SR_OBF      BIT(0)  /**<@brief OUT_BUF Full(1) - don't write args/commands! 0 after system reads it*/

//PS/2 Mouse Commands

#define MC_RESET            0xFF    /**<@brief Mouse Reset*/
#define MC_RESEND           0xFE    /**<@brief For serial communications errors*/
#define MC_DEFAULTS         0xF6    /**<@brief Set default values*/
#define MC_DIS_DATA_REP     0xF5    /**<@brief In stream mode, Disable (Data Reporting) */
#define MC_EN_DATA_REP      0xF4    /**<@brief In stream mode Enable (Data Reporting) */
#define MC_SET_SR           0xF3    /**<@brief Set Sample Rate*/
#define MC_SET_REMOTE_MODE  0xF0    /**<@brief Set Remote mode */
#define MC_READ_DATA        0xEB    /**<@brief Read Data*/
#define MC_SET_STREAM_MODE  0xEA    /**<@brief Set Stream Mode*/
#define MC_ST_REQ           0xE9    /**<@brief Status Request Get mouse configuration (3 bytes)*/
#define MC_SET_RES          0xE8    /**<@brief Set Resolution*/ 
#define MC_SET_RES21        0xE7    /**<@brief Acceleration mode*/
#define MC_SET_RES11        0xE6    /**<@brief Linear mode*/

//Mouse Controller Acknowledgment Byte
#define MC_ACK    0xFA    /**<@brief Everything OK*/
#define MC_NACK   0xFE    /**<@brief Invalid Byte (may be because of a serial communication error)*/
#define MC_ERROR  0xFC    /**<@brief Second consecutive invalid byte*/

//Packet Format -  Byte 1
#define B1_YOVFL       BIT(7)   /**<@brief Y(vertical) overflow*/
#define B1_XOVFL       BIT(6)   /**<@brief X(horizontal) overflow*/
#define B1_YSIGN       BIT(5)   /**<@brief Y sign*/
#define B1_XSIGN       BIT(4)   /**<@brief X sign*/
#define B1_3           BIT(3)   /**<@brief Bit 3 of packet's first byte*/
#define B1_MID         BIT(2)   /**<@brief Middle Button*/
#define B1_RIGHT       BIT(1)   /**<@brief Right Button*/
#define B1_LEFT        BIT(0)   /**<@brief Left Button*/

#endif // _LCOM_I8042_H
