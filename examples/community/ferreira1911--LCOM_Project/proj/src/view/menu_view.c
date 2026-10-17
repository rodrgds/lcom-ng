#include <lcom/lcf.h>

#include "menu_view.h"
#include "model/menu.h"
#include "devices/video.h"

void (draw_menu_item)(const MenuItem *item){
    if (item == NULL || !item->isVisible) return;

    vg_draw_xpm(item->x, item->y, &item->ItemImage);
}
