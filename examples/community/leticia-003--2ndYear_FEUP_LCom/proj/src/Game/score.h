#ifndef SCORE_H
#define SCORE_H

void score_init(void);
void score_reset(void);
void score_add(int points);
void score_draw_at(int x, int y);
int  score_get(void);

#endif
