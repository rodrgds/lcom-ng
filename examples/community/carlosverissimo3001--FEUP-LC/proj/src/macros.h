#ifndef _LCOM_MACROS_H_
#define _LCOM_MACROS_H_

#include <lcom/lcf.h>

/** @defgroup macros
 * @{
 *
 * Constants to be used in the project
 */

/* THIS FILE CONTAINS MACROS FROM THE FOLLOWING FILES
 * i8524.h
 * i8042.h
 * vg_macros.h
*/


/*******************************************/
/*                                         */
/*          Timer-related macros           */
/*                                         */
/*******************************************/

#define TIMER_FREQ 1193182 /**< @brief clock frequency for timer in PC and AT */
#define TIMER0_IRQ 0 /**< @brief Timer 0 IRQ line */
#define TIMER_MIN_FREQ 19 /**< @brief Mininum value that, when divided by TIMER_FREQ, fits within 16 bits */

/* I/O port addresses */

#define TIMER_0    0x40 /**< @brief Timer 0 count register */
#define TIMER_1    0x41 /**< @brief Timer 1 count register */
#define TIMER_2    0x42 /**< @brief Timer 2 count register */
#define TIMER_CTRL 0x43 /**< @brief Control register */

#define SPEAKER_CTRL 0x61 /**< @brief Register for speaker control */

/* Timer control */

/* Timer selection: bits 7 and 6 */

#define TIMER_SEL0   0x00              /**< @brief Control Word for Timer 0 */
#define TIMER_SEL1   BIT(6)            /**< @brief Control Word for Timer 1 */
#define TIMER_SEL2   BIT(7)            /**< @brief Control Word for Timer 2 */
#define TIMER_RB_CMD (BIT(7) | BIT(6)) /**< @brief Read Back Command */

/* Register selection: bits 5 and 4 */

#define TIMER_LSB     BIT(4)                  /**< @brief Initialize Counter LSB only */
#define TIMER_MSB     BIT(5)                  /**< @brief Initialize Counter MSB only */
#define TIMER_LSB_MSB (TIMER_LSB | TIMER_MSB) /**< @brief Initialize LSB first and MSB afterwards */

/* Operating mode: bits 3, 2 and 1 */

#define TIMER_SQR_WAVE (BIT(2) | BIT(1)) /**< @brief Mode 3: square wave generator */
#define TIMER_RATE_GEN BIT(2)            /**< @brief Mode 2: rate generator */

/* Counting mode: bit 0 */

#define TIMER_BCD 0x01 /**< @brief Count in BCD */
#define TIMER_BIN 0x00 /**< @brief Count in binary */

/* READ-BACK COMMAND FORMAT */

#define TIMER_RB_COUNT_  BIT(5)
#define TIMER_RB_STATUS_ BIT(4)
#define TIMER_RB_SEL(n)  BIT((n) + 1)

/* USEFUL FOR BYTE EXTRACTION */
#define EXTRACT_LSB 0x00FF  /**< @brief Used to get the LSB of a 2 byte value */
#define EXTRACT_MSB 0xFF00  /**< @brief Used to get the MSB of a 2 byte value */

#define TIMER_OPER_MODE (BIT(3) | BIT(2) | BIT(1))
#define TIMER_4LSB_MASK (TIMER_OPER_MODE | BIT(0))
#define TIMER_SEL_SHIFT(n) (n << 6)


/*******************************************/
/*                                         */
/* KBC (Keyboard and Mouse) related macros */
/*                                         */
/*******************************************/

#define KBC_IRQ 1                 /**< @brief Keyboard controller IRQ line */
#define MOUSE_IRQ 12              /**< @brief Mouse IRQ line */
#define KBC_DELAY 20000           /**< @brief Delay */
#define MAX_TRIES 30              /**< @brief Maximum tries to write a value */

/* The KBC registers */

#define KBC_IN_BUF      0x60      /**< @brief Keyboard controller input buffer */
#define KBC_OUT_BUF     0x60      /**< @brief Keyboard controller output buffer */
#define KBC_ST_REG      0x64      /**< @brief KBC status register */
#define KBC_CMD_REG     0x64      /**< @brief KBC command byte register */

/* Status register bits */

