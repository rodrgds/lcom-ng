#include "item_controller.h"

bool (is_selected_item)() {
    if (select_item(little_block) && !is_in_map(little_block))
        item = little_block;
    if (select_item(big_block) && !is_in_map(big_block))
        item = big_block;
    if (select_item(little_plank) && !is_in_map(little_plank))
        item = little_plank;
    if (select_item(big_plank) && !is_in_map(big_plank))
        item = big_plank;

    if (item != NULL) {
        off_x = cursor->x - item->x;
        off_y = cursor->y - item->y;
        item_x = item->x;
        item_y = item->y;
        return true;
    }
    return false;
}

void (update_item_pos)() {
    item->x = cursor->x - off_x;
    item->y = cursor->y - off_y;
}

void (check_item_pos)() {

    if (!is_in_map(item) || collide(item, panda) || 
        collide(item, little_block) || collide(item, big_block) || 
        collide(item, little_plank) || collide(item, big_plank) || 
        collide(item, map) || collide(item, lava) || 
        collide(item, home) || collide(item, bamboo)) {
        item->x = item_x;
        item->y = item_y;
        item->i = 0;
    }

    item = NULL;
}

void (update_item_animation)() {
    if (scancode == R_KEY) item->i = (item->i + 1) % item->num_pixmaps;
    else item->i = (item->i - 1 + item->num_pixmaps) % item->num_pixmaps;
}

void (update_inventory)() {
    setPos(bamboo, 190, 515);
}

bool (select_item)(Sprite *item) {
    return cursor->x >= item->x && cursor->x <= item->x + item->width[item->i] &&
           cursor->y >= item->y && cursor->y <= item->y + item->height[item->i];
}
