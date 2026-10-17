#ifndef _ITEM_H_
#define _ITEM_H_

#include "../viewer/guiDrawer.h"

#define NUM_ITEMS 4
typedef enum {EMPTY_ITEM, BANANA, APPLE, CHOCOLATE} item_t;

typedef struct{
    Sprite *sprite;
    int x;
    int y;
    item_t id;
    int quantity;
    int feed_level;
} Item;

Item *new_item(int x, int y, item_t type, int quantity, int feed_level);

void draw_item(Item* item);

// Getters
int item_get_x(Item *item);
int item_get_y(Item *item);
item_t item_get_id(Item *item);
int item_get_quantity(Item *item);
int item_get_feedLevel(Item *item);
Sprite* item_get_sprite(Item *item);

// Setters
void item_set_x(Item *item, int x);
void item_set_y(Item *item, int y);
void item_set_id(Item *item, item_t id);
void item_set_quantity(Item *item, int quantity);

#endif /* _ITEM_H_ */
