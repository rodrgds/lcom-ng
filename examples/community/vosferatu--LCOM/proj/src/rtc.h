#ifndef __RTC_H
#define __RTC_H

#include "utils.h"
#include "bitmap.h"

/** @defgroup rtc rtc
 * @{
 *
 * Functions for controlling the rtc and date
 */

#define RTC_IRQ 	  	8		/**< @brief rtc's irq */
#define RTC_ADDR_REG  	0x70	/**< @brief rtc send command */
#define RTC_DATA_REG  	0x71	/**< @brief rtc receive data */

#define SECONDS 		0		/**< @brief rtc seconds command */
#define MINUTES 		2		/**< @brief rtc minutes command */
#define HOURS 			4			/**< @brief rtc hours command */

#define REG_A 			10	/**< @brief rtc register A 0x0A */
#define REG_B 			11	/**< @brief rtc register B 0X0B */
#define REG_C 			12	/**< @brief rtc register C 0X0C */
#define REG_D 			13	/**< @brief rtc register D 0X0D */

#define RTC_UIP 		BIT(7)		/**< @brief rtc uip reg B*/
#define RTC_UF 			BIT(4)		/**< @brief rtc uf reg C */

/*! Represents the shield */
typedef struct{
	int hours;
	int minutes;

	int x, y, w, h;

	Bitmap* num0;
	Bitmap* num1;
	Bitmap* num2;
	Bitmap* num3;
	Bitmap* num4;
	Bitmap* num5;
	Bitmap* num6;
	Bitmap* num7;
	Bitmap* num8;
	Bitmap* num9;
	Bitmap* dots;
} Date;


/**
 * @brief creates the Date type pointer
 */
void newDate();

/**
 * @brief draws the date on the screen
 */
void drawDate();

/**
 * @brief retrieves date pointer
 * @return returns date pointer
 */
Date* getDate();

/**
 * @brief draws a date number on the screen
 * @param number number to be drawn
 * @param x coordinate X of where to draw
 * @param y coordinate Y of where to draw
 */
void drawSmall(int number, int x, int y);


void deleteDate();

/**
 * @brief unsubscribes rtc interrupts
 * @return 0 upon success, non-zero otherwise
 */
int rtc_unsubscribe_int();

/**
 * @brief subscribes rtc interrupts
 * @return 0 upon success, non-zero otherwise
 */
int rtc_subscribe_int();

/**
 * @brief transform bcd number to a binary
 * @param bcd binary coded decimal number
 * @return the binary equivalent
 */
unsigned char bcdToBinary(unsigned char bcd);

/**
 * @brief enables rtc updates throw reg B SET bit
 * @return 0 upon success, non-zero otherwise
 */
int rtc_enable();

/**
 * @brief disables rtc updates throw reg B SET bit
 * @return 0 upon success, non-zero otherwise
 */
int rtc_disable();

/**
 * @brief scans rtc for current time upon interrupt; validates information first
 */
void rtc_int_handler();

/** @} */

#endif /* __RTC.H */
