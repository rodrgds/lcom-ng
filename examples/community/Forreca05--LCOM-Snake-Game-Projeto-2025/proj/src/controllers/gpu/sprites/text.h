#ifndef TEXT_H
#define TEXT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "sprite.h"
#include "../../../assets/xpm/all.xpm"

typedef struct Text Text;

struct Text{
    int initial_x, initial_y, spacing, size;
    char* text;
        
    Sprite** characters;

    void (*draw)(Text* self);
    void (*destroy)(Text* self);
    void (*updateText)(Text* self, const char* newText);
};

Text *create_Text(const char *text, int initial_x, int initial_y, int spacing);

#endif
