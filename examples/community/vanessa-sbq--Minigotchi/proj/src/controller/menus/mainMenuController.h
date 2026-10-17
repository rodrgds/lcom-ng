#include <lcom/lcf.h>

#include "../../model/stateModels/mainMenu.h"
#include "../../model/button.h"
#include "../../model/cursor.h"

#include "../../viewer/menus/mainMenuViewer.h"

typedef enum {START, QUIT, NOP} collision_type_t;

void mainMenuController_load_mainMenu();

collision_type_t mainMenuController_checkCollision(Button* mainMenu);

collision_type_t mainMenuController_getButtonEvent();

void mainMenuController_step();

Cursor* getMainMenuCursor();

void setMainMenuCursor(Cursor* cursor);

void mainMenuController_delete_mainMenu();
