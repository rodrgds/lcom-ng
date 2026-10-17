#ifndef __MENU_H
#define __MENU_H

#include "bitmap.h"

Bitmap* Zero;
Bitmap* One;
Bitmap* Two;
Bitmap* Three;
Bitmap* Four;
Bitmap* Five;
Bitmap* Six;
Bitmap* Seven;
Bitmap* Eight;
Bitmap* Nine;
Bitmap* Points;

///@brief draws the bitmap that contains the main menu
void draw_main_menu();

///@brief presents the menu of the multiplayer mode
///@param irq_set_timer
///@param irq_set_keyboard
///@param irq_set_mouse
void multi_menu(int irq_set_timer,int irq_set_keyboard,int irq_set_mouse);

///@brief draws the bitmap that contains the multiplayer mode menu
void draw_multi_menu();

///@brief deletes the bitmaps related to the timer
void delete_relogio();

///@brief loads the bitmaps related to the timer
void load_relogio();

#endif
