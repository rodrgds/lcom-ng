#include <lcom/lcf.h>

#define KBD_IRQ 1
#define MOUSE_IRQ 12


#define DELAY_US 20000

/* Keyboard*/
#define ESC 0x81
#define UP 0x11 
#define DOWN 0x1f    
#define LEFT 0x1e    
#define RIGHT 0x20
#define A 0x9e 		
#define	B 0xb0
#define C 0xae
#define D 0xa0
#define E 0x92
#define F 0xa1
#define G 0xa2
#define H 0xa3
#define I 0x97
#define J 0xa4
#define K 0xa5
#define L 0xa6
#define M 0xb2
#define N 0xb1
#define O 0x98
#define P 0x99
#define Q 0x90
#define R 0x93
#define S 0x9F
#define T 0x94
#define U 0x96
#define V 0xaf
#define W 0x91
#define X 0xad
#define Y 0x95
#define Z 0xac
#define BACKSPACE 0x0e  
#define ENTER 0x1c 	
#define TWOBYTES 0xe0


/* I/O Port Adress */
#define STAT_REG 0x64
#define OUT_BUF 0x60
#define IN_BUF_CMD 0x64
#define IN_BUF_ARGS 0x60

/* Status macros*/
#define ST_OUT_BUF BIT(0)
#define ST_IN_BUF BIT(1)
#define ST_MOUSE_DATA BIT(5)
#define ST_TO_ERR BIT(6)
#define ST_PAR_ERR BIT(7)

/*Command*/
#define WRITE_COMMAND 0x60
#define COMMAND_BYTE_INTERRUPT_KB BIT(0)

/* Command Byte */
#define KBC_ST_REG 0x64
#define KBC_CMD_REG 0x64

/* KBC Time out */
#define KBC_WAIT 10000

/* KBD Commands */
#define READ_CMD_BYTE 0x20
#define WRITE_CMD_BYTE 0x60

/* KBD Command Byte */
#define CMD_BYTE_DISABLE_MOUSE BIT(5)
#define CMD_BYTE_DISABLE_KBD_INTERFACE BIT(4)
#define CMD_BYTE_ENABLE_INT_MOUSE BIT(1)
#define CMD_BYTE_ENABLE_INT_KBD BIT(0)

/* Mouse */
#define LB BIT(0)
#define RB BIT(1)
#define MB BIT(2)
#define MSB_X_DELTA BIT(4)
#define MSB_Y_DELTA BIT(5)
#define X_OVFL BIT(6)
#define Y_OVFL BIT(7)

#define DIS_MOUSE 0xA7
#define WRITE_BYTE_MOUSE 0xD4
#define DIS_DATA_REP 0xF5
#define ENA_DATA_REP 0xF4
#define STREAM_MODE 0xEA
#define REMOTE_MODE 0xF0
#define READ_DATA 0xEB

/* Mouse Controller command responses */
#define ACK 0xFA   //Acknowledged command or argument
#define NACK 0xFE  //Invalid byte
#define ERROR 0xFC //Second consecutive invalid byte
