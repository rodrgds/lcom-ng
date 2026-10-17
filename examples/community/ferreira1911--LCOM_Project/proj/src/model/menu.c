#include <lcom/lcf.h>

#include "menu.h"


void (create_menu_item)(MenuItem* item, int16_t x, int16_t y, xpm_map_t image) {
    item->x = x;
    item->y = y;

    item->ItemImage.bytes = xpm_load(image, XPM_8_8_8, &item->ItemImage);
    item->isVisible = true;

    item->width = item->ItemImage.width;
    item->height = item->ItemImage.height;
}
