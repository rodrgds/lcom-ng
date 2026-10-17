#include <lcom/lcf.h>

#include "../../model/stateModels/nameMinigotchi.h"
#include "../../model/button.h"
#include "../../model/cursor.h"

#include "../../viewer/menus/nameMinigotchiViewer.h"

#include "../../device_controllers/kbc/keyboardMappings.h"

typedef enum {CONTINUE_NAMEMINIGOTCHI, QUIT_NAMEMINIGOTCHI, NOP_NAMEMINIGOTCHI} collision_type_nameMinigotchi_t;

void nameMinigotchiController_load_nameMinigotchi();

bool nameMinigotchiController_checkCollision(Sprite* sprite, int x, int y);

collision_type_nameMinigotchi_t nameMinigotchiController_getButtonEvent();

void nameMinigotchiController_setButtonEvent(collision_type_nameMinigotchi_t ct);

void nameMinigotchi_setKeyReleased();

void nameMinigotchiController_step(uint8_t keyPress);

Cursor* getNameMinigotchiCursor();

void setNameMinigotchiCursor(Cursor* cursor);

void nameMinigotchiController_delete_nameMinigotchi();
