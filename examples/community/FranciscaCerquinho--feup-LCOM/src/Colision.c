#include "Colision.h"


int ColisionPipe(mario *newmario, Pipe *pipe)
{

	if(newmario->y+ 68   > 599 && newmario->x < pipe->x && pipe->x < newmario->x + 56 && newmario->y < 599+ pipe->image->bitmapInfoHeader.height){

		return 1;
	}
    return 0;
}
