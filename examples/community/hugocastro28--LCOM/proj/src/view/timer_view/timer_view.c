#include <lcom/lcf.h>

#include <stdint.h>

#include "../../devices/graphics/vbe.h"
#include "../../devices/graphics/graphics.h"
#include "../initialize_pixmaps.h"
#include "../../controller/timer_controller/timer_controller.h"
#include "timer_view.h"

extern unsigned int timer_minutes;
extern unsigned int timer_seconds;

extern uint8_t* digit0_pixmap; extern uint8_t* digit1_pixmap; extern uint8_t* digit2_pixmap; extern uint8_t* digit3_pixmap; 
extern uint8_t* digit4_pixmap; extern uint8_t* digit5_pixmap; extern uint8_t* digit6_pixmap;
extern uint8_t* digit7_pixmap; extern uint8_t* digit8_pixmap; extern uint8_t* digit9_pixmap;
extern uint8_t* time_divider_pixmap;

extern xpm_image_t digit0_info, digit1_info, digit2_info, digit3_info, digit4_info, 
    digit5_info, digit6_info, digit7_info, digit8_info, digit9_info;
extern xpm_image_t time_divider_info;

uint8_t* pixmap;
xpm_image_t image_info;

void (get_timer_digit_pixmap)(int digit) {
    switch (digit) {
        case 0: pixmap = digit0_pixmap; image_info = digit0_info;
            break;
        case 1: pixmap = digit1_pixmap; image_info = digit1_info;
            break;
        case 2: pixmap = digit2_pixmap; image_info = digit2_info;
            break;
        case 3: pixmap = digit3_pixmap; image_info = digit3_info;
            break;
        case 4: pixmap = digit4_pixmap; image_info = digit4_info;
            break;
        case 5: pixmap = digit5_pixmap; image_info = digit5_info;
            break;
        case 6: pixmap = digit6_pixmap; image_info = digit6_info;
            break;
        case 7: pixmap = digit7_pixmap; image_info = digit7_info;
            break;
        case 8: pixmap = digit8_pixmap; image_info = digit8_info;
            break;
        case 9: pixmap = digit9_pixmap; image_info = digit9_info;
            break;
        default:
            break;
    }
}

void (draw_timer)() {
    get_timer_digit_pixmap(timer_minutes);
    draw_xpm(pixmap, image_info, MIN_X, TIMER_Y);

    draw_xpm(time_divider_pixmap, time_divider_info, TIME_DIV_X, TIMER_Y);

    get_timer_digit_pixmap(timer_seconds/10);
    draw_xpm(pixmap, image_info, SEC_MSD_X, TIMER_Y);

    get_timer_digit_pixmap(timer_seconds%10);
    draw_xpm(pixmap, image_info, SEC_LSD_X, TIMER_Y);
}

void (erase_timer)() {
    draw_rectangle(TIMER_X, TIMER_Y, TIMER_WIDTH, TIMER_HEIGHT, BACKGROUND);
}
