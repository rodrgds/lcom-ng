#ifndef _LCOM_SPRITES_H_
#define _LCOM_SPRITES_H_

#include "xpm/mouse.xpm"
#include "xpm/fireboywatergirl.xpm"
#include "xpm/_1computer.xpm"
#include "xpm/_2computers.xpm"
#include "xpm/exit.xpm"
#include "xpm/levels.xpm"
#include "xpm/background.xpm"
#include "xpm/LevelBackground.xpm"
#include "xpm/Bottom.xpm"
#include "xpm/BottomLeft.xpm"
#include "xpm/BottomRight.xpm"
#include "xpm/Left.xpm"
#include "xpm/LeftRightBottom.xpm"
#include "xpm/LeftTopRight.xpm"
#include "xpm/NoBorder.xpm"
#include "xpm/Right.xpm"
#include "xpm/RightLeft.xpm"
#include "xpm/Top.xpm"
#include "xpm/TopBottom.xpm"
#include "xpm/TopLeft.xpm"
#include "xpm/TopLeftBottom.xpm"
#include "xpm/TopRight.xpm"
#include "xpm/TopRightBottom.xpm"
#include "xpm/fireboy.xpm"
#include "xpm/FireboyLeft1.xpm"
#include "xpm/FireboyLeft2.xpm"
#include "xpm/FireboyRight1.xpm"
#include "xpm/FireboyRight2.xpm"
#include "xpm/watergirlLeft1.xpm"
#include "xpm/watergirlLeft2.xpm"
#include "xpm/watergirlRight1.xpm"
#include "xpm/watergirlRight2.xpm"
#include "xpm/watergirl.xpm"
#include "xpm/leftblockfire.xpm"
#include "xpm/rightblockfire.xpm"
#include "xpm/centerblockfire.xpm"
#include "xpm/leftblockwater.xpm"
#include "xpm/rightblockwater.xpm"
#include "xpm/centerblockwater.xpm"
#include "xpm/leftblocktoxic.xpm"
#include "xpm/rightblocktoxic.xpm"
#include "xpm/centerblocktoxic.xpm"
#include "xpm/buttonright.xpm"
#include "xpm/buttonleft.xpm"
#include "xpm/buttonrightpressed.xpm"
#include "xpm/buttonleftpressed.xpm"
#include "xpm/redleverleft.xpm"
#include "xpm/greenleverright.xpm"
#include "xpm/doorred.xpm"
#include "xpm/doorblue.xpm"
#include "xpm/opendoor.xpm"
#include "xpm/AllBorders.xpm"
#include "xpm/PlaceTime.xpm"
#include "xpm/pontosNumber.xpm"
#include "xpm/barNumber.xpm"
#include "xpm/youLost.xpm"
#include "xpm/youWon.xpm"
#include "xpm/retry.xpm"
#include "xpm/retryLevel.xpm"
#include "xpm/next_level.xpm"
#include "xpm/waiting.xpm"
#include "xpm/n1.xpm"
#include "xpm/n2.xpm"
#include "xpm/n3.xpm"
#include "xpm/n4.xpm"
#include "xpm/n5.xpm"
#include "xpm/n6.xpm"
#include "xpm/n7.xpm"
#include "xpm/n8.xpm"
#include "xpm/n9.xpm"
#include "xpm/n0.xpm"
#include "sprites/sprite.h"

/// @brief draw a new Sprite
/// @param sprite the sprint thet will be draw
/// @param x x coordinate the Sprite will be draw in
/// @param y y coordinate the Sprite will be draw in
/// @return 0 in case of success and 1 otherwise
int drawSprite(Sprite *sprite, int x, int y);

/// @brief creates all the Sprites existent in the game
void load_sprites();

/// @brief destroys all the sprits in the end of the game
void destroy_sprites();

#endif

