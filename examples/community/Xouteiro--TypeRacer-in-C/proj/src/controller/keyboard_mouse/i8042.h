#ifndef _LCOM_I8042_KEY_H_
#define _LCOM_I8042_KEY_H_

#define IRQ_KEYBOARD    1
#define IRQ_MOUSE       12


#define BREAK_ESC       0x81
#define MAKE_CODE       BIT(7)
#define TWO_BYTES       0xE0
#define ENABLE_INT      BIT(0)

#define TIMEOUT_ERROR   BIT(6)
#define PARITY_ERROR    BIT(7)
#define FULL_OUT_BUFFER BIT(0)
#define FULL_IN_BUFFER  BIT(1)

#define KBC_STATUS_REG  0x64
#define KBC_IN_CMD      0x64
#define KBC_OUT_CMD     0x60
#define KBC_READ_CMD    0x20
#define KBC_WRITE_CMD   0x60

#define WAIT_KBC        20000
#define MAX_ATTEMPTS     10



#define Q_KEY           16
#define W_KEY           17
#define E_KEY           18
#define R_KEY           19
#define T_KEY           20
#define Y_KEY           21
#define U_KEY           22
#define I_KEY           23
#define O_KEY           24
#define P_KEY           25
#define A_KEY           30
#define S_KEY           31
#define D_KEY           32
#define F_KEY           33
#define G_KEY           34
#define H_KEY           35
#define J_KEY           36
#define K_KEY           37
#define L_KEY           38
#define Z_KEY           44
#define X_KEY           45
#define C_KEY           46
#define V_KEY           47
#define B_KEY           48
#define N_KEY           49
#define M_KEY           50
#define COMMA_KEY       51
#define DOT_KEY         52

#define ESC_KEY         0x81
#define SPACE_KEY       57
#define ENTER_KEY       28
#define BACKSPACE_KEY   14

#define KEY_1           2
#define KEY_2           3

#define MOUSE_LB            BIT(0)
#define MOUSE_RB            BIT(1)
#define MOUSE_MB            BIT(2)
#define FIRST_BYTE          BIT(3)
#define MOUSE_X_SIGNAL      BIT(4)
#define MOUSE_Y_SIGNAL      BIT(5)
#define MOUSE_X_OVERFLOW    BIT(6)
#define MOUSE_Y_OVERFLOW    BIT(7)
#define DISABLE_DATA_REPORT 0xF5
#define ENABLE_DATA_REPORT  0xF4
#define ENABLE_STREAM_MODE  0xEA
#define MOUSE_READ_DATA     0xEB
#define WRITE_BYTE_MOUSE    0xD4

#define ACK 0xFA    
#define NACK 0xFE

#endif
