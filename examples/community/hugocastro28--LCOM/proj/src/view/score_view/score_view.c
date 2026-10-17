#include <lcom/lcf.h>

#include <stdint.h>

#include "../../devices/graphics/vbe.h"
#include "../../devices/graphics/graphics.h"
#include "../initialize_pixmaps.h"
#include "../../view/score_view/score_view.h"
#include "score_view.h"

extern unsigned int score;

extern uint8_t* digit0_pixmap; extern uint8_t* digit1_pixmap; extern uint8_t* digit2_pixmap; extern uint8_t* digit3_pixmap; 
extern uint8_t* digit4_pixmap; extern uint8_t* digit5_pixmap; extern uint8_t* digit6_pixmap;
extern uint8_t* digit7_pixmap; extern uint8_t* digit8_pixmap; extern uint8_t* digit9_pixmap;

extern xpm_image_t digit0_info, digit1_info, digit2_info, digit3_info, digit4_info, 
    digit5_info, digit6_info, digit7_info, digit8_info, digit9_info;

uint8_t* pixmap;
xpm_image_t image_info;

void (get_score_digit_pixmap)(int digit) {
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

void (draw_score)() {
    int score_aux = score;
    int num_digits = 0;
    do {
        score_aux /= 10;
        num_digits++;
    } while (score_aux != 0);
    
    score_aux = score;
    int width = num_digits * DIGIT_WIDTH;
    int score_x = ((1024 - width)/2) + width;
    for (int i = 0; i < num_digits; i++) {
        int digit = score_aux%10;
        score_aux /= 10;
        get_score_digit_pixmap(digit);
        draw_xpm(pixmap, image_info, score_x, SCORE_Y);
        score_x -= DIGIT_WIDTH;
    }
}
