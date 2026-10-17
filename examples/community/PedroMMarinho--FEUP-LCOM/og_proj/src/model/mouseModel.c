#include "mouseModel.h"




Mouse* newMouse(){
  Mouse* mouse = (Mouse*)malloc(sizeof(Mouse));
  vector_t initialPos =  {300, 300};
  mouse->pos  = initialPos;
  return mouse;
}

void destroyMouse(Mouse* mouse){
  free(mouse);
}

