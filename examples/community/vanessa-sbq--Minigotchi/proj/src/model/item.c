#include "item.h"

/**
 * @brief Creates a new item "object" and returns a pointer to it.
 */
Item *new_item(int x, int y, item_t id, int quantity, int feed_level){
    Item *item = malloc(sizeof(*item));
    if (item == NULL) return NULL;

    item->sprite = guiDrawer_get_item_sprite(id); // Pass the id to get the corresponding sprite
    item->x = x;
    item->y = y;
    item->id = id;
    item->quantity = quantity;
    item->feed_level = feed_level;

    return item;
}

/**
 * @brief Draws an item
 */
void draw_item(Item* item){
    drawSprite(item->sprite, item->x, item->y);
}


// Getters
int item_get_x(Item *item){
    return item->x;
}

int item_get_y(Item *item){
    return item->y;
}

item_t item_get_id(Item *item){
    return item->id;
}

int item_get_quantity(Item *item){
    return item->quantity;
}

int item_get_feedLevel(Item *item){
    return item->feed_level;
}

Sprite* item_get_sprite(Item *item){
    return item->sprite;
}


// Setters
void item_set_x(Item *item, int x){
    item->x = x;
}

void item_set_y(Item *item, int y){
    item->y = y;
}

void item_set_type(Item *item, item_t id){
    item->id = id;
}

void item_set_quantity(Item *item, int quantity){
    item->quantity = quantity;
}
