#ifndef _LCOM_CURSOR_H_
#define _LCOM_CURSOR_H_

#include "../viewer/guiDrawer.h"

typedef struct{
    Sprite *sprite;
    int x; 
    int y;
    bool isClicked;
} Cursor;

// "Constructor"
Cursor *new_cursor(int x, int y);

// "Destructor"
void delete_cursor(Cursor *cursor);

void draw_cursor(Cursor *cursor);

void set_buttonClicked(bool isPressed, Cursor* cursor);
bool get_buttonClicked(Cursor* cursor);

// Getters and setters
Sprite* cursor_get_sprite(Cursor *cursos);
int cursor_get_x(Cursor *cursor);
int cursor_get_y(Cursor *cursor);

void cursor_set_x(Cursor *cursor, int x);

void cursor_set_y(Cursor *cursor, int y);

bool cursor_isClicked(Cursor *cursor);

#endif /* _CURSOR_H */
