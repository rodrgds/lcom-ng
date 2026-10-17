#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_
/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 KBC.
 */

#define KBC_IRQ 1       
#define OUT_BUF 0x60      
#define CMD_ARGS_REG 0x60 
#define STATUS_REG 0x64  
#define CMD_REG 0x64      
#define DELAY_US 20000 


#define ESC_KEY 0x81            
#define FIRST_OF_TWO_BYTES 0xE0 
#define UP_KEY 0x48             
#define UP_KEY_BREAK 0xC8 
#define DOWN_KEY 0x50           
#define DOWN_KEY_BREAK 0Xd0
#define LEFT_KEY 0x4b           
#define LEFT_KEY_BREAK 0xCB
#define RIGHT_KEY 0x4d          
#define RIGHT_KEY_BREAK 0xcd
#define W_KEY 0x11 
#define W_KEY_BREAK 0x91
#define S_KEY 0x1f
#define S_KEY_BREAK 0x9f
#define D_KEY 0x20
#define D_KEY_BREAK 0xAF
#define A_KEY 0x1E
#define A_KEY_BREAK 0x9E


#define KBC_PARE BIT(7)  
#define KBC_TIMEO BIT(6) 
#define KBC_AUXB BIT(5)  
#define KBC_KEYL BIT(4)  
#define KBC_CD BIT(3)    
#define KBC_SYSF BIT(2)  
#define KBC_INPB BIT(1)  

#define KBC_OUTB BIT(0)  
#define READ_CMD_B 0x20   
#define CHECK_KBC 0xAA  
#define CHECK_KB_IFC 0xAB 
#define DISABLE_KBD 0xAD  
#define ENABLE_KBS 0xAE  
#define WRITE_CMD_B 0x60  


#define ENABLE_KBD_IR BIT(0) 

#define ENABLE_DATA_REPORT 0xF4
#define DISABLE_DATA_REPORT 0xF5
#define MOUSE_IRQ_LINE 0x0C
#define OUTPUT_BUFFER 0x60
#define STATUS_REGISTER 0x64 
#define COMMAND_REGISTER 0x64
#define COMMAND_ARGUMENT 0x60
#define KBC_WRITE_COMMAND 0x60
#define KBC_READ_COMMAND 0x20
#define MOUSE_WRITE_COMMAND 0xD4
#define ACK 0xFA
#define NACK 0xFE
#define ERROR 0xFC

/**@}*/
#endif
