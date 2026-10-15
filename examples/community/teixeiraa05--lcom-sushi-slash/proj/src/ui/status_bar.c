#include "../../include/ui/status_bar.h"

#include <stdio.h>

#include "../../include/macros/menu.h"
#include "../../include/macros/global.h"

int ui_status_bar_draw(const Assets *assets, const StatusBarInfo *status) {
    if (assets == NULL || status == NULL || status->date == NULL || status->time == NULL) return 1;

    char time_status[20];
    char fps_status[16];

    snprintf(time_status, sizeof(time_status), "%02u/%02u/%02u %02u:%02u:%02u",
             (unsigned) status->date->day,
             (unsigned) status->date->month,
             (unsigned) status->date->year,
             (unsigned) status->time->hours,
             (unsigned) status->time->minutes,
             (unsigned) status->time->seconds);
    snprintf(fps_status, sizeof(fps_status), "fps %u", status->fps);

    uint16_t fps_width = font_half_text_width(&assets->game_font, fps_status);
    uint16_t fps_x = SCREEN_WIDTH - MENU_STATUS_RIGHT_MARGIN - fps_width;

    if (font_draw_half_text_color(&assets->game_font,
                                  time_status,
                                  MENU_STATUS_X,
                                  MENU_STATUS_Y,
                                  MENU_STATUS_COLOR) != 0) {
        return 1;
    }

    return font_draw_half_text_color(&assets->game_font,
                                     fps_status,
                                     fps_x,
                                     MENU_STATUS_Y,
                                     MENU_STATUS_COLOR);
}
