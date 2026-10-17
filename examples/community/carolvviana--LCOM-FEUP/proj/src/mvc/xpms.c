#include "xpms.h"

void load_xpms(){
    xpm_load(MAIN_MENU_SPRITE, XPM_8_8_8_8, &images[MAIN_MENU]);
    xpm_load(CREDITS_SPRITE, XPM_8_8_8_8, &images[CREDITS_INSTRUCTIONS]);

    xpm_load(MENU_SINGLEPLAYER_OPTION_SPRITE, XPM_8_8_8_8, &images[MENU_SINGLEPLAYER_OPTION]);
    xpm_load(MENU_MULTIPLAYER_OPTION_SPRITE, XPM_8_8_8_8, &images[MENU_MULTIPLAYER_OPTION]);
    xpm_load(MENU_CREDITS_OPTION_SPRITE, XPM_8_8_8_8, &images[MENU_CREDITS_OPTION]);

    xpm_load(CURSOR_SPRITE, XPM_8_8_8_8, &images[CURSOR]);

    xpm_load(ARENA_SPRITE, XPM_8_8_8_8, &images[ARENA]);

    xpm_load(DASH_SPRITE,XPM_8_8_8_8, &images[DASH]);
    xpm_load(ZERO_SPRITE,XPM_8_8_8_8, &images[ZERO]);
    xpm_load(ONE_SPRITE,XPM_8_8_8_8, &images[ONE]);
    xpm_load(TWO_SPRITE,XPM_8_8_8_8, &images[TWO]);
    xpm_load(THREE_SPRITE,XPM_8_8_8_8, &images[THREE]);
    xpm_load(FOUR_SPRITE,XPM_8_8_8_8, &images[FOUR]);
    xpm_load(FIVE_SPRITE,XPM_8_8_8_8, &images[FIVE]);
    xpm_load(SIX_SPRITE,XPM_8_8_8_8, &images[SIX]);	
    xpm_load(SEVEN_SPRITE,XPM_8_8_8_8, &images[SEVEN]);
    xpm_load(EIGHT_SPRITE,XPM_8_8_8_8, &images[EIGHT]);
    xpm_load(NINE_SPRITE,XPM_8_8_8_8, &images[NINE]);

    xpm_load(GAME_OVER_SPRITE, XPM_8_8_8_8, &images[GAME_OVER_SCREEN]);
    xpm_load(GAME_WIN_SPRITE, XPM_8_8_8_8, &images[GAME_WIN_SCREEN]);
    xpm_load(WAITING_SPRITE, XPM_8_8_8_8, &images[WAITING_SCREEN]);
    xpm_load(BALL_SPRITE, XPM_8_8_8_8, &images[BALL]);
    
    xpm_load(SMALLER_ZERO_SPRITE,XPM_8_8_8_8, &images[SMALLER_ZERO]);
    xpm_load(SMALLER_ONE_SPRITE,XPM_8_8_8_8, &images[SMALLER_ONE]);
    xpm_load(SMALLER_TWO_SPRITE,XPM_8_8_8_8, &images[SMALLER_TWO]);
    xpm_load(SMALLER_THREE_SPRITE,XPM_8_8_8_8, &images[SMALLER_THREE]);
    xpm_load(SMALLER_FOUR_SPRITE,XPM_8_8_8_8, &images[SMALLER_FOUR]);
    xpm_load(SMALLER_FIVE_SPRITE,XPM_8_8_8_8, &images[SMALLER_FIVE]);
    xpm_load(SMALLER_SIX_SPRITE,XPM_8_8_8_8, &images[SMALLER_SIX]);
    xpm_load(SMALLER_SEVEN_SPRITE,XPM_8_8_8_8, &images[SMALLER_SEVEN]);
    xpm_load(SMALLER_EIGHT_SPRITE,XPM_8_8_8_8, &images[SMALLER_EIGHT]);
    xpm_load(SMALLER_NINE_SPRITE,XPM_8_8_8_8, &images[SMALLER_NINE]);
    xpm_load(COLON_SPRITE,XPM_8_8_8_8, &images[COLON]);
    xpm_load(BLOCKED_SPRITE,XPM_8_8_8_8, &images[BLOCKED_SCREEN]);
    xpm_load(COOLDOWN0v3_SPRITE, XPM_8_8_8_8, &images[COOLDOWN_0v3]);
    xpm_load(COOLDOWN15_SPRITE, XPM_8_8_8_8, &images[COOLDOWN_15]); //1º o nome dentro do ficheiro xpm e depois o nome que terá nas images[] 
    xpm_load(COOLDOWN30_SPRITE, XPM_8_8_8_8, &images[COOLDOWN_30]);
    xpm_load(COOLDOWN45_SPRITE, XPM_8_8_8_8, &images[COOLDOWN_45]);
    xpm_load(COOLDOWN60_SPRITE, XPM_8_8_8_8, &images[COOLDOWN_60]);
    xpm_load(ACTIVATED_SPRITE, XPM_8_8_8_8, &images[ACTIVATED]);
}
