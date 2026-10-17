#include "pocket.h"



Pocket* newPocket(vector_t pos, int radius){
  Pocket* pocket = (Pocket*)malloc(sizeof(Pocket));
  pocket->position = pos;
  pocket->radius = radius;
  return pocket;
}
