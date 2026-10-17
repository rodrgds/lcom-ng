#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <lcom/lcf.h>
#include <stdint.h>

void (get_scoreboard)(uint8_t kb_irq_set, uint8_t mouse_irq_set, uint8_t timer_irq_set, int final_score);

#endif
