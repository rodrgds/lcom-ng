#include <lcom/lcf.h>

#include "target.h"
#include "devices/video.h"

extern unsigned h_res;
extern unsigned v_res;

void (create_target)(Target* target, int16_t x, int16_t y, xpm_map_t image) {
    target->x = x;
    target->y = y;

    target->TargetImage.bytes = xpm_load(image, XPM_8_8_8, &target->TargetImage);
    target->isVisible = true;

    target->width = target->TargetImage.width;
    target->height = target->TargetImage.height;
}



