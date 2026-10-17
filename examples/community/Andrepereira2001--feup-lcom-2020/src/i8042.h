#ifndef _LCOM_I8042_H
#define _LCOM_I8042_H


#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 keyboard and mouse. 
 */

#define KBC_IRQ           1  /**< @brief keyboard irq line */ 
#define MOUSE_IRQ         12 /**< @brief mouse irq line */ 

#define KBC_STATUS_REG    0x64 /**< @brief adress of the kbc status register */ 
#define KBC_COMAND_REG    0x64 /**< @brief adress of the kbc command register */ 
#define OUT_BUFFER        0x60 /**< @brief adress of the kbc output_buffer register */ 
#define IN_BUFFER         0x60 /**< @brief adress of the kbc input_buffer register */ 

#define ST_PARITY         BIT(7) /**< @brief status register parity bite */ 
#define ST_TIMEOUT        BIT(6) /**< @brief status register timeout bite */
#define ST_AUX            BIT(5) /**< @brief status register aux bite */
#define ST_IBF            BIT(1) /**< @brief status register input_buffer full bite */
#define ST_OBF            BIT(0) /**< @brief status register output_buffer full bite */

#define BREAK_CODE        BIT(7) /**< @brief break_code bite */

#define READ_CMD_BYTE     0x20   /**< @brief read command */
#define WRITE_CMD_BYTE    0x60   /**< @brief write command */
#define ENABLE_KBD_INTER  0xAE   /**< @brief enable kbc interrupts command */
#define DIS_KBC_INTER     0xAD   /**< @brief disable kbc interrupts command */

#define DELAY             20000  /**< @brief nice value to use as delay*/

//MOUSE COMMANDS

#define Y_OVERFLOW        BIT(7)  /**< @brief mouse y overflow bite*/
#define X_OVERFLOW        BIT(6)  /**< @brief mouse x overflow bite*/
#define MSB_Y_DELTA       BIT(5)  /**< @brief mouse y most significant bite */
#define MSB_X_DELTA       BIT(4)  /**< @brief mouse x most significant bite */
#define ALWAYS_1          BIT(3)  /**< @brief mouse always 1 bite */
#define MOUSE_BUTTON      BIT(2)  /**< @brief mouse middle button pressed */
#define RIGHT_BUTTON      BIT(1)  /**< @brief right middle button pressed */
#define LEFT_BUTTON       BIT(0)  /**< @brief left middle button pressed */

#define DIS_MOUSE         0xA7    /**< @brief disable mouse interrupts */
#define EN_MOUSE          0xA8    /**< @brief enable mouse interrupts */  
#define WRITE_B_TO_MOUSE  0xD4    /**< @brief write byte to mouse command */
#define DIS_DATA_REPORT   0xF5    /**< @brief disable data report byte to mouse command */
#define EN_DATA_REPORT    0xF4    /**< @brief enable data report byte to mouse command */

//arguments of 0xDa   
#define READ_DATA         0xEB    /**< @brief read data command */
#define SET_STREAM_MODE   0xEA    /**< @brief set mouse stream mode */

#define ACK               0xFA    /**< @brief valid acknowledge */
#define NACK              0xFE    /**< @brief not valid acknowledge */
#define ERROR             0xFC    /**< @brief error acknowledge */

#define N_BYTES_TO_READ   3       /**< @brief number of bytes to form the mouse packet */

#define MAKE_KEY_A        0x1E    /**< @brief make code of A*/
#define BREAK_KEY_A       0x9E    /**< @brief break code of A*/
#define MAKE_KEY_D        0x20    /**< @brief make code of D*/
#define BREAK_KEY_D       0xA0    /**< @brief break code of D*/
#define MAKE_KEY_W        0x11    /**< @brief make code of W*/
#define BREAK_KEY_W       0x91    /**< @brief break code of W*/
#define MAKE_KEY_S        0x1F    /**< @brief make code of S*/
#define BREAK_KEY_S       0x9F    /**< @brief break code of S*/

#define MAKE_ESC_DOWN     0x81    /**< @brief make code of ESC*/
#define MAKE_ENTER_DOWN   0x1C    /**< @brief make code of ENTER*/

#define MAKE_KEY_UP       0x48    /**< @brief make code of UP arrow*/
#define BREAK_KEY_UP      0xC8    /**< @brief break code of UP arrow*/
#define MAKE_KEY_DOWN     0x50    /**< @brief make code of DOWN arrow*/
#define BREAK_KEY_DOWN    0xD0    /**< @brief break code of DOWN arrow*/
#define MAKE_KEY_LEFT     0x4B    /**< @brief make code of RIGHT arrow*/
#define BREAK_KEY_LEFT    0xCB    /**< @brief break code of RIGHT arrow*/
#define MAKE_KEY_RIGHT    0x4D    /**< @brief make code of LEFT arrow*/
#define BREAK_KEY_RIGHT   0xCD    /**< @brief break code of LEFT arrow*/


#endif
