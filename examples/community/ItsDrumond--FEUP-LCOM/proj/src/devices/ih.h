/**
 * @file ih.h
 * 
 */

#ifndef _LCOM_IH_H_
#define _LCOM_IH_H_


#include <stdio.h>

// Include menu
#include "game/menu.h"
#include "Graphic.Macros.h"

// Include devices
#include "timer.h"
#include "keyboard.h"
#include "i8042.h"
#include "video.h"
#include "rtc.h"

// Include models 
#include "models/player.h"
#include "models/bullet.h"
#include "models/helicopter.h"
#include "models/platforms.h"

// Include views
#include "view/canvas.h"

// Include xpm
#include "../xpm/xpm_sprites.h"


/**
 * @brief This method initializes the game
 * 
 * @return Returns an int value of zero if successful otherwise returns 1
 */
int init_game();

/**
 * @brief This method handles the interrupts for the game
 * 
 * @return Returns an int value of zero
 */
int proj_int();

/**
 * @brief This method closes the game
 * 
 * @return Returns vg_exit()
 */
int close_game();

/**
 * @brief This method handles the timer interrupts
 * 
 */
void timer_int_h();

/**
 * @brief This method handles the keyboard interrupts
 * 
 */
void kbc_int_h();

/**
 * @brief This method handles the mouse interrupts
 * 
 */
void mouse_int_h();



#endif // _LCOM_IH_H_
