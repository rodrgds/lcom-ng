#ifndef TARGET_VIEW_H
#define TARGET_VIEW_H

#include <lcom/lcf.h>

#include "target_view.h"

#include "model/target.h"
#include "devices/video.h"

void (draw_target)(const Target *target){
    if(target == NULL || !target->isVisible) return;

    vg_draw_xpm(target->x, target->y, &target->TargetImage);
}

#endif
