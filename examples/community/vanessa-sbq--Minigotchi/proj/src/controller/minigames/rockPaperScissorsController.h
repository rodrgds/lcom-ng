#include <lcom/lcf.h>

#include "../../model/stateModels/rockPaperScissors.h"
#include "../../model/button.h"
#include "../../model/cursor.h"

#include "../../viewer/minigames/rockPaperScissorsViewer.h"
#include "../../viewer/guiDrawer.h"
#include "../../player_data/database.h"

void rps_set_canClick();

void rockPaperScissorsController_load_rockPaperScissors();

collision_type_rps_t rockPaperScissorsController_getButtonEvent();

void rockPaperScissorsController_setButtonEvent(collision_type_rps_t ct);

bool rockPaperScissorsController_checkCollision(Sprite* sprite, int x, int y);

void rockPaperScissorsController_step();

void rockPaperScissorsController_resetGame();

void rockPaperScissorsController_randomize_reward();

void rockPaperScissorsController_delete_rockPaperScissors();

void rockPaperScissorsController_process_choice();

Cursor* getRockPaperScissorsCursor();

void setRockPaperScissorsCursor(Cursor* cursor);
