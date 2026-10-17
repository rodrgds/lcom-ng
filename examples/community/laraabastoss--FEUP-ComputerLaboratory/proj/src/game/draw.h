#ifndef _LCOM_DRAW_H_
#define _LCOM_DRAW_H_


#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "sprites/sprite_management.h"
#include "sprites/sprite.h"
#include "handle_int.h"

/// @brief draws a new frame depending on the current state of the game
void new_frame();

/// @brief draw the initial menu
void initial_menu();

/// @brief draws the game screen
void game();

/// @brief draws the menu for when the user wins a level
void won_menu();

/// @brief draws the menu for when the user loses a level
void lost_menu();

/// @brief draws the menu for when, in the multiplayer mode, the first player is waiting for the second one to connect
void wait_menu();


/// @brief displays the game date (day, month year, hour and minute) according to the data received by the RTC
void display_time();

/// @brief sets the frame buffers
/// @param mode submode desired
/// @return o in case of success and 0 otherwise
int  set_frame_buffers(uint16_t mode);

#endif