#define KBC_PAR_ERR     BIT(7)    /**< @brief Parity error - invalid data */
#define KBC_TO_ERR      BIT(6)    /**< @brief Timeout error - invalid data */
#define KBC_MOUSE_DATA  BIT(5)    /**< @brief Mouse data */
#define KBC_IBFULL      BIT(1)    /**< @brief Input buffer full - dont write commands or arguments*/
#define KBC_OBFULL      BIT(0)    /**< @brief Output buffer full - data avaliabe for reading' */

/* Keyboard-Related KBC Commands for PC-AT/PS2 - ALL WRITEN USING THE ADRESS 0X64*/

#define KBC_SEND_CMD   0x64       /**< @brief Issue a command to the KBC */
#define KBC_SEND_ARG   0x60       /**< @brief Arguments of the command must be passed here */
#define KBC_READ_RET   0x60       /**< @brief Return values must be read from here */

#define KBC_READ_CMD_BYTE  0x20   /**< @brief Read command byte - Return value: command byte */
#define KBC_WRITE_CMD_BYTE 0x60   /**< @brief Write command byte - Argument: command byte */
#define KBC_DIS_KBD_INT    0xAD   /**< @brief Disable KBD Interface */
#define KBC_ENA_KBD_INT    0xAE   /**< @brief Enable KBD Interface */

/* KBC Command byte bits */

#define KBC_DIS_MOUSE_INT     BIT(5) /**< @brief Disable Mouse interface */
#define KBC_DIS_KBD_INT_BIT   BIT(4) /**< @brief Disable KBD interface */
#define KBC_ENA_INT_MOUSE     BIT(1) /**< @brief Enable interrupt on OBF, from mouse */
#define KBC_ENA_INT_KBD       BIT(0) /**< @brief Enable interrupt on OBF, from keyboard */


/* Some useful codes */

#define ESC_BRK_CODE    0x81      /**< @brief ESC key break code */
#define TWO_BYTE_SCODE  0xE0      /**< @brief Two-byte scandode, have 0xE0 as their first byte */
#define ENTER_BRK_CODE  0x9C      /**< @brief ENTER key break code */
#define BACKSPACE_BRK_CODE 0x8E   /**< @brief BACKSPACE key break code */
#define SPACE_BRK_CODE  0xB9      /**< @brief SPACE key break code */
#define NUMBER_1_BRK_CODE 0x82    /**< @brief 1 key break code */
#define NUMBER_2_BRK_CODE 0x83    /**< @brief 2 key break code */
#define NUMBER_3_BRK_CODE 0x84    /**< @brief 3 key break code */

/* Useful combination of macros */
#define KBC_CHECK_ERR   (KBC_PAR_ERR | KBC_TO_ERR)      /**< @brief Mask to check for both errors */


/* MOUSE SPECIFIC MACROS */

#define Y_OVF                 BIT(7)            /**< @brief Vertical overflow bit */
#define X_OVF                 BIT(6)            /**< @brief Horizontal overflow bit */
#define MSB_DELTA_Y           BIT(5)            /**< @brief Most significant bit of vertical movement(signal) */
#define MSB_DELTA_X           BIT(4)            /**< @brief Most significant bit of horizontal movement(signal) */
#define IS_FIRST_BYTE         BIT(3)            /**< @brief Checks if a given byte is the first one of the packet */
#define MIDDLE_BUTTON         BIT(2)            /**< @brief Middle button bit */
#define RIGHT_BUTTON          BIT(1)            /**< @brief Right button bit */
#define LEFT_BUTTON           BIT(0)            /**< @brief Left button bit */

#define WRITE_BYTE_TO_MOUSE   0xD4 /**< @brief instructs the KBC to forward the arg directly to the mouse without any interpretation */


/* KBC MOUSE RELATED COMMANDS */
#define DIS_DATA_REPORT       0xF5  /**< @brief In stream mode, sent before any other command */
#define ENA_DATA_REPORT       0xF4  /**< @brief In stream mode only */
#define SET_STREAM_MODE       0xEA  /**< @brief Send data on events */

/* Acknowledgement bytes */

#define ACK 0xFA /**< @brief if everything OK */
#define NACK 0xFE /**< @brief if invalid byte (may be because of a serial communication error) */
#define ERROR 0xFC /**< @brief second consecutive invalid byte */


/*******************************************/
/*                                         */
/*        Video Card related macros        */
/*                                         */
/*******************************************/

