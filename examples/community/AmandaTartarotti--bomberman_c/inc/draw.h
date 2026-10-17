#ifndef _LCOM_DRAW_H_
#define _LCOM_DRAW_H_


#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "graphic.h"
#include "sprite.h"

/**
     * @brief Draw xpm's from the game map according to the different symbols current in the map.
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int draw_map();
/**
     * @brief Draw xpm's from the mouse arrow.
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int arrow_draw();
/**
     * @brief Draw xpm's from numbers to display the hour, minutes and seconds of the day in the screen.
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int create_clock();
/**
     * @brief Erase everything inside the screen resolution by coloring the screen black.
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int erase_screen();
/**
     * @brief Draw xpm's from the gameover page.
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int draw_gameover();
/**
     * @brief Draw xpm's from the menu page.
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int draw_menu();
/**
     * @brief Draw xpm's from the instructions page.
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int draw_instructions();
/**
     * @brief Draw xpm's from the winner page.
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int draw_gamewin();

#endif /* __DRAW_H */
