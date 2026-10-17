#include <lcom/lcf.h>

#include "../../model/stateModels/ticTacToe.h"
#include "../../model/button.h"
#include "../../model/cursor.h"

#include "../../player_data/database.h"

#include "../../viewer/minigames/ticTacToeViewer.h"
#include "../../viewer/guiDrawer.h"

#include "../../device_controllers/uart/uart.h"

typedef enum {QUIT_TTT, HOST_TTT, GUEST_TTT, NOP_TTT} collision_type_ttt_t;

void ttt_set_canClick();
void ticTacToeController_load_tictactoe();
bool ticTacToeController_checkCollisionWithSprites(Sprite* sprite, int x, int y);
bool ticTacToe_checkCollision(int x, int y, int width, int height);
collision_type_ttt_t ticTacToeController_getButtonEvent();
void ticTacToeController_setButtonEvent(collision_type_ttt_t ct);
void checkIfPlayerWon(uint8_t curI, uint8_t curPath[9], bool plays[9]);
void ticTacToeController_step();
void ticTacToeController_delete_tictactoe();
Cursor* getTicTacToeCursor();
TicTacToe* ticTacToeController_get_saved_ttt();
void setTicTacToeCursor(Cursor* cursor);
void ticTacToeController_randomize_reward();
