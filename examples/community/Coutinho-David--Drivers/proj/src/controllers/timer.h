#ifndef TIMER_H
#define TIMER_H

#include <lcom/lcf.h>
#include <stdint.h>
//#include "i8254.h"

void (timer_ih)();
void (timer_reset)();
uint32_t (timer_get_elapsed)();

#endif 
