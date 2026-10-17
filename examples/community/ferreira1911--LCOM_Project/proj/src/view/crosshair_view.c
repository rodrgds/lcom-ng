#include <lcom/lcf.h>

#include "crosshair_view.h"
#include "model/crosshair.h"
#include "devices/video.h"

extern Crosshair crosshair;

extern char *video_mem;
extern unsigned h_res;
extern unsigned v_res;
extern unsigned bits_per_pixel;


void (draw_crosshair)(){
    vg_draw_hline(crosshair.x, (crosshair.y + (crosshair.height / 2)), crosshair.width, CROSSHAIR_COLOR);

    vg_draw_vline((crosshair.x + (crosshair.width / 2)), crosshair.y, crosshair.height, CROSSHAIR_COLOR);
}
