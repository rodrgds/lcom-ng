#include "button.h"

/**
 * @brief Creates a new button "object" and returns a pointer to it.
 */
Button *new_button(int x, int y, int text_index) { 
     Button *button = malloc(sizeof(*button));
    if (button == NULL) return NULL;

    // Initialize Cursor
    button->sprite = guiDrawer_get_button_sprite(text_index);
    button->x = x; 
    button->y = y;
    button->activated = true;

    return button;
}

/**
 * @brief Button destructor
 */
void button_delete(Button *button) {
    free(button);
}

/**
 * @brief Draws a button
 */
void draw_button(Button *button) {
    if (button->activated){
        drawSprite(button->sprite, button->x, button->y);
    }
}

// Getters and setters
Sprite* button_get_sprite(Button *button) {
    return button->sprite;
}

int button_get_x(Button *button) {
    return button->x;
}

int button_get_y(Button *button) {
    return button->y;
}

bool button_get_activated(Button *button){
    return button->activated;
}

void button_set_activated(Button *button, bool activated){
    button->activated = activated;
}
