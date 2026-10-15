#ifndef WATCH_H
#define WATCH_H

#include "../macros/includes.h"
#include "../load/view.h"

/**
 * @brief Calls drawing function to draw watch
 */
void draw_watch();

/**
 * @brief Sets the current inventory position to a watch.
 * @param index Current inventory index position
 * 
 */
void get_watch(int index);

/**
 * @brief Calls number drawing function to draw values of rtc
 */
int draw_rtc();


#endif

