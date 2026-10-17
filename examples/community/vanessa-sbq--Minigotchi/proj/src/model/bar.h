#ifndef _LCOM_BAR_H_
#define _LCOM_BAR_H_

#include <lcom/lcf.h>

#include "../viewer/guiDrawer.h"

typedef struct{
    Sprite *sprite;
    int level;
    int x;
    int y;
} Bar;

// "Constructor"
Bar *new_bar(int x, int y, int level, int bar_type);

// "Destructor"
void bar_delete(Bar *bar);

void draw_bar(Bar *bar);

// Getters
Sprite* bar_get_sprite(Bar *bar);
int bar_get_level(Bar *bar);
int bar_get_x(Bar *bar);
int bar_get_y(Bar *bar);

// Setters
void bar_set_sprite(Bar *bar, Sprite *sprite);
void bar_set_level(Bar *bar, int level);
void bar_set_x(Bar *bar, int x);
void bar_set_y(Bar *bar, int y);

#endif /* _BAR_H */
