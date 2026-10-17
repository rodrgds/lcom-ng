#include "../headers/images.h"

void load_all_images() {
  digits[0].bytes = xpm_load(zero_xpm, XPM_8_8_8_8, &digits[0]);
  digits[1].bytes = xpm_load(one_xpm, XPM_8_8_8_8, &digits[1]);
  digits[2].bytes = xpm_load(two_xpm, XPM_8_8_8_8, &digits[2]);
  digits[3].bytes = xpm_load(three_xpm, XPM_8_8_8_8, &digits[3]);
  digits[4].bytes = xpm_load(four_xpm, XPM_8_8_8_8, &digits[4]);
  digits[5].bytes = xpm_load(five_xpm, XPM_8_8_8_8, &digits[5]);
  digits[6].bytes = xpm_load(six_xpm, XPM_8_8_8_8, &digits[6]);
  digits[7].bytes = xpm_load(seven_xpm, XPM_8_8_8_8, &digits[7]);
  digits[8].bytes = xpm_load(eight_xpm, XPM_8_8_8_8, &digits[8]);
  digits[9].bytes = xpm_load(nine_xpm, XPM_8_8_8_8, &digits[9]);


  game_images[BACKGROUND_IMG].bytes = xpm_load(background_xpm, XPM_8_8_8_8, &game_images[BACKGROUND_IMG]);

  game_images[MENU_BACKGROUND_IMG].bytes = xpm_load(menu_background_xpm, XPM_8_8_8_8, &game_images[MENU_BACKGROUND_IMG]);

  game_images[SP_END_BACKGROUND_IMG].bytes = xpm_load(sp_end_background_xpm, XPM_8_8_8_8, &game_images[SP_END_BACKGROUND_IMG]);

  game_images[HELP1_BACKGROUND_IMG].bytes = xpm_load(helpscreen_1_xpm, XPM_8_8_8_8, &game_images[HELP1_BACKGROUND_IMG]);

  game_images[HELP2_BACKGROUND_IMG].bytes = xpm_load(helpscreen_2_xpm, XPM_8_8_8_8, &game_images[HELP2_BACKGROUND_IMG]);
  
  game_images[SINGLEPLAYER_IMG].bytes = xpm_load(singleplayer_xpm, XPM_8_8_8_8, &game_images[SINGLEPLAYER_IMG]);
  
  game_images[CHOPPED_SINGLEPLAYER_IMG].bytes = xpm_load(choppedsingleplayer_xpm, XPM_8_8_8_8, &game_images[CHOPPED_SINGLEPLAYER_IMG]);
  
  game_images[MULTIPLAYER_IMG].bytes = xpm_load(multiplayer_xpm, XPM_8_8_8_8, &game_images[MULTIPLAYER_IMG]);

  game_images[CHOPPED_MULTIPLAYER_IMG].bytes = xpm_load(choppedmultiplayer_xpm, XPM_8_8_8_8, &game_images[CHOPPED_MULTIPLAYER_IMG]);

  game_images[BOMB_IMG].bytes = xpm_load(bomb_xpm, XPM_8_8_8_8, &game_images[BOMB_IMG]);

  game_images[APPLE_IMG].bytes = xpm_load(apple_xpm, XPM_8_8_8_8, &game_images[APPLE_IMG]);

  game_images[CHOPPED_APPLE_IMG].bytes = xpm_load(choppedapple_xpm, XPM_8_8_8_8, &game_images[CHOPPED_APPLE_IMG]);

  game_images[PINEAPPLE_IMG].bytes = xpm_load(pineapple_xpm, XPM_8_8_8_8, &game_images[PINEAPPLE_IMG]);

  game_images[CHOPPED_PINEAPPLE_IMG].bytes = xpm_load(choppedpineapple_xpm, XPM_8_8_8_8, &game_images[CHOPPED_PINEAPPLE_IMG]);

  game_images[WATERMELON_IMG].bytes = xpm_load(watermelon_xpm, XPM_8_8_8_8, &game_images[WATERMELON_IMG]);

  game_images[CHOPPED_WATERMELON_IMG].bytes = xpm_load(choppedwatermelon_xpm, XPM_8_8_8_8, &game_images[CHOPPED_WATERMELON_IMG]);

  game_images[KATANA_UP_IMG].bytes = xpm_load(katana_up_xpm, XPM_8_8_8_8, &game_images[KATANA_UP_IMG]);

  game_images[KATANA_DOWN_IMG].bytes = xpm_load(katana_down_xpm, XPM_8_8_8_8, &game_images[KATANA_DOWN_IMG]);
  
  game_images[COMBO_IMG].bytes = xpm_load(combo_xpm, XPM_8_8_8_8, &game_images[COMBO_IMG]);
  
  game_images[CANON_IMG].bytes = xpm_load(canon_xpm, XPM_8_8_8_8, &game_images[CANON_IMG]);
}
