#ifndef NIGHT_MODE_H
#define NIGHT_MODE_H

#include <lcom/lcf.h>
#include "our_framework/framework_essencials.h"

bool* night_mode_filter;

/**
 * @brief starts the 'night mode'
 * 
 */
void start_night_mode();

/**
 * @brief gets the number to multiply the pixels by from the RTC
 * 
 * @return double value to multiply the pixels by
 */
double get_value_to_multiply();

/**
 * @brief applies the 'night mode' to the pixmap
 * 
 * @param pixmap pixmap to apply the 'night mode' to
 * @param img image to apply the 'night mode' to
 */
void apply_night_mode_on_xpm(uint8_t* pixmap, xpm_image_t img);

/**
 * @brief highlights the circle with center in (x,y) and radius 'range' in the pixmap
 * 
 * @param x x coordinate of the center of the circle
 * @param y y coordinate of the center of the circle
 * @param range radius of the circle
 * @param pixmap pixmap to highlight the circle in
 * @param img image to highlight the circle in
 */
void highlight_map(int x, int y, int range, uint8_t* pixmap, xpm_image_t img);

#endif
