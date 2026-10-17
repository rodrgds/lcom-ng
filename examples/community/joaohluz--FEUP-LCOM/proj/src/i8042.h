#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard and mouse.
 */

extern int counter;
#define BIT(n) (0x01<<(n))

#ifdef LAB3
int sys_inb_cnt(port_t port, uint32_t *byte);
#else
#define sys_inb_cnt(p,q) sys_inb(p,q)
#endif

#define KB_IRQ   1
#define MOUSE_IRQ  12

#define KB_STATUS_REG      0x64 /**< @brief Register to read the KBDx's status */
#define KBC_CM_REG         0x64 /**< @brief Control wor for KBC's command register */
#define KBC_READ           0x20 /**< @brief Command to read the KBDx */
#define KB_ENABLE 		   BIT(0)
#define WRITE_BYTE         0x60 /**< @brief Comand to write ro the KBC */

#define OUT_BUF      	   0x60 /**< @brief Adress to the KBC's output buffer */

#define ESC_BREAK          0x81 /**< @brief Break code of the ESC key */
#define TWO_BYTE_SCAN      0xe0 /**< @brief First byte of a two byte make or brake code */

#define DELAY_US      	   20000 /**< @brief Delay to get keyboard data */
#define TEMP_HOOK_KB 	   2
#define TEMP_HOOK_MOUSE    3

#define	OBF 		       BIT(0) /**< @brief Control word for OBF */
#define IBF 		       BIT(1) /**< @brief Control word for IBF */
#define AUX                BIT(5) /**< @brief Control word for AUX */
#define PAR_ERR 	       BIT(7) /**< @brief Control word for PAR_ERR */
#define TO_ERR 		       BIT(6) /**< @brief Control word for TO_ERR */

#define LEFT_ARROW        0x20 /**< @brief Make code of the A key */
#define RIGHT_ARROW       0x1e /**< @brief Make code of the D key */
#define SPACE_BAR         0x39 /**< @brief Make code of the SOPCE_BAR key */
#define KEY_P             0x19 /**< @brief Make code of the P key */
#define KEY_H             0x23 /**< @brief Make code of the H key */


#define WRITE_TO_MOUSE     0xd4 /**< @brief Control word ro writw to mouse */
#define READ_DATA 		   0xeb /**< @brief Control word for OBF */
#define DISABLE_MOUSE      0xf5 /**< @brief Control word to disable the mouse */
#define STREAM_MODE        0xea /**< @brief Control word to set the mouse stream mode */
#define MOUSE_DEFAULT      0xf6 /**< @brief Control word to reset the mouse's default settings */
#define REMOTE_MODE        0xf0 /**< @brief Control word to set the mouse remote mode */
#define ENABLE_DR          0xF4
#define MOUSE_RESET        0xff
//#define DISABLE_MOUSE      0xa7
#define DISABLE_DATA_REPORTING   0xf5 /**< @brief Control word to enable the mouse's dara reporting mode */
#define ACK     0xfa /**< @brief Control word for ACK */
#define NACK    0xfe /**< @brief Control word for NACK */
#define ERROR   0xfc /**< @brief Control word for ERROR */

/**@}*/

#endif /* _LCOM_I8042_H */
