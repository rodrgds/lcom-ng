#ifndef DECK_H
#define DECK_H

#include "controllers/timer.h"

extern int player_cards[5];
extern int player_c_count;
extern int dealer_cards[5];
extern int dealer_c_count;
extern bool dealer_hidden;

void getCard(int* score, int who);

#endif
