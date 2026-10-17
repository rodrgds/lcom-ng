#ifndef __NUMBER_H
#define __NUMBER_H

#include <lcom/lcf.h>
#include "graphics.h"
#include "Sprite.h"
#include "macros.h"
#include "rtc.h"


#include "img/xpms/numbers/number0.xpm"
#include "img/xpms/numbers/number1.xpm"
#include "img/xpms/numbers/number2.xpm"
#include "img/xpms/numbers/number3.xpm"
#include "img/xpms/numbers/number4.xpm"
#include "img/xpms/numbers/number5.xpm"
#include "img/xpms/numbers/number6.xpm"
#include "img/xpms/numbers/number7.xpm"
#include "img/xpms/numbers/number8.xpm"
#include "img/xpms/numbers/number9.xpm"
#include "img/xpms/numbers/separator.xpm"

/** @defgroup numbers
 * @{
 *
 *
 *
 * Functions developped for all the numerical elements displayed on the screen
 */

/**
 * @brief Loads the numbers from 1 to 10 as well as time separator (:)
*/
void (load_numbers)();

/**
 * @brief Given the time remaining, displays it on the screen
 * @param time_left The time to display
 */
void(display_time_left)(int time_left);

/**
 * @brief Given a number, displays it on the screen. Used to display remaining guesses
 * @param number The number to display
 * @param xpos The x position of the number
 * @param ypos The y position of the number
 * @param color The color of the number
 */
void(draw_number)(int number, int xpos, int ypos, uint32_t color);

/**
 * @brief Given a time in the hh:mm:ss format, displays it on the screen
 * @param Time time data structure
 * @return Return 0 upon success and non-zero otherwise
 */
int (print_time)(Time time);


#endif /* __NUMBER_H */

