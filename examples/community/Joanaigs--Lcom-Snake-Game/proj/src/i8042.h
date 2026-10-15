#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @brief KBC IRQ Line */
#define KBC_IRQ     1   
 /** @brief Mouse IRQ Line */
#define MOUSE_IRQ   12  

/** @brief Delay */
#define DELAY           20000 

/** @brief Send commands to KBC */
#define CMD_KBC     0x64 
/** @brief Write to KBC Command */
#define CMD_ARG_KBC 0x60 
/** @brief Status Register*/
#define STATUS_REG  0x64 
/** @brief Output Buffer */
#define OUT_BUF  0x60 

/** @brief Read Command Byte */
#define READ_KBC    0x20 
/** @brief Write KBC Command */
#define WRITE_KBC   0x60 


/** @brief Output Buffer Full */
#define OBF     BIT(0) 
/** @brief Input Buffer Full */
#define IBF     BIT(1)
/** @brief Mouse Bit */
#define MOUSE_AUX       BIT(5) 
/** @brief Time Out Bit */
#define TOB    BIT(6) 
/** @brief Parity bit */
#define PARITY_BIT    BIT(7)

/** @brief ESC */
#define ESC_BREAK_CODE  0x81  
/**  @brief ENTER */  
#define ENTER_BREAK_CODE  0x1C 
/** @brief W */
#define W_MAKE_CODE 0x11  
/** @brief D */
#define D_MAKE_CODE 0x20  
/** @brief S */
#define S_MAKE_CODE 0x1f
/** @brief A */
#define A_MAKE_CODE 0x1e 
/** @brief UP */
#define UP_MAKE_CODE 0x48
/** @brief RIGHT */
#define RIGHT_MAKE_CODE 0x4d 
/** @brief DOWN */
#define DOWN_MAKE_CODE 0x50 
/** @brief LEFT */
#define LEFT_MAKE_CODE 0x4b
/** @brief First of a two byte scancode */
#define TWO_BYTE   0xE0    
/** @brief Make code or break code */
#define BREAK_CODE  BIT(7)

/** @brief Left button*/
#define LEFT_BUTTON     BIT(0) 
/** @brief Right button */
#define RIGHT_BUTTON    BIT(1)
/** @brief Middle button */
#define MIDDLE_BUTTON   BIT(2)
/** @brief X delta most significant bit  */
#define MSB_X_DELTA     BIT(4) 
/** @brief Y delta most significant bit  */
#define MSB_Y_DELTA     BIT(5) 
/** @brief X overflowed */
#define X_OVERFLOW      BIT(6)
/** @brief Y  overflowed */
#define Y_OVERFLOW      BIT(7) 

/** @brief Operation sucessful */
#define ACK      0xFA 

/** @brief Write to mouse */
#define WRITE_MOUSE 0xD4
/** @brief Disable date reporting */
#define DISABLE_DATA_REPORT 0xF5
/** @brief enable date reporting */
#define ENABLE_DATA_REPORT 0xF4

#endif /*_LCOM_I8042_H_ */
