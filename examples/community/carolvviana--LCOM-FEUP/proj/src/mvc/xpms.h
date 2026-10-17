#pragma once

#include <lcom/lcf.h>

#include "../sprites/menu/main_menu.h"
#include "../sprites/menu/menu_credits_option.h"
#include "../sprites/menu/menu_multiplayer_option.h"
#include "../sprites/menu/menu_singleplayer_option.h"
#include "../sprites/menu/credits_instructions.h"
#include "../sprites/cursor.h"
#include "../sprites/arena/arena.h"
#include "../sprites/numbers/-.h"
#include "../sprites/numbers/0.h"
#include "../sprites/numbers/1.h"
#include "../sprites/numbers/2.h"
#include "../sprites/numbers/3.h"
#include "../sprites/numbers/4.h"
#include "../sprites/numbers/5.h"
#include "../sprites/numbers/6.h"
#include "../sprites/numbers/7.h"
#include "../sprites/numbers/8.h"
#include "../sprites/numbers/9.h"
#include "../sprites/smaller_numbers/0.h"
#include "../sprites/smaller_numbers/1.h"
#include "../sprites/smaller_numbers/2.h"
#include "../sprites/smaller_numbers/3.h"
#include "../sprites/smaller_numbers/4.h"
#include "../sprites/smaller_numbers/5.h"
#include "../sprites/smaller_numbers/6.h"
#include "../sprites/smaller_numbers/7.h"
#include "../sprites/smaller_numbers/8.h"
#include "../sprites/smaller_numbers/9.h"
#include "../sprites/smaller_numbers/colon.h"

#include "../sprites/game/game_over.h"
#include "../sprites/game/game_win.h"
#include "../sprites/game/waiting.h"
#include "../sprites/game/ball.h"
#include "../sprites/menu/blocked.h"
#include "../sprites/powerup/cooldown0v3.h"
#include "../sprites/powerup/cooldown15.h"
#include "../sprites/powerup/cooldown30.h"
#include "../sprites/powerup/cooldown45.h"
#include "../sprites/powerup/cooldown60.h"
#include "../sprites/powerup/activated.h"


/**
 * @brief Enumerated type for the images
 * 
 */
enum images{
    MAIN_MENU,
    MENU_SINGLEPLAYER_OPTION,
    MENU_MULTIPLAYER_OPTION,
    MENU_CREDITS_OPTION,
    CURSOR,
    ARENA,
    DASH,
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    GAME_OVER_SCREEN,
    GAME_WIN_SCREEN,
    WAITING_SCREEN,
    CREDITS_INSTRUCTIONS,
    SMALLER_ZERO,
    SMALLER_ONE,
    SMALLER_TWO,
    SMALLER_THREE,
    SMALLER_FOUR,
    SMALLER_FIVE,
    SMALLER_SIX,
    SMALLER_SEVEN,
    SMALLER_EIGHT,
    SMALLER_NINE,
    COLON,
    BLOCKED_SCREEN,
    COOLDOWN_0v3,
    COOLDOWN_15,
    COOLDOWN_30,
    COOLDOWN_45,
    COOLDOWN_60,
    ACTIVATED,
    BALL
};

/**
 * @brief array of images
 * 
 */
xpm_image_t images[40];

/**
 * @brief Loads the images
 * 
 */
void (load_xpms)();
