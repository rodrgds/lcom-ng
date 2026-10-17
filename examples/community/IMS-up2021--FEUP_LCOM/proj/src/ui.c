#include "ui.h"
#include "game_state.h"

Button *create_button(unsigned int pos_x, unsigned int pos_y, xpm_map_t xpm, void (*on_click)(void)) {
    Button *button = (Button *)malloc(sizeof(Button));

    xpm_image_t image;
    uint8_t *sprite = xpm_load(xpm, XPM_INDEXED, &image);

    *button = (Button) { pos_x, pos_y, image.width, image.height, sprite, on_click };
    return button;
}

Cursor *create_cursor(unsigned int pos_x, unsigned int pos_y, xpm_map_t xpm) {
    Cursor *cursor = (Cursor *)malloc(sizeof(Cursor));

    xpm_image_t image;
    uint8_t *sprite = xpm_load(xpm, XPM_INDEXED, &image);

    *cursor = (Cursor){ pos_x, pos_y, image.width, image.height, sprite};

    return cursor;
}
