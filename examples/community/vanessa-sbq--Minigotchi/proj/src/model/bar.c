#include "bar.h"

/**
 * @brief Creates a new bar "object" and returns a pointer to it.
 */
Bar *new_bar(int x, int y, int level, int bar_type) { 
    Bar *bar = malloc(sizeof(*bar));
    if (bar == NULL) return NULL;

    // Initialize Bar
    bar->sprite = guiDrawer_get_bar_sprite(bar_type, level);
    bar->level = level; 
    bar->x = x; 
    bar->y = y;

    return bar;
}

/**
 * @brief Bar destructor
 */
void bar_delete(Bar *bar) {
    free(bar);
}

/**
 * @brief Draws a bar
 */
void draw_bar(Bar *bar) {
    drawSprite(bar->sprite, bar->x, bar->y);
}

// Getters
Sprite* bar_get_sprite(Bar *bar) {
    return bar->sprite;
}

int bar_get_level(Bar *bar) {
    return bar->level;
}

int bar_get_x(Bar *bar) {
    return bar->x;
}

int bar_get_y(Bar *bar) {
    return bar->y;
}


// Setters
void bar_set_sprite(Bar *bar, Sprite *sprite) {
    bar->sprite = sprite;
}

void bar_set_level(Bar *bar, int level) {
    bar->level = level;
}

void bar_set_x(Bar *bar, int x) {
    bar->x = x;
}

void bar_set_y(Bar *bar, int y) {
    bar->y = y;
}
