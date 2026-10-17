#ifndef BACKGROUND_H
#define BACKGROUND_H
#include <lcom/lcf.h>
#include "videocard.h"


//https://ansimuz.itch.io/cyberpunk-street-environment
#include "../assets/background.xpm"



/**
 * @brief allocs the memory needed to display the background
 * 
 */
void alloc_background();

/**
 * @brief frees the memory that we alloced to display the background
 * 
 */
void free_background();


/**
 * @brief draws the background on the second buffer
 * 
 * @param init if its the first time we're drawing the background
 * @return int 0 in case it goes successfully, 1 otherwise
 */
int draw_background(bool init);

/**
 * @brief clears the screen and resets it to display the background
 * 
 */
void reset_to_background();


#endif 
