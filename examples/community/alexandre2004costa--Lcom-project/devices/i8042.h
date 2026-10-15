#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8042
  * @{
 */

#define MOUSE_REQUEST 0xD4
#define STATUS_REG 0x64
#define IN_KBC_CMD 0x64
#define OUT_KBC_CMD 0x60
#define READ_CMD 0x20
#define WRITE_CMD 0x60
#define KEYBOARD_IRQ 1 
#define TWO_BYTES 0xE0
#define MOUSE_DISABLE 0xF5
#define SUCESSS_BYTE 0xFA
/*
*/
#define ST_OUT_FULL_BUFFER BIT(0)
#define ST_IN_FULL_BUFFER BIT(1)
#define ST_INHIBIT_FLAG BIT(4)
#define ST_MOUSE_DATA BIT(5)
#define ST_TIME_OUT_ERROR BIT(6)
#define ST_PARITY_ERROR BIT(7)

#define IS_BREAK_CODE(code) (code & BIT(7))

// Mouse data packet

#define LB_MOUSE_PACKET BIT(0)
#define RB_MOUSE_PACKET BIT(1)
#define MB_MOUSE_PACKET BIT(2)
#define MSB_X_DELTA BIT(4)
#define MSB_Y_DELTA BIT(5)
#define X_OV_MOUSE_PACKET BIT(6)
#define Y_OV_MOUSE_PACKET BIT(7)

#define MOUSE_IRQ 12
#define KBD_IRQ
#endif /* _LCOM_I8042_H_ */
// Keyboard Keys
#define MAKE_A_KEY 0x1e
#define MAKE_D_KEY 0x20
#define MAKE_S_KEY 0x1f
#define BREAK_A_KEY 0x9e
#define BREAK_D_KEY 0xa0
#define SPACE_BAR_KEY 0x39
#define ENTER_KEY 0x1C
#define ESC_BREAK 0x81


#define WindowWidth 1152
#define WindowHeight 864

/**@}*/
