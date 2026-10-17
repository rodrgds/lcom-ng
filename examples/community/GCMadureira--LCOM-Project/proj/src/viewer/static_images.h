/** 
 * @file static_images.h
 * @brief Header file for @ref static_images.c
 * 
 * This file declares all the functions to be used for the static images, as well as the static image global variables themselves
 */


#ifndef _STATIC_IMAGES_PROJ_H_
#define _STATIC_IMAGES_PROJ_H_

#include <lcom/lcf.h>

extern xpm_image_t start_background_img;       ///< The starting menu background image
extern xpm_image_t game_background_img;        ///< The arena background image
extern xpm_image_t game_over_background_img;   ///< The sgame over menu background image
extern xpm_image_t start_selected_img;         ///< The start option selected image
extern xpm_image_t quit_selected_img;          ///< The quit option selected image
extern xpm_image_t play_again_selected_img;    ///< The play again option selected image
extern xpm_image_t back_to_menu_selected_img;  ///< The back to menu option selected image
extern xpm_image_t cursor_img;                 ///< The image for the cursor
extern xpm_image_t health_bars[5];             ///< Array for the health bar images
extern xpm_image_t number_sprites[10];         ///< Array for number sprites 0-9
extern xpm_image_t two_points;                 ///< The image for the character :
extern xpm_image_t pr_img;                     ///< The letters shown in the personal record

/**
 * @brief Loads all the static image xpms to the gloabl variables
 * 
 * Also sets up the global arrays of images.
 * 
 * @return 0 in case of success, 1 otherwise
 */
int (static_images_load)();

#endif /* _STATIC_IMAGES_PROJ_H_ */
