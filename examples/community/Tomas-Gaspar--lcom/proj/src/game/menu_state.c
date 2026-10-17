#include <lcom/lcf.h>

#include "../getters.h"

#include "menu_state.h"

#include "../img/buttons/START.xpm"
#include "../img/buttons/START_SELECTED.xpm"
#include "../img/buttons/GUESS.xpm"
#include "../img/buttons/GUESS_SELECTED.xpm"
#include "../img/buttons/EXIT.xpm"
#include "../img/buttons/EXIT_SELECTED.xpm"
#include "../img/buttons/SINGLEPLAYER.xpm"
#include "../img/buttons/SINGLEPLAYER_SELECTED.xpm"
#include "../img/buttons/MULTIPLAYER.xpm"
#include "../img/buttons/MULTIPLAYER_SELECTED.xpm"
#include "../img/cursor.xpm"


bool process_singleplayer_button(Cursor* cursor) {
    static bool isSinglePlayerSelected = false;
    if(cursor->x >= 300 && cursor->x < 500 && cursor->y >= 140 && cursor->y < 240) {
        cursor->xpmCode = 1;
        if (get_color_theme() == LIGHTMODE) cursor->cursorMap = hand_day;
        else cursor->cursorMap = hand_night;
        if(!isSinglePlayerSelected) {
            paint_image(singlePlayer_selected, 300, 140);
            isSinglePlayerSelected = true;
        }
        if(get_packet().lb) {
            isSinglePlayerSelected = false;
            return true;
        }
    } else {
        cursor->xpmCode = 0;
        if (get_color_theme() == LIGHTMODE) cursor->cursorMap = cursor_day;
        else cursor->cursorMap = cursor_night;
        if(isSinglePlayerSelected) {
            paint_image(singlePlayer_button, 300, 140);
            isSinglePlayerSelected = false;
        }
    }

    return false;
}

bool process_multiplayer_button(Cursor* cursor) {
    static bool isMultiPlayerSelected = false;
    if(cursor->x >= 300 && cursor->x < 500 && cursor->y >= 250 && cursor->y < 350) {
        cursor->xpmCode = 1;
        if (get_color_theme() == LIGHTMODE) cursor->cursorMap = hand_day;
        else cursor->cursorMap = hand_night;
        if(!isMultiPlayerSelected) {
            paint_image(multiPlayer_selected, 300, 250);
            isMultiPlayerSelected = true;
        }
        if(get_packet().lb) {
            isMultiPlayerSelected = false;
            return true;
        }
    } else {
        if(isMultiPlayerSelected) {
            paint_image(multiPlayer_button, 300, 250);
            isMultiPlayerSelected = false;
        }
    }

    return false;
}



bool process_start_button(Cursor* cursor) {
    static bool isStartSelected = false;
    if(cursor->x >= 300 && cursor->x < 500 && cursor->y >= 140 && cursor->y < 240) {
        cursor->xpmCode = 1;
        if (get_color_theme() == LIGHTMODE) cursor->cursorMap = hand_day;
        else cursor->cursorMap = hand_night;
        if(!isStartSelected) {
            paint_image(start_selected, 300, 140);
            isStartSelected = true;
        }
        if(get_packet().lb) {
            isStartSelected = false;
            return true;
        }
    } else {
        cursor->xpmCode = 0;
        if (get_color_theme() == LIGHTMODE) cursor->cursorMap = cursor_day;
        else cursor->cursorMap = cursor_night;
        if(isStartSelected) {
            paint_image(start_button, 300, 140);
            isStartSelected = false;
        }
    }

    return false;
}

bool process_guess_button(Cursor* cursor) {
    static bool isGuessSelected = false;
    if(cursor->x >= 300 && cursor->x < 500 && cursor->y >= 250 && cursor->y < 350) {
        cursor->xpmCode = 1;
        if (get_color_theme() == LIGHTMODE) cursor->cursorMap = hand_day;
        else cursor->cursorMap = hand_night;
        if(!isGuessSelected) {
            paint_image(guess_selected, 300, 250);
            isGuessSelected = true;
        }
        if(get_packet().lb) {
            isGuessSelected = false;
            return true;
        }
    } else {
        if(isGuessSelected) {
            paint_image(guess_button, 300, 250);
            isGuessSelected = false;
        }
    }

    return false;
}

bool process_exit_button(Cursor* cursor) {
    static bool isExitSelected = false;
    if(cursor->x >= 300 && cursor->x < 500 && cursor->y >= 360 && cursor->y < 460) {
        cursor->xpmCode = 1;
        if (get_color_theme() == LIGHTMODE) cursor->cursorMap = hand_day;
        else cursor->cursorMap = hand_night;
        if(!isExitSelected) {
            paint_image(exit_selected, 300, 360);
            isExitSelected = true;
        }
        if(get_packet().lb) {
            isExitSelected = false;
            return true;
        }
    } else {
        if(isExitSelected) {
            paint_image(exit_button, 300, 360);
            isExitSelected = false;
        }
    }

    return false;
}
