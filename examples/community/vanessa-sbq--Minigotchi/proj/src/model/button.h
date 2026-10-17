#ifndef _LCOM_BUTTON_H_
#define _LCOM_BUTTON_H_

#include <lcom/lcf.h>

#include "../viewer/guiDrawer.h"

typedef struct{
    Sprite *sprite;
    int x;
    int y;
    bool activated;
} Button;

Button *new_button(int x, int y, int text_index); // "Constructor"

void button_delete(Button *button); // "Destructor"

void draw_button(Button *button);

// Getters and setters
Sprite* button_get_sprite(Button *button);
int button_get_x(Button *button);
int button_get_y(Button *button);
bool button_get_activated(Button *button);
void button_set_activated(Button *button, bool activated);

#endif /* _BUTTON_H */
