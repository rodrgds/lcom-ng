#include "stateviewer.h"
#include "gameviewer.h"
#include <lcom/lcf.h>
#include "../../drivers/graphics.h"

void showsState(struct Game *game)
{
  
  switch (game->state)
  {
  case STATE_GAME:
    showGame(game);
    break;

  case STATE_MENU:
    clean_screen();
    showMenu(game);
    vg_f7_withsubfunction80();
    break;

  case STATE_WIN: 
    {
      showWin(game);
    }
    break;

  case STATE_LOSE:
    {
      showLose(game);
    }
    break;

  default:
    break;
  }
}
