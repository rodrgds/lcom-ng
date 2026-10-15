#include "cursor.h"

Cursor *createCursor(Sprite *sprite) {
    Cursor *cursor = (Cursor *) malloc(sizeof(Cursor));
    if (cursor == NULL) {
        return NULL; // Memory allocation failed
    }
    cursor->x = 800/2;
    cursor->y = 600/2;
    cursor->sprite = sprite;
    return cursor;
}

void destroyCursor(Cursor *cursor) {
    if (cursor != NULL) {
        if (cursor->sprite != NULL) {
            destroy_sprite(cursor->sprite);
        }
        free(cursor);
    }
}

int drawCursor(Cursor *cursor) {
    if (cursor != NULL && cursor->sprite != NULL) {
        draw_sprite(cursor->sprite, cursor->x, cursor->y);
        return 0; 
    }
    return 1;
}

void (cursor_update_location)(Cursor *cursor, struct packet *mouse_packet) {
    cursor->x+= mouse_packet->delta_x;
    cursor->y-= mouse_packet->delta_y;
    if (cursor->x < 0) cursor->x = 0;
    if (cursor->x > 800 - cursor->sprite->width) cursor->x = 800 - cursor->sprite->width; // 800 - cursor->sprite->width
    if (cursor->y < 0) cursor->y = 0;
    if (cursor->y > 600 - cursor->sprite->height) cursor->y = 600 - cursor->sprite->height; // 600 - cursor->sprite->height
}
