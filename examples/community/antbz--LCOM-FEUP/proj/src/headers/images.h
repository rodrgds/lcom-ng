#pragma once

#include <lcom/lcf.h>

#include "assets/digits/zero.xpm"
#include "assets/digits/one.xpm"
#include "assets/digits/two.xpm"
#include "assets/digits/three.xpm"
#include "assets/digits/four.xpm"
#include "assets/digits/five.xpm"
#include "assets/digits/six.xpm"
#include "assets/digits/seven.xpm"
#include "assets/digits/eight.xpm"
#include "assets/digits/nine.xpm"

#include "../assets/backgrounds/background.xpm"
#include "../assets/backgrounds/menu_background.xpm"
#include "../assets/backgrounds/sp_end_background.xpm"
#include "../assets/backgrounds/helpscreen_1.xpm"
#include "../assets/backgrounds/helpscreen_2.xpm"

#include "../assets/elements/singleplayer.xpm"
#include "../assets/elements/choppedsingleplayer.xpm"
#include "../assets/elements/multiplayer.xpm"
#include "../assets/elements/choppedmultiplayer.xpm"
#include "../assets/elements/combo.xpm"
#include "../assets/elements/katana_up.xpm"
#include "../assets/elements/katana_down.xpm"
#include "../assets/elements/canon.xpm"

#include "../assets/fruits/apple.xpm"
#include "../assets/fruits/choppedapple.xpm"
#include "../assets/fruits/pineapple.xpm"
#include "../assets/fruits/choppedpineapple.xpm"
#include "../assets/fruits/watermelon.xpm"
#include "../assets/fruits/choppedwatermelon.xpm"
#include "../assets/fruits/bomb.xpm"


#define NUM_IMAGES 20


xpm_image_t game_images[NUM_IMAGES]; /**< Game images array. */
xpm_image_t digits[10]; /**< Digit images array. */


/** 
 * @brief Names of game images
 */
enum game_img_t {
  BACKGROUND_IMG,
  MENU_BACKGROUND_IMG,
  HELP1_BACKGROUND_IMG,
  HELP2_BACKGROUND_IMG,
  SP_END_BACKGROUND_IMG,
  SINGLEPLAYER_IMG,
  CHOPPED_SINGLEPLAYER_IMG,
  MULTIPLAYER_IMG,
  CHOPPED_MULTIPLAYER_IMG,
  BOMB_IMG,
  APPLE_IMG,
  CHOPPED_APPLE_IMG,
  PINEAPPLE_IMG,
  CHOPPED_PINEAPPLE_IMG,
  WATERMELON_IMG,
  CHOPPED_WATERMELON_IMG,
  KATANA_UP_IMG,
  KATANA_DOWN_IMG,
  COMBO_IMG,
  CANON_IMG
};


/**
 * @brief Loads all included xpm images to the game_images and digits array.
 */
void load_all_images();