#define BIOS_ACESS_GRAPHICS 0x10     /**< @brief BIOS service interrupt vector for graphics card> */

/* VBE MODES */
#define INDEXED_MODE    0x105 /**< @brief 1024*768, 8 bits per pixel */
#define DIRECT_MODE_15  0x110 /**< @brief 640*480, 15 bits per pixel ((1:)5:5:5) */
#define DIRECT_MODE_24  0x115 /**< @brief 800x600, 24 bits per pixel (8:8:8) */
#define DIRECT_MODE_16M 0x11B /**< @brief 1280x1024 16.8M colors, 24 bits per pixel (8:8:8) */
#define DIRECT_MODE_16  0x11A /**< @brief 1280x1024, 16 bits per pixel (5:6:5) */
#define DIRECT_MODE_32  0x14C /**< @brief 1152x864, 32 bits per pixel ((8:)8:8:8) */

/* Values passed toreg86_t registers */

#define GET_CONTROLLER_INFO 0X4F00     /**< @brief VBE call, function 00 -- Return Controller Info */
#define GET_VBE_MODE        0x4F01     /**< @brief VBE call, function 01 -- Return VBE Mode Info */
#define SET_VBE_MODE        0x4F02     /**< @brief VBE call, function 02 -- set VBE mode */

#define FUNCTION_SUPPORTED 0x4F       /**< @brief al register returns 0x4f if the VBE function is supported */
#define FUNCTION_CALL_FAIL 0x01       /**< @brief If the VBE function call failed */
#define FUNCTION_NOT_SUPPORTED_BY_HW 0x02   /**< @brief If the VBE function call is not suppported by hw */
#define FUNCTION_INV_IN_CURR_VIDEO_MODE 0X03    /**< @brief If the VBE function is invalid in the current video mode */

#define LINEAR_BUFFER       BIT(14)    /**< @brief bit 14: linear framebuffer */
#define TRANSPARENCY 0x00B140


/*******************************************/
/*                                         */
/*           RTC related macros            */
/*                                         */
/*******************************************/

#define RTC_IRQ 8         /**< @brief U RTC irq line */
#define RTC_ADDR_REG 0x70 /**< @brief Must be loaded with the address of the RTC register to be accessed */
#define RTC_DATA_REG 0x71 /**< @brief Used to transfer the data to/from the RTC’s register accessed */

#define SECONDS_REG 0x00        /**< @brief RTC seconds register */
#define MINUTES_REG 0x02        /**< @brief RTC minutes register */
#define HOURS_REG   0x04        /**< @brief RTC hours register */
#define DAY_OF_WEEK_REG 0X06    /**< @brief RTC day of the week register */
#define DAY_REG 0X07            /**< @brief RTC day register */
#define MONTH_REG 0X08          /**< @brief RTC month register */
#define YEAR_REG 0x09           /**< @brief RTC year register */

#define RTC_REG_A 0x0A          /**< @brief RTC A register */
#define RTC_REG_B 0x0B          /**< @brief RTC B register */
#define RTC_REG_C 0x0C          /**< @brief RTC C register */
#define RTC_REG_D 0x0D          /**< @brief RTC D register */


/* Some useful color definitions */
#define WHITE 0xffffff          /**< @brief White rgb value */
#define BLACK 0x000000          /**< @brief Black rgb value */
#define RED   0xff0000          /**< @brief Red rgb value */
#define GREEN 0x00ff00          /**< @brief Green rgb value */
#define BLUE  0x0000ff          /**< @brief Blue rgb value */
#define YELLOW 0xffff00         /**< @brief Yellow rgb value */
#define SIDEBAR_GREY 0X989898   /**< @brief Grey used in the sidebar rgb value */
#define DARK_MODE_GREY 0X161617 /**< @brief Grey, used in dark mode background, rgb value */


#define MINUTES(n) (60 * n)             /**< @brief Minutes definition -> usage: MINUTES(5) == 5 minutes */

#define MAX_WORD_SIZE 10                /**< @brief Maximum word size possible */
#define MAX_GUESSES 3                   /**< @brief Maximum guesses possible */
#define DRAWING_TIME 60                 /**< @brief Time to draw */
#define ANALYSE_TIME (DRAWING_TIME/2)   /**< @brief Time to analyse */

#endif /* _LCOM_MACROS_H */
