/**
 * @file canvas.h
 * 
 */

#ifndef _LCOM_CANVAS_H_
#define _LCOM_CANVAS_H_

#include "../devices/video.h"
#include "../models/player.h"
#include "../models/helicopter.h"
#include "../devices/values.h"

/**
 * @brief This method draws the game main menu
 * 
 */
int (canvas_draw_menu)(Mouse *mouse);

/**
 * @brief This method draws the game instructions menu
 * 
 */
int (canvas_draw_instructions)(Mouse *mouse);

/**
 * @brief This method draws the game over menu
 * 
 */
int (canvas_draw_game_over)(Mouse *mouse);

/**
 * @brief This method draws the background image
 * 
 */
void (draw_background)(bool day);

/**
 * @brief This method draws the mouse cursor
 * 
 * @param mouse Pointer to the mouse
 */
void draw_mouse(Mouse *mouse);

/**
 * @brief This method draws the numbers
 * 
 * @param number Number to draw
 * @param x The x coordinate of the position to draw the number
 * @param y The y coordinate of the position to draw the number
 * @return Returns an int value of zero upon success and one otherwise
 */
int draw_numbers(uint8_t number,uint16_t x, uint16_t y);

/**
 * @brief This method draws the month
 * 
 * @param month Month to draw
 * @param x The x coordinate of the position to draw the month
 * @param y The y coordinate of the position to draw the month
 * @return Returns an int value of zero upon success and one otherwise
 */
int draw_month(uint8_t month, uint16_t x, uint16_t y);

/**
 * @brief This method draws the double dots
 * 
 * @param x The x coordinate of the position to draw the double dots
 * @param y The y coordinate of the position to draw the double dots
 * @return Returns an int value of zero upon success and one otherwise
 */
int draw_double_dots(uint16_t x, uint16_t y);

/**
 * @brief This method draw the date
 * 
 * @param hours Hour to draw
 * @param minutes Minutes to draw
 * @param seconds Seconds to draw
 * @param day Day to draw
 * @param month Month to draw
 * @return Returns an int value of zero upon success and one otherwise
 */
int draw_date(uint8_t hours, uint8_t minutes,uint8_t seconds, uint8_t day, uint8_t month);



#endif // _LCOM_CANVAS_H_
