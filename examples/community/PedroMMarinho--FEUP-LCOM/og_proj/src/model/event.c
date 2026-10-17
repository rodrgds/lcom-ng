
#include "event.h"
#include <math.h>



Event newInvalidEvent(){
  Event event = {INFINITY, INVALID, NULL, NULL, NULL, NULL, -1};
  return event;
}

