#include "cursor.h"

/**
 * @brief Represents a mouse cursor instance.
 *
 * The Cursor struct is opaque.
 */
struct Cursor {
    Sprite *sprite;
    CursorMode mode;
};

/* Create and destroy section */

Cursor *create_cursor(CursorMode mode, int x, int y) {
    Cursor *cursor = (Cursor *) malloc(sizeof(Cursor));
    if (cursor == NULL) {
        printf("create_cursor: malloc failed\n");
        return NULL;
    }

    if (mode == CURSOR_DEFAULT) {
        cursor->sprite = create_sprite((xpm_map_t) cursor_xpm, x, y, 0, 0);
    } else {
        cursor->sprite = create_sprite((xpm_map_t) crosshair_xpm, x_res / 2, y_res / 2, 0, 0);
    }

    if (cursor->sprite == NULL) {
        printf("create_cursor: create_sprite failed\n");
        free(cursor);
        return NULL;
    }

    cursor->mode = mode;
    return cursor;
}

int destroy_cursor(Cursor *cursor) {
    if (cursor == NULL) {
        printf("destroy_cursor: NULL pointer provided\n");
        return 1;
    }

    if (destroy_sprite(cursor->sprite)) {
        printf("destroy_cursor: destroy_sprite failed\n");
        return 1;
    }
    free(cursor);
    return 0;
}

/* Getter and setter section */

uint16_t cursor_get_x(Cursor *cursor) {
    if (cursor == NULL) {
        printf("cursor_get_x: NULL pointer provided\n");
        return 0;
    }

    if (cursor->mode == CURSOR_CROSSHAIR) {
        return cursor->sprite->x + cursor->sprite->width / 2;
    }
    
    return cursor->sprite->x;
}

uint16_t cursor_get_y(Cursor *cursor) {
    if (cursor == NULL) {
        printf("cursor_get_y: NULL pointer provided\n");
        return 0;
    }

    if (cursor->mode == CURSOR_CROSSHAIR) {
        return cursor->sprite->y + cursor->sprite->height / 2;
    }
    
    return cursor->sprite->y;
}

/* Statics section */

static inline int clamp(int val, int min, int max) {
    return (val < min) ? min : (val > max) ? max : val;
}

static int cursor_check_bound(Cursor *cursor) {
    if (cursor == NULL) {
        printf("cursor_check_bound: NULL pointer provided\n");
        return 1;
    }

    Sprite *s = cursor->sprite;
    s->x = clamp(s->x, 0, x_res - s->width);
    s->y = clamp(s->y, 0, y_res - s->height);
    return 0;
}

/* Others section */

int cursor_update(Cursor *cursor, double dx, double dy) {
    if (cursor == NULL) {
        printf("cursor_update: NULL pointer provided\n");
        return 1;
    }

    cursor->sprite->x += dx;
    cursor->sprite->y += dy;
    if (cursor_check_bound(cursor)) {
        printf("cursor_update: cursor_check_bound failed\n");
        return 1;
    }
    return 0;
}

/* Draw section */

int draw_cursor(Cursor *cursor, uint8_t *frame_buffer) {
    if (cursor == NULL) {
        printf("draw_cursor: NULL pointer provided\n");
        return 1;
    }

    if (draw_transparent_sprite(cursor->sprite, frame_buffer)) {
        printf("draw_cursor: draw_transparent_sprite failed\n");
        return 1;
    }
    return 0;
}
