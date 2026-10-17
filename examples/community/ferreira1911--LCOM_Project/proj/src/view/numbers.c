#include <lcom/lcf.h>

#include "numbers.h"
#include "devices/video.h"

#include "xpms/numbers/0.xpm"
#include "xpms/numbers/1.xpm"
#include "xpms/numbers/2.xpm"
#include "xpms/numbers/3.xpm"
#include "xpms/numbers/4.xpm"
#include "xpms/numbers/5.xpm"
#include "xpms/numbers/6.xpm"
#include "xpms/numbers/7.xpm"
#include "xpms/numbers/8.xpm"
#include "xpms/numbers/9.xpm"

static xpm_map_t number_xpms[] = {
    (xpm_map_t) zero, (xpm_map_t) one, (xpm_map_t) two, (xpm_map_t) three, (xpm_map_t) four, (xpm_map_t) five, (xpm_map_t) six, (xpm_map_t) seven, (xpm_map_t) eight, (xpm_map_t) nine
};

void draw_number(uint32_t number, int x, int y) {
    char number_str[5];
    sprintf(number_str,"%u", number);

    for(size_t i = 0; number_str[i] != '\0'; i++) {
        int digit = number_str[i] - '0';

        xpm_image_t img;
        img.bytes = xpm_load(number_xpms[digit], XPM_8_8_8, &img);
        vg_draw_xpm(x + i * (DIGIT_WIDTH + DIGIT_SPACING), y, &img);
    }
}


