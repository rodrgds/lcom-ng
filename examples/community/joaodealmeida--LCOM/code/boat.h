#ifndef __BOAT_H
#define __BOAT_H

//#include "vector2.h"//not really need because of colision but works as a reminder
#include "collision.h"
#include "sprite.h"


/** @defgroup boat boat
 * @{*/

struct playerUnit{
vector2 position;
Sprite* sp;
ColisionRadius colRad;
};

typedef struct playerUnit boat;

boat createBoat();

/**@brief  updates boat
@param boat boat to be updated
@param movHor indicates if boat moves horizontally, -1 moves left, 1 moves right, 0 does not move horizontally
@param movVert indicates if boat moves vertically, -1 moves up, 1 moves down, 0 does not move vertically
*/
void updateBoat(boat *b, char movHor, char movVert);

void drawBoat(boat *b);
//void deleteBoat(boat *b);///<frees memory allocated for boad


void freeBoat(boat *b);

/** @} end of boat */
#endif// __BOAT_H
