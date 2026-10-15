
#include "controller.h"

/** @defgroup controller controller
 * @{
 */

/**
 * @brief Based on the pressed key, the player moves or jumps in the play game state.
 * @param player Reference to the Player object.
 * @param Kscancode kbc scancode that represents a key in the keyboard.
 * @param gs Current game state,
 */
void (manageInputKeyboard)(Object *player, uint8_t Kscancode, GameState *gs){
    if(*gs == PLAY){
      switch (Kscancode){
    case MAKE_D_KEY:
      if (!IS_BREAK_CODE(Kscancode)) player->xspeed = 10;
      break;
    case MAKE_A_KEY:
      if (!IS_BREAK_CODE(Kscancode)) player->xspeed = -10;
      break;
    case BREAK_D_KEY:
      if (IS_BREAK_CODE(Kscancode) && player->xspeed > 0) player->xspeed = 0;
      break;
    case BREAK_A_KEY:
      if (IS_BREAK_CODE(Kscancode) && player->xspeed < 0) player->xspeed = 0;
      break;  
    case SPACE_BAR_KEY:
      if (player->yspeed == 0) player->yspeed -= 15; // Força de impulso
      break;    
    default:
      break;
    }
    }
    
}

/**@}*/

