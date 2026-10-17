#ifndef UTILS_H
#define UTILS_H

/** @file */

/** @defgroup Utils Utils
 * @{
 * Useful defined macros
 */

/* -------------------------------------------- GENERAL ---------------------------------------------------------*/

#define BIT(n) 							(0x01<<(n))
#define DELAY_US 						10000
#define DEFAULT_FREQUENCY 				60

#define TRUE 							1
#define FALSE 							0

/* -------------------------------------------- PROJECT ---------------------------------------------------------*/

#define BALLOONS_FIRST_FASE				15
#define BALLOONS_SECOND_FASE			15

#define STARTING_ARROWS					15
#define SECOND_LEVEL_ARROW_NUMBER		30

#define PAGEUP_MAKE_CODE 				0xE049

#define HERO_STARTING_POS_X				80
#define HERO_STARTING_POS_Y				384
#define SPACE_MAKE_CODE					0x39
#define SPACE_BREAK_CODE				0xB9

#define ARROW_UP_MAKECODE				0xE048
#define ARROW_DOWN_MAKECODE				0xE050
#define ENTER_BREAKCODE					0x9C
#define ENTER_MAKECODE					0x1C

#define LOST							5
#define WON								4
#define FIRST_STAGE 					1
#define SECOND_STAGE 					2

/* -------------------------------------------- GLOBAL VARIABLES ---------------------------------------------------------*/

int hook_id;
int mouseCounter;
int arrow_size;
int timer_counter;
int current_game_level;
int next_level_timer;
int end_game_timer;

/* -------------------------------------------- TIMER ---------------------------------------------------------*/

#define TIMER_NOTIFICATION 				15
#define TIMER0_IRQ	       				0	/**< @brief Timer 0 IRQ line */

#define TIMER_FREQ  					1193182	/**< @brief clock frequency for timer in PC and AT */

/* I/O port addresses */

#define TIMER_0							0x40 /**< @brief Timer 0 count register */
#define TIMER_1							0x41 /**< @brief Timer 1 count register */
#define TIMER_2							0x42 /**< @brief Timer 2 count register */
#define TIMER_CTRL						0x43 /**< @brief Control register */
#define SPEAKER_CTRL					0x61 /**< @brief Register for speaker control  */

/* Timer control */

/* Timer selection: bits 7 and 6 */

#define TIMER_SEL0						0x00     /**< @brief Control Word for Timer 0 */
#define TIMER_SEL1						BIT(6)   /**< @brief Control Word for Timer 1 */
#define TIMER_SEL2						BIT(7)   /**< @brief Control Word for Timer 2 */
#define TIMER_RB_CMD					(BIT(7) | BIT(6))  /**< @brief Read Back Command */

/* Register selection: bits 5 and 4 */

#define TIMER_LSB						BIT(4)  /**< @brief Initialize Counter LSB only */
#define TIMER_MSB						BIT(5)  /**< @brief Initialize Counter MSB only */
#define TIMER_LSB_MSB					(TIMER_LSB | TIMER_MSB) /**< @brief Initialize LSB first and MSB afterwards */

/* Operating mode: bits 3, 2 and 1 */

#define TIMER_SQR_WAVE					(BIT(2)|BIT(1)) /**< @brief Mode 3: square wave generator */
#define TIMER_RATE_GEN					BIT(2)          /**< @brief Mode 2: rate generator */

/* Counting mode: bit 0 */

#define TIMER_BCD						0x01   /**< @brief Count in BCD */
#define TIMER_BIN						0x00   /**< @brief Count in binary */

/* READ-BACK COMMAND FORMAT */

#define TIMER_RB_COUNT_       			BIT(5)
#define TIMER_RB_STATUS_       	 		BIT(4)
#define TIMER_RB_SEL(n)         		BIT((n)+1)

/* -------------------------------------------- KEYBOARD ---------------------------------------------------------*/

#define KBD_NOTIFICATION 				16
#define KBD_IRQ 						1

#define KB_ECHO 						0xFE
#define KB_RESET 						0xFF

#define PAR_ERR							BIT(7)
#define TIMEOUT 						BIT(6)

/* SCAN */

#define ESC_BREAK_CODE 					0x81
#define KBD_TWO_BYTE 					0xE0

/* LEDS */

#define KBD_LED_CMD 					0xED
#define KBD_LED_ERROR	 				0xFC
#define ACK								0xFA
#define RESEND 							0xFE
#define ERROR 							0xFC

#define KBD_OUT_BUF 					0x60
#define KBD_IN_BUF 						0x64
#define IN_BUF							0x60
#define OUT_BUF							0x60

/* -------------------------------------------- MOUSE ---------------------------------------------------------*/

#define MOUSE_NOTIFICATION 				17
#define MOUSE_IRQ						12

#define MOUSE_WRITE_BYTE 				0xD4
#define MOUSE_STAT_REQUEST				0xE9
#define SET_STREAM_MODE 				0xEA
#define ENABLE_STREAM_MODE				0xF4
#define DISABLE_STREAM_MODE 			0xF5

/* MOUSE BYTE */

#define MOUSE_LEFT_BUTTON 				BIT(0)
#define MOUSE_RIGHT_BUTTON 				BIT(1)
#define MOUSE_MIDDLE_BUTTON 			BIT(2)
#define MOUSE_VERIFICATION 				BIT(3)
#define MOUSE_X_SIGN 					BIT(4)
#define MOUSE_Y_SIGN 					BIT(5)
#define MOUSE_X_OVERFLOW 				BIT(6)
#define MOUSE_Y_OVERFLOW 				BIT(7)

/* MOUSE CONFIG */

#define MOUSE_CONFIG_LEFT_BUTTON		BIT(2)
#define MOUSE_CONFIG_RIGHT_BUTTON		BIT(0)
#define MOUSE_CONFIG_MIDDLE_BUTTON		BIT(1)
#define MOUSE_CONFIG_SCALLING			BIT(4)
#define MOUSE_CONFIG_DATA_REPORTING		BIT(5)
#define MOUSE_CONFIG_MODE				BIT(6)

/* MOUSE SLOPE */

#define MAX_SLOPE						1.6
#define MIN_SLOPE						0.75

/* -------------------------------------------- GRAPHICS CARD ---------------------------------------------------------*/

/* VIDEO_GR */

#define VIDEO_GR_MODE_105				0x105
#define VIDEO_GR_MODE_103				0X103
#define VIDEO_GR_MODE_107				0x107
#define VIDEO_GR_MODE_101				0X101
#define VIDEO_GR_MODE_117				0x117

/* VBE */

#define VBE_INT10						0x10
#define VBE_AX							0x4F02
#define VBE_AH							0x4F
#define VBE_AL							0x01
#define VBE_SET_VIDEO_MODE 				0x00
#define VBE_SET_TEXT_MODE 				0x03

#define DAC								BIT(0)
#define VGA_COMPA						BIT(1)
#define RAMDAC							BIT(2)

#endif /**@}*/
