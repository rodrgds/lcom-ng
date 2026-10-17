#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_


#include <lcom/lcf.h>


/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 KBC.
 */

//KBC STATUS REGISTER

#define KBC_ST_REG 0x64

#define KBC_ST_IBF BIT(1)
#define KBC_ST_OBF BIT(0)
#define KBC_ST_AUX BIT(5)
#define KBC_ERR (BIT(7)|BIT(6))

//KBC OUT BUFFER

#define KBC_OUT_BUF 0x60
#define KBC_OUT_BUF_BIT BIT(0)

//KBC IRQ LINE

#define KBC_IRQ 1
#define MOUSE_IRQ 12

//DELAY

#define DELAY_US 2000

// MAKE/BREAK CODE

#define KBC_MAKE_CODE_CHECK 0x80

#define KBC_ESC_MK 0x01
#define KBC_ESC_BR 0x81

#define KBC_SPACE_MK 0x39
#define KBC_SPACE_BR 0xB9

#define KBC_ENTER_MK 0x1C
#define KBC_ENTER_BR 0x9C

#define KBC_W_MK 0x11
#define KBC_A_MK 0x1e
#define KBC_S_MK 0x1f
#define KBC_D_MK 0x20

#define KBC_A_BR 0x9e
#define KBC_B_BR 0xb0
#define KBC_C_BR 0xae
#define KBC_D_BR 0xa0
#define KBC_E_BR 0x92
#define KBC_F_BR 0xa1
#define KBC_G_BR 0xa2
#define KBC_H_BR 0xa3
#define KBC_I_BR 0x97
#define KBC_J_BR 0xa4
#define KBC_K_BR 0xa5
#define KBC_L_BR 0xa6
#define KBC_M_BR 0xb2
#define KBC_N_BR 0xb1
#define KBC_O_BR 0x98
#define KBC_P_BR 0x99
#define KBC_Q_BR 0x90
#define KBC_R_BR 0x93
#define KBC_S_BR 0x9f
#define KBC_T_BR 0x94
#define KBC_U_BR 0x96
#define KBC_V_BR 0xaf
#define KBC_W_BR 0x91
#define KBC_X_BR 0xad
#define KBC_Y_BR 0x95
#define KBC_Z_BR 0xac
#define KBC_BKSPC 0X8e

#define NUMBER_0_BR 0x8b
#define NUMBER_1_BR 0x82
#define NUMBER_2_BR 0x83
#define NUMBER_3_BR 0x84

#define KP_NUMBER_0_BR 0xd2
#define KP_NUMBER_1_BR 0xcf
#define KP_NUMBER_2_BR 0xd0
#define KP_NUMBER_3_BR 0xd1

// 2 BYTE MAKE/BREAK CODE

#define KBC_2_BYTE 0xE0

#define KBC_UP_MK 0x48
#define KBC_UP_BR 0xc8

#define KBC_LEFT_MK 0x4b
#define KBC_LEFT_BR 0xcb

#define KBC_RIGHT_MK 0x4d
#define KBC_RIGHT_BR 0xcd

#define KBC_DOWN_MK 0x50
#define KBC_DOWN_BR 0xd0

// KBC COMMAND

#define KBC_CMD_REG 0x64
#define KBC_READ_CMD_BYTE 0x60
#define KBC_WRITE_CMD_BYTE 0X60
#define KBC_WRITE_CMD_ARG 0X60

#define READ_BACK_CMD 0x20
#define WRITE_CMD 0x60

#define KBC_DIS_CMD BIT(4)
#define KBC_ENBL_CMD BIT(0)

//MOUSE COMMANDS

#define KBC_WR_BYTE_MOUSE 0xd4

#define MOUSE_ENABLE_DATA_REPORT 0xf4
#define MOUSE_DISABLE_DATA_REPORT 0xf5

//MOUSE ANSWER

#define MOUSE_ACK 0xfa
#define MOUSE_NACK 0xfe
#define MOUSE_ERROR 0xfc

//MOUSE PACKET

#define MOUSE_L_BUTTON BIT(0)
#define MOUSE_R_BUTTON BIT(1)
#define MOUSE_M_BUTTON BIT(2)
#define MOUSE_CK_FST_BYTE BIT(3)
#define MOUSE_MSB_X_DELTA BIT(4)
#define MOUSE_MSB_Y_DELTA BIT(5)
#define MOUSE_X_OVFL BIT(6) 
#define MOUSE_Y_OVFL BIT(7)

/**@}*/


#endif /* _LCOM_I8042_H_ */
