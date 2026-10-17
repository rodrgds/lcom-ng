#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8254
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

#define BIT(n) (0x01<<(n))

#define KBD_IRQ	        1
#define PS2_IRQ	        12

/* I/O port addresses */

#define KBD_STATUS	    0x64 /**< @brief Timer 2 count register */
#define	KBC_CMD_REG		0x64
#define KBD_CTRL		0x60 /**< @brief Control register */
#define OUT_BUF         0x60 /** Command Byte for PS2 */

#define READ_CMD        0x20

#define CHECK_KBC       0xAA

#define CHECK_KBD_INT   0xAB

#define DIS_KBD_INT     0xAD

#define EN_KBD_INT      0xAE

#define OBF             BIT(0)

#define IBF             BIT(1)

#define AUX             BIT(5)  //Mouse Data

#define TO_ERR          BIT(6)

#define PAR_ERR         BIT(7)

#define A_MAKE 			0x1E

#define A_BREAK 		0x9E

#define B_MAKE 			0x30

#define B_BREAK 		0xB0

#define C_MAKE 			0x2E

#define C_BREAK 		0xAE

#define D_MAKE 			0x20

#define D_BREAK 		0xA0

#define E_MAKE 			0x12

#define E_BREAK 		0x92

#define F_MAKE 			0x21

#define F_BREAK 		0xA1

#define G_MAKE 			0x22

#define G_BREAK 		0xA2

#define H_MAKE 			0x23

#define H_BREAK 		0xA3

#define I_MAKE 			0x17

#define I_BREAK 		0x97

#define J_MAKE 			0x24

#define J_BREAK 		0xA4

#define K_MAKE 			0x25

#define K_BREAK 		0xA5

#define L_MAKE 			0x26

#define L_BREAK 		0xA6

#define M_MAKE 			0x32

#define M_BREAK 		0xB2

#define N_MAKE 			0x31

#define N_BREAK 		0xB1

#define O_MAKE 			0x18

#define O_BREAK 		0x98

#define P_MAKE 			0x19

#define P_BREAK 		0x99

#define Q_MAKE 			0x10

#define Q_BREAK 		0x90

#define R_MAKE 			0x13

#define R_BREAK 		0x93

#define S_MAKE 			0x1F

#define S_BREAK 		0x9F

#define T_MAKE 			0x14

#define T_BREAK 		0x94

#define U_MAKE 			0x16

#define U_BREAK 		0x96

#define V_MAKE 			0x2F

#define V_BREAK 		0xAF

#define W_MAKE 			0x11

#define W_BREAK 		0x91

#define X_MAKE 			0x2D

#define X_BREAK 		0xAD

#define Y_MAKE 			0x15

#define Y_BREAK 		0x95

#define Z_MAKE 			0x2C

#define Z_BREAK 		0xAC

#define ONE_MAKE		0x02

#define ONE_BREAK		0x82

#define TWO_MAKE		0x03

#define TWO_BREAK		0x83

#define THREE_MAKE		0x04

#define THREE_BREAK		0x84

#define FOUR_MAKE		0x05

#define FOUR_BREAK		0x85

#define FIVE_MAKE		0x06

#define FIVE_BREAK		0x86

#define SIX_MAKE		0x07

#define SIX_BREAK		0x87

#define SEVEN_MAKE		0x08

#define SEVEN_BREAK		0x88

#define EIGHT_MAKE		0x09

#define EIGHT_BREAK		0x89

#define NINE_MAKE		0x0A

#define NINE_BREAK		0x8A

#define ZERO_MAKE		0x0B

#define ZERO_BREAK		0x8B

#define ESC_MAKE 		0x01

#define ESC_BREAK 		0x81

#define ENTER_MAKE		0x1C

#define ENTER_BREAK		0x9C

#define L_CTRL_MAKE 	0x1D

#define L_CTRL_BREAK 	0x9D

#define L_SHIFT_MAKE 	0x2A

#define L_SHIFT_BREAK 	0xAA

#define R_SHIFT_MAKE 	0x36

#define R_SHIFT_BREAK 	0xB6

#define SPACE_MAKE		0x39

#define SPACE_BREAK 	0xB9

#define UP_ARROW_MAKE	0x48

#define UP_ARROW_BREAK	0xC8

#define DOWN_ARROW_MAKE	0x50

#define DOWN_ARROW_BREAK 0xD0

#define LEFT_ARROW_MAKE	0x4B

#define LEFT_ARROW_BREAK 0xCB

#define RIGHT_ARROW_MAKE 0x4D

#define RIGHT_ARROW_BREAK 0xCD

#define DOUBLE_CODE 	0xE0

#define DELAY_US    	20000

#define EN_DATA_REP		0xF4

#define COMPLEMENT(x)	((0xFF^(x))+1)

/**Mouse-Related KBC Commands
 * These commands must be written to port 0x64
 * Arguments and return values are passed via port 0x60*/
#define RCB				0x20 /** Read Command Byte*/
#define WCB				0x60 /** Write Command Byte*/
#define DIS_PS2			0xA7 /** Disable Mouse*/
#define EN_PS2			0xA8 /** Enable Mouse*/
#define CHECK_PS2_INT	0xA9 /** Check Mouse Interface*/
#define WR_TO_PS2		0xD4 /** Write Byte to Mous*/

/**Command Byte
 * Use kbc command 0x20 on port 0x64 to read
 * Use kbc command 0x60 on port 0x64 to write */
#define DIS2			BIT(5) /** disable mouse*/
#define DIS				BIT(4) /** disable keyboard*/
#define INT2			BIT(1) /** enable interrupt on OBF, from mouse*/
#define INT				BIT(0) /** enable interrupt on OBF, from keyboard*/
 
/**Commands passed as arguments of command 0xD4
  * Arguments of these commands, if any, must also be passed as arguments of command 0xD4
	Responses to these commands, if any, are put in the KBC’s OUT_BUF and should be read via port 0x60*/
#define RESET			0xFF /** Mouse reset*/
#define RESEND			0xFE /** For serial communications errors*/
#define DEFAULT			0xF6 /** Set default values*/
#define	DISABLE			0xF5 /** Disable (Data Reporting) In stream mode, should be sent before any other command*/
#define ENABLE			0xF4 /** Enable (Data Reporting) In stream mode only*/
#define SET_SAMPLE_RATE	0xF3 /** Sets state sampling rate*/
#define SET_REMOTE_MODE	0xF0 /** Send data on request only*/
#define READ_DATA		0xEB /** Send data packet request*/
#define SET_STREAM_MODE	0xEA /** Send data on events*/
#define	STATUS_REQUEST	0xE9 /** Get mouse configuration (3 bytes)*/
#define SET_RESOLUTION	0xE8 /** Set Resolution*/
#define	SET_SCALING_2_1	0xE7 /** Set Scaling 2:1 Acceleration mode*/
#define	SET_SCALING_1_1	0xE6 /** Set Scaling 1:1 Linear mode*/

/**Mouse Controller Returns*/
#define	ACK				0xFA /** If everything OK*/
#define	NACK			0xFE /** If invalid byte (may be because of a serial communication error)*/
#define	ERROR			0xFC /** Second consecutive invalid byte*/

/** */


/**@}*/

#endif /* _LCOM_I8042_H */

