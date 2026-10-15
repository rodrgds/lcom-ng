#include <lcom/lcf.h>
#include <stdint.h>

#include "../../include/macros/mouse.h"
#include "../../include/ui/mouse_trail.h"
#include "../../include/ui/ui.h"

static int trail_x[MOUSE_TRAIL_LENGTH];
static int trail_y[MOUSE_TRAIL_LENGTH];
static uint8_t trail_size = 0;
static uint8_t trail_frames_left = 0;

static uint32_t mouse_trail_color(uint8_t index, uint8_t count) {
    if (count <= 1) return MOUSE_SLASH_1;

    uint8_t bucket = (index * 4) / count;

    switch (bucket) {
        case 0:
            return MOUSE_SLASH_1;
        case 1:
            return MOUSE_SLASH_2;
        case 2:
            return MOUSE_SLASH_3;
        default:
            return MOUSE_SLASH_4;
    }
}

static uint8_t mouse_trail_width(uint8_t segment_index, uint8_t segment_count) {
    if (segment_count <= 1) return MOUSE_TRAIL_MIN_WIDTH;

    uint16_t middle_distance = segment_index * 2;
    uint16_t max_distance = segment_count - 1;

    if (middle_distance > max_distance) {
        middle_distance = 2 * max_distance - middle_distance;
    }

    return MOUSE_TRAIL_MIN_WIDTH +
           ((MOUSE_TRAIL_WIDTH - MOUSE_TRAIL_MIN_WIDTH) * middle_distance) / max_distance;
}

static int mouse_trail_draw_thick_pixel(int x, int y, uint8_t width, uint32_t color) {
    return ui_draw_rectangle(x - width / 2,
                             y - width / 2,
                             width,
                             width,
                             color);
}

static int mouse_trail_draw_segment(int x0, int y0, int x1, int y1, uint8_t width, uint32_t color) {
    int dx = x1 > x0 ? x1 - x0 : x0 - x1;
    int sx = x0 < x1 ? 1 : -1;
    int dy = y1 > y0 ? y0 - y1 : y1 - y0;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while (true) {
        if (mouse_trail_draw_thick_pixel(x0, y0, width, color) != 0) return 1;
        if (x0 == x1 && y0 == y1) break;

        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }

    return 0;
}

int mouse_trail_draw(MouseGameState *mouse_state) {
    if (mouse_state == NULL) return 1;
    if (trail_frames_left == 0) return 0;

    for (int i = trail_size - 1; i > 0; i--) {
        uint32_t color = mouse_trail_color((uint8_t)i, trail_size);
        uint8_t width = mouse_trail_width((uint8_t)(trail_size - 1 - i), trail_size - 1);

        if (mouse_trail_draw_segment(trail_x[i],
                                     trail_y[i],
                                     trail_x[i - 1],
                                     trail_y[i - 1],
                                     width,
                                     color) != 0) {
            return 1;
        }
    }

    trail_frames_left--;
    return 0;
}

static void mouse_trail_clear(void) {
    trail_size = 0;
    trail_frames_left = 0;
}

void mouse_trail_update(const MouseGameState *mouse_state) {
    if (mouse_state == NULL) return;

    if (!mouse_state->lb_pressed) {
        mouse_trail_clear();
        return;
    }

    if (mouse_state->dx == 0 && mouse_state->dy == 0) return;

    for (int i = MOUSE_TRAIL_LENGTH - 1; i > 0; i--) {
        trail_x[i] = trail_x[i - 1];
        trail_y[i] = trail_y[i - 1];
    }

    trail_x[0] = mouse_state->x + MOUSE_CURSOR_CENTER_OFFSET_X;
    trail_y[0] = mouse_state->y + MOUSE_CURSOR_CENTER_OFFSET_Y;

    if (trail_size < MOUSE_TRAIL_LENGTH) {
        trail_size++;
    }

    trail_frames_left = trail_size > 1 ? MOUSE_TRAIL_VISIBLE_FRAMES : 0;
}
