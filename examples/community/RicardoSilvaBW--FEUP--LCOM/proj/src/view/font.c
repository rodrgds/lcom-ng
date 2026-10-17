#include "font.h"

#include <lcom/lcf.h>

#include "../model/sprite.h"
#include "renderer.h"

#include "../../assets/font/font_A.xpm"
#include "../../assets/font/font_B.xpm"
#include "../../assets/font/font_C.xpm"
#include "../../assets/font/font_D.xpm"
#include "../../assets/font/font_E.xpm"
#include "../../assets/font/font_F.xpm"
#include "../../assets/font/font_G.xpm"
#include "../../assets/font/font_H.xpm"
#include "../../assets/font/font_I.xpm"
#include "../../assets/font/font_J.xpm"
#include "../../assets/font/font_K.xpm"
#include "../../assets/font/font_L.xpm"
#include "../../assets/font/font_M.xpm"
#include "../../assets/font/font_N.xpm"
#include "../../assets/font/font_O.xpm"
#include "../../assets/font/font_P.xpm"
#include "../../assets/font/font_R.xpm"
#include "../../assets/font/font_S.xpm"
#include "../../assets/font/font_T.xpm"
#include "../../assets/font/font_U.xpm"
#include "../../assets/font/font_V.xpm"
#include "../../assets/font/font_W.xpm"
#include "../../assets/font/font_X.xpm"
#include "../../assets/font/font_Y.xpm"
#include "../../assets/font/font_Z.xpm"

#include "../../assets/font/font_0.xpm"
#include "../../assets/font/font_1.xpm"
#include "../../assets/font/font_2.xpm"
#include "../../assets/font/font_3.xpm"
#include "../../assets/font/font_4.xpm"
#include "../../assets/font/font_5.xpm"
#include "../../assets/font/font_6.xpm"
#include "../../assets/font/font_7.xpm"
#include "../../assets/font/font_8.xpm"
#include "../../assets/font/font_9.xpm"

#include "../../assets/font/font_space.xpm"
#include "../../assets/font/font_excl.xpm"
#include "../../assets/font/font_dot.xpm"
#include "../../assets/font/font_dash.xpm"
#include "../../assets/font/font_colon.xpm"
#include "../../assets/font/font_slash.xpm"
#include "../../assets/font/font_apos.xpm"

static sprite *letters[128];

static void (draw_sprite_at)(sprite *sp, int x, int y) {
    if (!sp) return;
    int old_x = sp->x;
    int old_y = sp->y;

    sp->x = x;
    sp->y = y;
    draw_sprite(sp);
    
    sp->x = old_x;
    sp->y = old_y;
}

int (load_font)(void) {
    for (int i = 0; i < 128; i++)
        letters[i] = NULL;

    letters['A'] = create_sprite((xpm_map_t) font_A_xpm, 0, 0);
    letters['B'] = create_sprite((xpm_map_t) font_B_xpm, 0, 0);
    letters['C'] = create_sprite((xpm_map_t) font_C_xpm, 0, 0);
    letters['D'] = create_sprite((xpm_map_t) font_D_xpm, 0, 0);
    letters['E'] = create_sprite((xpm_map_t) font_E_xpm, 0, 0);
    letters['F'] = create_sprite((xpm_map_t) font_F_xpm, 0, 0);
    letters['G'] = create_sprite((xpm_map_t) font_G_xpm, 0, 0);
    letters['H'] = create_sprite((xpm_map_t) font_H_xpm, 0, 0);
    letters['I'] = create_sprite((xpm_map_t) font_I_xpm, 0, 0);
    letters['J'] = create_sprite((xpm_map_t) font_J_xpm, 0, 0);
    letters['K'] = create_sprite((xpm_map_t) font_K_xpm, 0, 0);
    letters['L'] = create_sprite((xpm_map_t) font_L_xpm, 0, 0);
    letters['M'] = create_sprite((xpm_map_t) font_M_xpm, 0, 0);
    letters['N'] = create_sprite((xpm_map_t) font_N_xpm, 0, 0);
    letters['O'] = create_sprite((xpm_map_t) font_O_xpm, 0, 0);
    letters['P'] = create_sprite((xpm_map_t) font_P_xpm, 0, 0);
    letters['R'] = create_sprite((xpm_map_t) font_R_xpm, 0, 0);
    letters['S'] = create_sprite((xpm_map_t) font_S_xpm, 0, 0);
    letters['T'] = create_sprite((xpm_map_t) font_T_xpm, 0, 0);
    letters['U'] = create_sprite((xpm_map_t) font_U_xpm, 0, 0);
    letters['V'] = create_sprite((xpm_map_t) font_V_xpm, 0, 0);
    letters['W'] = create_sprite((xpm_map_t) font_W_xpm, 0, 0);
    letters['Y'] = create_sprite((xpm_map_t) font_Y_xpm, 0, 0);
    letters['X'] = create_sprite((xpm_map_t) font_X_xpm, 0, 0);
    letters['Z'] = create_sprite((xpm_map_t) font_Z_xpm, 0, 0);
    letters['0'] = create_sprite((xpm_map_t) font_0_xpm, 0, 0);
    letters['1'] = create_sprite((xpm_map_t) font_1_xpm, 0, 0);
    letters['2'] = create_sprite((xpm_map_t) font_2_xpm, 0, 0);
    letters['3'] = create_sprite((xpm_map_t) font_3_xpm, 0, 0);
    letters['4'] = create_sprite((xpm_map_t) font_4_xpm, 0, 0);
    letters['5'] = create_sprite((xpm_map_t) font_5_xpm, 0, 0);
    letters['6'] = create_sprite((xpm_map_t) font_6_xpm, 0, 0);
    letters['7'] = create_sprite((xpm_map_t) font_7_xpm, 0, 0);
    letters['8'] = create_sprite((xpm_map_t) font_8_xpm, 0, 0);
    letters['9'] = create_sprite((xpm_map_t) font_9_xpm, 0, 0);
    letters[' '] = create_sprite((xpm_map_t) font_space_xpm, 0, 0);
    letters['!'] = create_sprite((xpm_map_t) font_excl_xpm, 0, 0);
    letters['.'] = create_sprite((xpm_map_t) font_dot_xpm, 0, 0);
    letters['-'] = create_sprite((xpm_map_t) font_dash_xpm, 0, 0);
    letters[':'] = create_sprite((xpm_map_t) font_colon_xpm, 0, 0);
    letters['/'] = create_sprite((xpm_map_t) font_slash_xpm, 0, 0);
    letters['\''] = create_sprite((xpm_map_t) font_apos_xpm, 0, 0);

    return 0;
}

void (destroy_font)(void) {
    for (int i = 0; i < 128; i++) {
        if (letters[i] != NULL) {
            destroy_sprite(&letters[i]);
        }
    }
}

void (draw_text)(const char *text, int x, int y) {
    int cursor_x = x;
    for (int i = 0; text[i] != '\0'; i++) {
        sprite *sp = letters[(unsigned char) text[i]];

        if (sp != NULL) {
            draw_sprite_at(sp, cursor_x, y);
            cursor_x += sp->width + 1;
        }
        else {
            cursor_x += 4;
        }
    }
}

int (text_width)(const char *text) {
    int width = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        sprite *sp = letters[(unsigned char) text[i]];
        if (sp != NULL)
            width += sp->width + 1;
        else
            width += 4;
    }

    return width;
}

void (draw_text_centered)(const char *text, int center_x, int y) {
    int width = text_width(text);

    draw_text(
        text,
        center_x - width / 2,
        y
    );
}
