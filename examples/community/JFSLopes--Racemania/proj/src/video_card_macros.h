#ifndef _VIDEO_CARD_MACROS_H_
#define _VIDEO_CARD_MACROS_H_

#include <lcom/lcf.h>

#define NO_COLLISION 0 /*<@brief Indicates that the car didn´t collide*/
#define CRASH 1 /*<@brief Indicates if there was a carr collision */
#define FINISH_LINE 2 /*<@brief Indicates that the car has crossed the finish line */
#define GREEN 0x00ff00 /*<@brief Color of tracks´s grass (detect collision)*/
#define BLUE 0x0000ff /*<@brief Color of the finish line */
#define PURPLE 0x330033 /*<@brief Color used to detect if the car is going backwards */
#define TRANSPARENT_COLOR 0xF0F0F0 /*<@brief Color that will never be drawn*/

#endif
