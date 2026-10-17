#include <lcom/lcf.h>

#include "../../model/stateModels/mainRoom.h"
#include "../../model/hotbar.h"
#include "../../player_data/database.h"
#include "../../utilities/vector.h"

Vector* getSpriteVector();
void setSpriteVector(Vector* vec);
void setRTCWindow(Sprite* window);
void mainRoomViewer_setMainRoom(MainRoom* mr);
void mainRoomViewer_draw();
