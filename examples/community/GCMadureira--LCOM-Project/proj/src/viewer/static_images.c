/** 
 * @file static_images.c
 * @brief Source code file for all the logic related to the static images
 */


#include "static_images.h"

xpm_image_t start_background_img;
xpm_image_t game_background_img;
xpm_image_t game_over_background_img;
xpm_image_t start_selected_img;
xpm_image_t quit_selected_img;
xpm_image_t play_again_selected_img;
xpm_image_t back_to_menu_selected_img;
xpm_image_t cursor_img;
xpm_image_t health_bars[5];
xpm_image_t number_sprites[10];
xpm_image_t two_points;
xpm_image_t pr_img;

int (static_images_load)() {
  #include "../resources/Static Images/static_images.h"

  if(xpm_load(start_background_xpm, XPM_8_8_8_8, &start_background_img) == NULL) return 1;
  if(xpm_load(game_background_xpm, XPM_8_8_8_8, &game_background_img) == NULL) return 1;
  if(xpm_load(game_over_background_xpm, XPM_8_8_8_8, &game_over_background_img) == NULL) return 1;
  if(xpm_load(start_selected_xpm, XPM_8_8_8_8, &start_selected_img) == NULL) return 1;
  if(xpm_load(quit_selected_xpm, XPM_8_8_8_8, &quit_selected_img) == NULL) return 1;
  if(xpm_load(play_again_selected_xpm, XPM_8_8_8_8, &play_again_selected_img) == NULL) return 1;
  if(xpm_load(back_to_menu_selected_xpm, XPM_8_8_8_8, &back_to_menu_selected_img) == NULL) return 1;
  if(xpm_load(cursor_xpm, XPM_8_8_8_8, &cursor_img) == NULL) return 1;
  if(xpm_load(pr_xpm, XPM_8_8_8_8, &pr_img) == NULL) return 1;

  xpm_image_t health_0_img, health_1_img, health_2_img, health_3_img, health_4_img;
  if(xpm_load(health_4_xpm, XPM_8_8_8_8, &health_4_img) == NULL) return 1;
  if(xpm_load(health_3_xpm, XPM_8_8_8_8, &health_3_img) == NULL) return 1;
  if(xpm_load(health_2_xpm, XPM_8_8_8_8, &health_2_img) == NULL) return 1;
  if(xpm_load(health_1_xpm, XPM_8_8_8_8, &health_1_img) == NULL) return 1;
  if(xpm_load(health_0_xpm, XPM_8_8_8_8, &health_0_img) == NULL) return 1;

  health_bars[0] = health_0_img;
  health_bars[1] = health_1_img;
  health_bars[2] = health_2_img;
  health_bars[3] = health_3_img;
  health_bars[4] = health_4_img;


  if(xpm_load(zero_xpm, XPM_8_8_8_8, &number_sprites[0]) == NULL) return 1;
  if(xpm_load(one_xpm, XPM_8_8_8_8, &number_sprites[1]) == NULL) return 1;
  if(xpm_load(two_xpm, XPM_8_8_8_8, &number_sprites[2]) == NULL) return 1;
  if(xpm_load(three_xpm, XPM_8_8_8_8, &number_sprites[3]) == NULL) return 1;
  if(xpm_load(four_xpm, XPM_8_8_8_8, &number_sprites[4]) == NULL) return 1;
  if(xpm_load(five_xpm, XPM_8_8_8_8, &number_sprites[5]) == NULL) return 1;
  if(xpm_load(six_xpm, XPM_8_8_8_8, &number_sprites[6]) == NULL) return 1;
  if(xpm_load(seven_xpm, XPM_8_8_8_8, &number_sprites[7]) == NULL) return 1;
  if(xpm_load(eight_xpm, XPM_8_8_8_8, &number_sprites[8]) == NULL) return 1;
  if(xpm_load(nine_xpm, XPM_8_8_8_8, &number_sprites[9]) == NULL) return 1;

  if(xpm_load(two_points_xpm, XPM_8_8_8_8, &two_points) == NULL) return 1;

  return 0;
}
