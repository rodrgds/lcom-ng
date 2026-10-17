#include <lcom/lcf.h>

#include <stdint.h>

#include "../../devices/graphics/vbe.h"
#include "../../devices/graphics/graphics.h"
#include "../../controller/mouse_controller/mouse_controller.h"
#include "../initialize_pixmaps.h"
#include "cursor_view.h"

extern uint8_t* cursor_pixmap;
extern xpm_image_t cursor_info;

extern int mouse_x;
extern int mouse_y;

void (draw_cursor)() {
    draw_xpm(cursor_pixmap, cursor_info, mouse_x, mouse_y);
}

void (erase_cursor)() {
    draw_rectangle(mouse_x, mouse_y, cursor_info.width, cursor_info.height, BACKGROUND);
}
