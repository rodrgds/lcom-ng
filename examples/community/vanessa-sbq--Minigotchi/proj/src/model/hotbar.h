#ifndef _HOTBAR_H_
#define _HOTBAR_H_

#include "../viewer/guiDrawer.h"

#include "item.h"

#define HIGHLIGHT_HOTBAR_CELL_SIZE 80

typedef struct{
    Sprite *sprite;
    Item* item_array[9];
    uint8_t highlighted;
    uint16_t x;
    uint16_t y;
    bool hidden;
} Hotbar;

Hotbar *new_hotbar(uint16_t x, uint16_t y, uint8_t highlighted);

uint8_t hotbar_get_highlighted(Hotbar *hotbar);

Item* hotbar_get_highlighted_item(Hotbar *hotbar);

bool hotbar_is_hidden(Hotbar* hotbar);

void drawHotbar(Hotbar *hotbar);

void draw_slider(Hotbar *hotbar);

void hotbar_set_highlighted(Hotbar *hotbar, uint8_t highlighted);

void hotbar_update_items(Hotbar *hotbar, int* item_array);

#endif /* _HOTBAR_H_ */
