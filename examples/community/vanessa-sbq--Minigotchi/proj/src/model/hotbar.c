#include "hotbar.h"

#define HOTBAR_START_SLOT_X 228
#define HOTBAR_SLOT_WIDTH 80
#define HOTBAR_START_SLOT_Y 765

/**
 * @brief Creates a new hotbar "object" and returns a pointer to it.
 */
Hotbar *new_hotbar(uint16_t x, uint16_t y, uint8_t highlighted) {

    Hotbar *hotbar = malloc(sizeof(*hotbar));
    if (hotbar == NULL) return NULL;

    hotbar->sprite = guiDrawer_get_hotbar_sprite();
    hotbar->x = x;
    hotbar->y = y;
    hotbar->highlighted = highlighted;
    hotbar->hidden = true; 

    Item* item_array[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
    for (int i = 0; i < 9; i++){
        item_array[i] = new_item(HOTBAR_START_SLOT_X + HOTBAR_SLOT_WIDTH * i, HOTBAR_START_SLOT_Y, 0, 1, 20); // Create empty items
    }
    memcpy(hotbar->item_array, item_array, sizeof(item_array));

    return hotbar;
}


/**
 * @brief Draws a hotbar
 */
void drawHotbar(Hotbar *hotbar) {
    for (int i = 0; i < 9; i++){
        draw_item(hotbar->item_array[i]);
    }
}

/**
 * @brief Draws a hotbar slider
 */
void draw_slider(Hotbar *hotbar) {
    drawSprite(guiDrawer_get_hotbar_select_sprite(), (hotbar->x + HIGHLIGHT_HOTBAR_CELL_SIZE * hotbar->highlighted), hotbar->y);
}

/**
 * @brief Updates the items in a hotbar
 */
void hotbar_update_items(Hotbar *hotbar, int* item_array){
    for (int i = 0; i < 9; i++){
        if (item_get_id(hotbar->item_array[i]) != item_array[i]){
            Item* item = new_item(HOTBAR_START_SLOT_X + HOTBAR_SLOT_WIDTH * i, HOTBAR_START_SLOT_Y, item_array[i], 1, 300);
            hotbar->item_array[i] = item;
        } 
    }
}


// Getters and setters

uint8_t hotbar_get_highlighted(Hotbar *hotbar) {
    return hotbar->highlighted;
}

Item* hotbar_get_highlighted_item(Hotbar *hotbar) {
    return hotbar->item_array[hotbar->highlighted];
}

bool hotbar_is_hidden(Hotbar* hotbar){
    return hotbar->hidden;
}



void hotbar_set_highlighted(Hotbar *hotbar, uint8_t highlighted) {
    hotbar->highlighted = highlighted;
}

void hotbar_set_items(Hotbar *hotbar, Item* item_array){
    memcpy(hotbar->item_array, item_array, sizeof(hotbar->item_array));
}
