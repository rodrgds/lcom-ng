#include "sprite.h"

Sprite *create_sprite(xpm_map_t xpm) {
    Sprite *sp = (Sprite *) malloc(sizeof(Sprite));

    xpm_image_t img;
    sp->colors = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &img);

    if (sp->colors == NULL) {
        free(sp);
        return NULL;
    }

    sp->width = img.width;
    sp->height = img.height;
    sp->x = 0;
    sp->y = 0;

    return sp;
}

void destroy_sprite(Sprite *sp) {
    if (sp == NULL)
        return;

    if (sp->colors != NULL)
        free(sp->colors);

    free(sp);
}

int draw_sprite(Sprite *sp, int x, int y) {
    for (int h = 0; h < sp->height; h++) {
        for (int w = 0; w < sp->width; w++) {
            uint32_t color = sp->colors[h * sp->width + w];
            if (color != xpm_transparency_color(XPM_8_8_8_8)) {
                if (vg_draw_pixel(x + w, y + h, color)) return 1;
            }
        }
    }
    
    sp->x = x;
    sp->y = y;

    return 0;
}

void load_sprites() {
    cursor = create_sprite((xpm_map_t) cursor_xpm);
    background = create_sprite((xpm_map_t) background_xpm);
    logo = create_sprite((xpm_map_t) logo_xpm);
    
    single_machine = create_sprite((xpm_map_t) single_machine_xpm);
    return_button = create_sprite((xpm_map_t) return_xpm);
    quit = create_sprite((xpm_map_t) quit_xpm);
    undo = create_sprite((xpm_map_t) undo_xpm);
    clock_background = create_sprite((xpm_map_t) clock_xpm);
    
    board_img = create_sprite((xpm_map_t) board_xpm);
    board_dark_img = create_sprite((xpm_map_t) board_dark_xpm);
    sel_img= create_sprite((xpm_map_t) selected_xpm);

    bB = create_sprite((xpm_map_t) bB_xpm);
    bK = create_sprite((xpm_map_t) bK_xpm);
    bN = create_sprite((xpm_map_t) bN_xpm);
    bP = create_sprite((xpm_map_t) bP_xpm);
    bQ = create_sprite((xpm_map_t) bQ_xpm);
    bR = create_sprite((xpm_map_t) bR_xpm);
    wB = create_sprite((xpm_map_t) wB_xpm);
    wK = create_sprite((xpm_map_t) wK_xpm);
    wN = create_sprite((xpm_map_t) wN_xpm);
    wP = create_sprite((xpm_map_t) wP_xpm);
    wQ = create_sprite((xpm_map_t) wQ_xpm);
    wR = create_sprite((xpm_map_t) wR_xpm);

    numbers[0] = create_sprite((xpm_map_t) zero_xpm);
    numbers[1] = create_sprite((xpm_map_t) one_xpm);
    numbers[2] = create_sprite((xpm_map_t) two_xpm);
    numbers[3] = create_sprite((xpm_map_t) three_xpm);
    numbers[4] = create_sprite((xpm_map_t) four_xpm);
    numbers[5] = create_sprite((xpm_map_t) five_xpm);
    numbers[6] = create_sprite((xpm_map_t) six_xpm);
    numbers[7] = create_sprite((xpm_map_t) seven_xpm);
    numbers[8] = create_sprite((xpm_map_t) eight_xpm);
    numbers[9] = create_sprite((xpm_map_t) nine_xpm);

    black_wins = create_sprite((xpm_map_t) black_wins_xpm);
    white_wins = create_sprite((xpm_map_t) white_wins_xpm);
}

void dump_sprites() {
    destroy_sprite(cursor);
    destroy_sprite(background);
    destroy_sprite(logo);
    destroy_sprite(single_machine);
    destroy_sprite(quit);
    destroy_sprite(return_button);
    destroy_sprite(undo);
    destroy_sprite(clock_background);
    destroy_sprite(board_img);
    destroy_sprite(sel_img);
    destroy_sprite(bB);
    destroy_sprite(bK);
    destroy_sprite(bN);
    destroy_sprite(bP);
    destroy_sprite(bQ);
    destroy_sprite(bR);
    destroy_sprite(wB);
    destroy_sprite(wK);
    destroy_sprite(wN);
    destroy_sprite(wP);
    destroy_sprite(wQ);
    destroy_sprite(wR);
    
    for (int i = 0; i < 10; i++) {
        destroy_sprite(numbers[i]);
    }

    destroy_sprite(black_wins);
    destroy_sprite(white_wins);
}
