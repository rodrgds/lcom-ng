#ifndef _LCOM_I8042_H_

#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define DELAY_US        20000

#define KBD_IRQ             1     /**< @brief Keyboard IRQ line */
#define MOUSE_IRQ           12    /**< @brief Mouse IRQ line */

/*-----------KEYBOARD-------------*/

/* I/O port addresses */
#define STAT_REG          0x64    /**< @brief Status Register */
#define KBC_CMD_REG       0x64
#define OUT_BUF           0x60    /**< @brief Output Buffer */
#define IN_BUF_COM        0x64    /**< @brief Input Buffer Commands to the KBC */
#define IN_BUF_ARG        0x60    /**< @brief Input Buffer Arguments of KBC Commands */

#define IBF               BIT(1)

#define OBF               BIT(0)   /* Output buffer */
#define PAR_ERR           BIT(7)   /* Parity error */
#define TO_ERR            BIT(6)   /* Timeout error */
#define AUX               BIT(5)  /*Mouse data*/

/* Measuring Time */
#define DELAY_US          20000

/* Scancodes */
#define TW0_BYTE_SCANCODE 0xE0

#define ESC_KEY            0x81
#define SPACE_KEY          0x39
#define BACKSPACE_KEY      0x0E
#define ENTER_KEY          0X1C

#define A_KEY			   0x1E
#define B_KEY              0x30
#define C_KEY              0x2E
#define D_KEY              0x20
#define E_KEY              0x12
#define F_KEY              0x21
#define G_KEY              0x22
#define H_KEY              0x23
#define I_KEY              0x17
#define J_KEY              0x24
#define K_KEY              0x25
#define L_KEY              0x26
#define M_KEY              0x32
#define N_KEY              0x31
#define O_KEY              0x18
#define P_KEY              0x19
#define Q_KEY              0x10
#define R_KEY              0x13
#define S_KEY              0x1F
#define T_KEY              0x14
#define U_KEY              0x16
#define V_KEY              0x2F
#define W_KEY              0x11
#define X_KEY              0x2D
#define Y_KEY              0x15
#define Z_KEY              0x2C

#define ZERO_KEY           0x0B
#define ONE_KEY            0x02
#define TWO_KEY            0x03
#define THREE_KEY          0x04
#define FOUR_KEY           0x05
#define FIVE_KEY           0x06
#define SIX_KEY            0x07
#define SEVEN_KEY          0x08
#define EIGHT_KEY          0x09
#define NINE_KEY           0x0A


/*KBC Commands */
#define READ_COMD_BYTE    0x20
#define WRITE_COMD_BYTE   0x60


/*-----------MOUSE-------------*/
#define MOUSE_DISABLE     0xF5
#define MOUSE_ENABLE      0xF4
#define MOUSE_STREAM_MODE 0xEA
#define MOUSE_REMOTE      0xF0
#define MOUSE_READ_DATA   0xEB
#define ACK               0xFA
#define MOUSE_WRITE_BYTE  0XD4
#define CTRL_REG          0x64
#define IN_BUF            0x60

#define MOUSE_LB            BIT(0)
#define MOUSE_RB            BIT(1)
#define MOUSE_MB            BIT(2)
#define MOUSE_X             BIT(4)
#define MOUSE_Y             BIT(5)
#define MOUSE_X_OVERFLOW    BIT(6)
#define MOUSE_Y_OVERFLOW    BIT(7)


#endif /* _LCOM_I8042_H */
