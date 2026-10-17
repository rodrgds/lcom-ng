#ifndef _LCOM_I8042_KBD_H_
#define _LCOM_I8042_KBD_H_

#include <lcom/lcf.h>

/* I/O Ports */
#define KBD_IRQ       1             // keyboard interrupt line
#define OUT_BUF       0x60          // output buffer port
#define KBC_ARGS      0x60          // same as output buffer, used for sending KBC command arguments
#define STATUS_REG    0x64          // status register port
#define KBC_CMD_REG   0x64          // same as status register, but used for issuing KBC commands

/* Bit masks */
#define PARITY        BIT(7)        // bit mask to check for parity error
#define TIME_OUT      BIT(6)        // bit mask to check for timeout error
#define AUX           BIT(5)        // bit mask to check if we're receiving mouse data
#define IBF           BIT(1)        // bit mask to check if input buffer is full
#define OBF           BIT(0)        // bit mask to check if output buffer is full
#define KBC_INT       BIT(0)        // bit mask to check if keyboard interrupts are enabled
#define IS_BREAK      BIT(7)        // bit mask to check if a scancode is a breakcode

/* Constants */
#define TWO_BYTE_SC   0xE0          // value to check if we are dealing with a 2 byte scan code
#define ESC_MAKE      0x01          // makecode for ESC
#define ESC_BREAK     0x81          // breakcode for ESC
#define KBC_OK        0x55          // code for valid keyboard controller
#define KBC_ERR       0xFC          // code for invalid keyboard controller

/* KBC Commands */
#define READ_CMDB     0x20          // read command byte
#define WRITE_CMDB    0x60          // write command byte
#define KBC_CHK       0xAA          // check keyboard controller; returns KBC_OK if ok, else KBC_ERR
#define KBI_CHK       0xAB          // check keyboard interface; returns 0 if OK
#define KBI_DIS       0xAD          // disable keyboard interface
#define KBI_ENA       0xAE          // enable keyboard interface

/* Keys */
#define LEFT_ARROW    0x4b
#define RIGHT_ARROW   0x4d
#define DOWN_ARROW    0x50
#define UP_ARROW      0x48

#define ENTER         0x1c
#define SPACE         0x39
#define A_T            0x1e
#define B_T             0x30
#define C_T             0x2e
#define D_T             0x20
#define E_T             0x12
#define F_T             0x21
#define G_T             0x22
#define H_T             0x23
#define I_T             0x17
#define J_T             0x24
#define K_T             0x25
#define L_T             0x26
#define M_T             0x32
#define N_T             0x31
#define O_T             0x18
#define P_T             0x19
#define Q_T             0x10
#define R_T             0x13
#define S_T             0x1f
#define T_T             0x14
#define U_T             0x16
#define V_T             0x2f
#define W_T             0x11
#define X_T             0x2d
#define Y_T             0x15
#define Z_T             0x2c

#endif /*_LCOM_I8042_KBD_H_*/
