#include <lcom/lcf.h>

#include "../../model/stateModels/minigameMenu.h"
#include "../../model/button.h"
#include "../../model/cursor.h"

#include "../../viewer/menus/minigameMenuViewer.h"
#include "../../viewer/guiDrawer.h"

typedef enum {QUIT_MINIGAMEMENU, MINIGAME1_MINIGAMEMENU, MINIGAME2_MINIGAMEMENU, NOP_MINIGAMEMENU} collision_type_minigameMenu_t;


void minigameMenuController_load_minigameMenu();

bool minigameMenuController_checkCollision(Sprite* sprite, int x, int y);

collision_type_minigameMenu_t minigameMenuController_getButtonEvent();

void minigameMenuController_setButtonEvent(collision_type_minigameMenu_t ct);

void minigameMenuController_step();

void minigameMenuController_delete_minigameMenu();

Cursor* getMinigameMenuCursor();

void setMinigameMenuCursor(Cursor* cursor);
