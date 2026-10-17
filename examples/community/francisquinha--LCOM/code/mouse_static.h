#ifndef __MOUSE_STATIC_H
#define __MOUSE_STATIC_H

/** @file mouse_static.h
 * @brief file containing declarations of the static functions that deal with the game mouse
 */

/** @defgroup mouse_static mouse_static
 * @{
 *
 * @brief static functions for handling the mouse
 */

/**
 * @brief To read mouse replys from KBC
 *
 * @return mouse data
 */
static long mse_read();

/**
 * @brief To write mouse commands to the KBC
 *
 * @param port place to write the command to, may be KBC status register or KBC command register
 * @param cmd command we want to write
 *
 * @return 0
 */
static int mse_write(unsigned long cmd, unsigned long port);

/**
 * @brief to interpret the mouse packets and turn them into usable mouse information
 * @param mouse whose packets we want to parse
 */
static void get_mse_info(Mouse *mouse);

/** @} end of mouse_static */

#endif /* __MOUSE_STATIC_H */
